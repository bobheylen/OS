#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdbool.h>
#include "config.h"
#include "datamgr.h"
#include "sensor_db.h"
#include "connmgr.h"
#include "sbuffer.h"
#include "lib/tcpsock.h"
#include "lib/dplist.h"

/**
 * basic node for the buffer, these nodes are linked together to create the buffer
 */
typedef struct sbuffer_node {
    struct sbuffer_node *next;  /**< a pointer to the next node*/
    sensor_data_t data;         /**< a structure containing the data */
} sbuffer_node_t;

/**
 * a structure to keep track of the buffer
 */
struct sbuffer {
    sbuffer_node_t *head;       /**< a pointer to the first node in the buffer */
    sbuffer_node_t *tail;       /**< a pointer to the last node in the buffer */
};

/**
 * Global Variables
 **/
sbuffer_t *sbuffer;
pthread_mutex_t mutex;
int fd[2];              /**< an array to store read and write end of the pipe */
bool available;         /**< indicator: still data available in sbuffer? */
bool datamgr_ready;     /**< indicator: datamgr has put it in dplist, now stormgr can sbuffer_remove it */

void print_help(void);

void *connmgr(void *arg)
{
    printf("Reached connmgr thread!\n");

    // make int-pointer from void-pointer
    int* port_nr = (int*)arg;

    // startup connection manager
    test_server(*port_nr, sbuffer);

    printf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n&&&&&&& EXIT CONNMGR &&&&&&&\n&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
    pthread_exit(0);
}

void *datamgr(void *arg)
{
    printf("Reached datamgr thread!\n");

    // reading out room_sensor.map and parse sensors into dplist
    FILE * map = fopen("room_sensor.map", "r");
    datamgr_parse_sensor_files(map);
    fclose(map);
    //printf("reading room_sensor.map done!\n");

    // fetch sensor data from sbuffer into dplist
    sensor_data_t data;
    while(!available);
    while(available){
        while(datamgr_ready);
        while(sbuffer->head==NULL);
        pthread_mutex_lock(&mutex);
        data = (sbuffer->head)->data;
        pthread_mutex_unlock(&mutex);

        // detect end of data indicator
        if(data.id==0){
            datamgr_ready=true;
            available=false;
            break;
        }

        // checks whether sensor from sbuffer is a valid one
        // -> insert if valid
        // -> remove if invalid
        if(datamgr_check_new_sensor(data)==0){
            datamgr_parse_sensor_data(data);
            printf("Data inserted successfully in dplist!\n");
            datamgr_ready=true;
        }
        else{
            pthread_mutex_lock(&mutex);
            sensor_data_t *element = malloc(sizeof(sensor_data_t));
            sbuffer_remove(sbuffer, element);
            free(element);
            pthread_mutex_unlock(&mutex);
        }
    }

    // free dplist
    datamgr_free();

    printf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n&&&&&&& EXIT DATAMGR &&&&&&&\n&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
    pthread_exit(0);
}

void *stormgr(void *arg)
{
    printf("Reached stormgr thread!\n");

    // open data.csv to overwrite
    FILE *file = open_db("data.csv", false);

    // fetch sensor data from sbuffer into data.csv
    while(!available);
    while(available){
        while(!datamgr_ready);
        pthread_mutex_lock(&mutex);
        sensor_data_t *element = malloc(sizeof(sensor_data_t));
        int buffer_check = sbuffer_remove(sbuffer, element);
        datamgr_ready = false;
        
        // buffer_check = SBUFFER_SUCCESS
        if(buffer_check==0){
            insert_sensor(file, element->id, element->value, element->ts);
            printf("Data inserted successfully in data.csv!\n");
            free(element);
            pthread_mutex_unlock(&mutex);
        }

        // buffer_check = SBUFFER_NO_DATA
        else if(buffer_check==1){
            available = false;
            free(element);
            pthread_mutex_unlock(&mutex);
            break;
        }

        // buffer_check = SBUFFER_FAILURE
        else{
            printf("Error occurred stormgr (sbuffer)\n");
            free(element);
            pthread_mutex_unlock(&mutex);
            break;
        }
    }

    // close data.csv
    close_db(file);

    printf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n&&&&&&& EXIT STORMGR &&&&&&&\n&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    printf("Reached main.c\n");
    
    // checks whether port_nr is a valid one (command line argument)
    int port_nr;
    if (argc != 2) {
        print_help();
        exit(EXIT_SUCCESS);
        return -1;
    } else {
        port_nr = atoi(argv[1]);
        if(port_nr > MAX_PORT || port_nr < MIN_PORT){
            print_help();
            printf("\t\t\t  --> INVALID PORT NUMBER: Need to pass port number from 1024 to 65536\n");
            exit(EXIT_SUCCESS);
        }
        printf("listening on port: %d\n",port_nr);
    }

    // make pipe variable
    pid_t pid;

    // create the pipe
    if (pipe(fd) == -1){
        printf("Pipe failed\n");
        return 1;
    }

    // fork the child
    pid = fork();
    if (pid < 0){ // fork error
        printf("fork failed\n");
        return 1;
    }

    if (pid > 0){ // PARENT PROCESS

        close(fd[READ_END]); // parent doesn't need reading end

        // init mutex
        pthread_mutex_init(&mutex, NULL);

        // init sbuffer
        if(sbuffer_init(&sbuffer)==0) printf("sbuffer init succesfully!\n");
        else printf("sbuffer init failed\n");

    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CREATE AND JOIN 3 MAIN THREADS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

        // start threads
        pthread_t threads[3];
        pthread_attr_t attr;

        if(pthread_create(&threads[0], &attr, connmgr, &port_nr)!=0){
            printf("Can't create connection manager thread!\n");
        } else {
            printf("Connection manager thread created succesfully!\n");
        }
        if(pthread_create(&threads[1], &attr, datamgr, NULL)!=0){
            printf("Can't create data manager thread!\n");
        } else {
            printf("Data manager thread created succesfully!\n");
        }
        if(pthread_create(&threads[2], &attr, stormgr, NULL)!=0){
            printf("Can't create storage manager thread!\n");
        } else {
            printf("Storage manager thread created succesfully!\n");
        }

        // joining threads
        for(int i=0; i<3; i++){
            if(pthread_join(threads[i], NULL)!=0){
                printf("Can't join thread!\n");
            }
            else{
                printf("Thread %d joined succesfully!\n", i);
            }
        }

    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< CREATE AND JOIN 3 MAIN THREADS <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

        // free sbuffer
        if(sbuffer_free(&sbuffer)==0) printf("sbuffer freed succesfully!\n");
        else printf("sbuffer freed failed!\n");

        close(fd[WRITE_END]);
        wait(NULL);
        exit(EXIT_SUCCESS);
    }
    else{ // CHILD PROCESS

        close(fd[WRITE_END]); // child doesn't need writing end

        // make new gateway.log
        FILE *log_file = fopen("gateway.log", "w");
        fclose(log_file);

        // init sequence_nr
        int sequence_nr = 0;

        // keep receiving as long as parent keeps writing
        char rmsg[SIZE];
        while (true)
        {
            int read_check = read(fd[READ_END], rmsg, SIZE);
            if(read_check<=0){ // stop receiving, because parent no longer sending
                break;
            }

            // open gateway.log to append
            log_file = fopen("gateway.log", "a");

            // write message in gateway.log
            fprintf(log_file, "%d %ld %s\n", sequence_nr, time(0), rmsg);

            // close gateway.log 
            fclose(log_file);

            // increment sequence_nr
            sequence_nr++;
        }

        close(fd[READ_END]);
        exit(EXIT_SUCCESS);
    }
}

/**
 * Helper method to print a message on how to use this application
 */
void print_help(void) {
    printf("Use this program with 1 command line options: \n");
    printf("\t%-15s : TCP server port number\n", "\'server port\'");
}

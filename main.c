#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include "sbuffer.h"

#define CONN 3

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

sbuffer_t *sbuffer;
sbuffer_node_t sbuffer_node;
pthread_mutex_t mutex;
extern bool available;          // still data available in sbuffer?
bool start = false;             // start reading from sbuffer?

void *writer(){
    printf("Reached writer\n");

    // opening binary file in read binary mode
    FILE *fp_sensor_data = fopen("sensor_data", "rb");

    // seek to end of file
    fseek(fp_sensor_data, 0L, SEEK_END);

    // calculating the size of the file
    int binary_size = ftell(fp_sensor_data)/sizeof(sensor_data_t);
    printf("Size of binary file = %d\n", binary_size);

    // seek to beginning of file
    fseek(fp_sensor_data, 0L, SEEK_SET);
    
    // init array of sensor_data_t with size of binary file
    sensor_data_t data[binary_size];

    fread(&data, sizeof(sensor_data_t), binary_size, fp_sensor_data);
    for(int k=0; k < binary_size; k++){
        //printf("id = %d, value = %f, timestamp = %ld\n", data[k].id, data[k].value, data[k].ts); // test
        sbuffer_insert(sbuffer, &data[k]);
    }

    // end of data indicator
    sensor_data_t end_data;
    end_data.id=0;
    sbuffer_insert(sbuffer, &end_data);

    // closing binary file
    fclose(fp_sensor_data);

    // start reading of reader threads
    start = true;

    // exit thread
    pthread_exit(0);
}


void *reader(){
    printf("Reached reader\n");

    // opening file in append mode
    FILE *fp_sensor_data_out = fopen("sensor_data_out", "a");

    // start reading from sbuffer?
    while(!start){
        // wait until writer is ready --> start=true
    }

    // still data available in sbuffer?
    while(available){
        pthread_mutex_lock(&mutex);
        sensor_data_t *element = malloc(sizeof(sensor_data_t));
        int buffer_check = sbuffer_remove(sbuffer, element);

        // buffer_check = SBUFFER_SUCCESS
        if(buffer_check==0){
            fprintf(fp_sensor_data_out, "id=%d\t\tvalue=%f\t\ttimestamp=%ld\n", (*element).id, (*element).value, (*element).ts);
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
            printf("Error occurred!");
            free(element);
            pthread_mutex_unlock(&mutex);
            break;
        }
    }

    // closing file
    fclose(fp_sensor_data_out);

    // exit thread
    pthread_exit(0);
}


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> THREAD CHECK >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
/*
void *reader(void *r){
    printf("Reached reader\n");

    // convert to integer
    int beurt = atoi(r);

    // opening file in append mode
    FILE *fp_sensor_data_out = fopen("sensor_data_out", "a");

    // start reading from sbuffer?
    while(!start){
        // wait until writer is ready --> start=true
    }

    // still data available in sbuffer?
    while(available){
        pthread_mutex_lock(&mutex);
        sensor_data_t *element = malloc(sizeof(sensor_data_t));
        int buffer_check = sbuffer_remove(sbuffer, element);

        // buffer_check = SBUFFER_SUCCESS
        if(buffer_check==0){
            fprintf(fp_sensor_data_out, "id=%d\t\tvalue=%f\t\ttimestamp=%ld\t\tthread: %d\n",(*element).id, (*element).value, (*element).ts, beurt);
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
            printf("Error occurred!");
            free(element);
            pthread_mutex_unlock(&mutex);
            break;
        }
    }

    // closing file
    fclose(fp_sensor_data_out);

    // exit thread
    pthread_exit(0);
}
*/
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< THREAD CHECK <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

int main(){

    // init sbuffer
    if(sbuffer_init(&sbuffer)!=0) return -1;

    // init mutex
    pthread_mutex_init(&mutex, NULL);

    // start threads
    pthread_t tid[CONN];
    pthread_attr_t attr;

    pthread_create(&tid[0], &attr, writer, NULL);
    pthread_create(&tid[1], &attr, reader, NULL);
    pthread_create(&tid[2], &attr, reader, NULL);

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// >>>>>>>>>>>>>>>> THREAD CHECK >>>>>>>>>>>>>>>>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
/*
    void *r1 = "1";
    void *r2 = "2";
    pthread_create(&tid[0], &attr, writer, NULL);
    pthread_create(&tid[1], &attr, reader, r1);
    pthread_create(&tid[2], &attr, reader, r2);
*/
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// <<<<<<<<<<<<<<<< THREAD CHECK <<<<<<<<<<<<<<<<
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    // threads joining
    for(int i=0; i<CONN; i++){
        if(pthread_join(tid[i], NULL)!=0){
            printf("Can't join thread!\n");
        }
        else{
            printf("Thread joined succesfully!\n");
        }
    }

    // destroy mutex
    pthread_mutex_destroy(&mutex);

    // free sbuffer
    if(sbuffer_free(&sbuffer)!=0) return -1;

    return 0;
}

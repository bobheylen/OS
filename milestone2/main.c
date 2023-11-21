#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <aio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "sensor_db.h"
#include "sensor_db.c"

#define SIZE 150
#define READ_END 0
#define WRITE_END 1

int main(void){
    char wmsg[SIZE];
    char rmsg[SIZE];
    pid_t pid;
    extern int fd[2];

    int sequence_nr = 0;

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
    
        close(fd[READ_END]);
        /*
        // Dit geeft nog een error, maar waarom 2x file closen?
        FILE *file = open_db("sensor_data.csv", true);
        insert_sensor(file, 69, 69.69, time(0));
        insert_sensor(file, 1, 1.0, time(0));
        close_db(file);
        close_db(file);
        */

//#################### file=NULL situation ####################

        // open file
        FILE *file0 = open_db(NULL, true);

        // insert 5 sensors
        for(int i=1; i<=5; i++){
            insert_sensor(file0, i, 1.45*i, time(0));
        }

        // close file
        close_db(file0);

//#################### fule=NULL situation ####################

//#################### Normal situation append ####################

        // open file
        FILE *file = open_db("sensor_data.csv", true);

        // insert 5 sensors
        for(int i=1; i<=5; i++){
            insert_sensor(file, i, 1.45*i, time(0));
        }

        // close file
        close_db(file);

//#################### Normal situation append ####################

//#################### Normal situation overwrite ####################

        // open file
        FILE *file2 = open_db("sensor_data2.csv", false);

        // insert 5 sensors
        for(int i=1; i<=5; i++){
            insert_sensor(file2, i, 2.45*i, time(0));
        }

        // close file
        close_db(file2);

//#################### Normal situation overwrite ####################
        
        close(fd[WRITE_END]);
        wait(NULL);
    }
    else{ // CHILD PROCESS

        close(fd[WRITE_END]);

        // open gateway.log
        FILE *log_file = fopen("gateway.log", "a");

        // keep receiving as long as parent keeps writing
        bool keep = true;
        while (keep)
        {
            int read_check = read(fd[READ_END], rmsg, SIZE);
            if(read_check<=0){ // stop receiving, because parent no longer sending
                keep = false;
                break;
            }

            // write message in gateway.log
            fprintf(log_file, "%d %ld %s\n", sequence_nr, time(0), rmsg);

            // increment sequence_nr
            sequence_nr++;
        }

        fclose(log_file);
        close(fd[READ_END]);
        exit(0);

    }
    
    return 0;

}
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "config.h"
#include "sensor_db.h"

char wmsg_stormgr[SIZE];

FILE * open_db(char * filename, bool append){

    FILE *file;

    // Append file when existing and create new when file doesn't exist
    if(append) {
        file = fopen(filename, "a");

        // file = NULL
        if(file == NULL) {
            sprintf(wmsg_stormgr, "Failed to open csv file to append: filename = NULL");
            write(fd[WRITE_END], wmsg_stormgr, SIZE);
            return NULL;
        }

        // file != NULL
        sprintf(wmsg_stormgr, "A new data.csv file has been created)");
        write(fd[WRITE_END], wmsg_stormgr, SIZE);
    }

    // Overwrite file when existing and create new when file doesn't exist
    else{
        file = fopen(filename, "w");

        // file = NULL
        if(file == NULL) {
            sprintf(wmsg_stormgr, "Failed to open csv file to overwrite: filename = NULL");
            write(fd[WRITE_END], wmsg_stormgr, SIZE);
            return NULL;
        }

        // file != NULL
        sprintf(wmsg_stormgr, "A new data.csv file has been created");
        write(fd[WRITE_END], wmsg_stormgr, SIZE);
    }

    return file;
}

int insert_sensor(FILE * f, sensor_id_t id, sensor_value_t value, sensor_ts_t ts){

    // file = NULL
    if(f == NULL) {
        sprintf(wmsg_stormgr, "Data insertion failed: filename = NULL");
        write(fd[WRITE_END], wmsg_stormgr, SIZE);
        return -1;
    }

    // Insert sensor in .csv file
    int r = fprintf(f, "%d,%f,%ld\n", id, value, ts);

    // Failed to insert
    if(r<0) {
        sprintf(wmsg_stormgr, "Data insertion from sensor %d failed (id: %d, value: %f, timestamp: %ld)", id, id, value, ts);
        write(fd[WRITE_END], wmsg_stormgr, SIZE);
        return -1;
    }

    // Succeeded to insert
    else{
        sprintf(wmsg_stormgr, "Data insertion from sensor %d succeeded (id: %d, value: %f, timestamp: %ld)", id, id, value, ts);
        write(fd[WRITE_END], wmsg_stormgr, SIZE);
        return r;
    }
}

int close_db(FILE * f){

    // f = NULL
    if(f == NULL) {
        sprintf(wmsg_stormgr, "Failed to close data.csv file: file = NULL");
        write(fd[WRITE_END], wmsg_stormgr, SIZE);
        return -1;
    }

    int close_check = fclose(f);
    
    // Succeeded to close file
    if(close_check==0) {
        sprintf(wmsg_stormgr, "The data.csv has been closed");
        write(fd[WRITE_END], wmsg_stormgr, SIZE);
        return 0;
    }
    
    // Failed to close file
    else {
        sprintf(wmsg_stormgr, "Failed to close data.csv file");
        write(fd[WRITE_END], wmsg_stormgr, SIZE);
        return -1;
    }

}

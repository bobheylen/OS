#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "config.h"
#include "sensor_db.h"

int fd[2];
#define SIZE 150
#define READ_END 0
#define WRITE_END 1

FILE * open_db(char * filename, bool append){

    FILE *file;

    // Append file when existing and create new when file doesn't exist
    if(append) {
        file = fopen(filename, "a");

        // file = NULL
        if(file == NULL) {
            char dest[SIZE] = "Failed to open csv file to append: filename = NULL";
            write(fd[WRITE_END], dest, SIZE);
            return NULL;
        }

        // file != NULL 
        char dest[SIZE] = "csv file opened to append: ";
        strcat(dest, filename);
        write(fd[WRITE_END], dest, SIZE);
    }

    // Overwrite file when existing and create new when file doesn't exist
    else{
        file = fopen(filename, "w");

        // file = NULL
        if(file == NULL) {
            char dest[SIZE] = "Failed to open csv file to overwrite: filename = NULL";
            write(fd[WRITE_END], dest, SIZE);
            return NULL;
        }

        // file != NULL
        char dest[SIZE] = "csv file opened to overwrite: ";
        strcat(dest, filename);
        write(fd[WRITE_END], dest, SIZE);
    }

    return file;
}

int insert_sensor(FILE * f, sensor_id_t id, sensor_value_t value, sensor_ts_t ts){

    // file = NULL
    if(f == NULL) {
        char dest[SIZE] = "Failed to insert sensor in csv file: filename = NULL";
        write(fd[WRITE_END], dest, SIZE);
        return -1;
    }

    // Insert sensor in .csv file
    int r = fprintf(f, "%d,%0.2f,%ld\n", id, value, ts);

    // Failed to insert
    if(r<0) {
        char dest[SIZE];
        sprintf(dest, "Failed to insert sensor in csv file - id: %d, value: %0.2f, timestamp: %ld", id, value, ts);
        write(fd[WRITE_END], dest, SIZE);
        return -1;
    }

    // Succeeded to insert
    else{
        char dest[SIZE];
        sprintf(dest, "Succeeded to insert sensor in csv file - id: %d, value: %0.2f, timestamp: %ld", id, value, ts);
        write(fd[WRITE_END], dest, SIZE);
        return r;
    }
}

int close_db(FILE * f){

    // f = NULL
    if(f == NULL) {
        char dest[SIZE] = "Failed to close csv file: file = NULL";
        write(fd[WRITE_END], dest, SIZE);
        return -1;
    }

    int close_check = fclose(f);
    
    // Succeeded to close file
    if(close_check==0) {
        char dest[SIZE] = "csv file closed succesfully";
        write(fd[WRITE_END], dest, SIZE);
        return 0;
    }
    
    // Failed to close file
    else {
        char dest[SIZE] = "Failed to close csv file";
        write(fd[WRITE_END], dest, SIZE);
        return -1;
    }

}

#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include <stdbool.h>

FILE * open_db(char * filename, bool append){
    printf("I'm in function open_db!\n");

    FILE *file;
    if(append){
        file = fopen(filename, "a");
        //if(file == NULL) return NULL;
    }
    else{
        file = fopen(filename, "w");
        //if(file == NULL) return NULL;
    }

    return file;
}

int insert_sensor(FILE * f, sensor_id_t id, sensor_value_t value, sensor_ts_t ts){
    printf("I'm in fucntion insert_sensor!\n");

    int r = fprintf(f, "%d,%f,%ld\n", id, value, ts);
    if(r<0) return -1;
    else return r;
}

int close_db(FILE * f){
    printf("I'm in function close_db!\n");

    if(f == NULL) return -1;
    fclose(f);

    return 0;
}
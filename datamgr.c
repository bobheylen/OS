#define _GNU_SOURCE
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include "datamgr.h"
#include "lib/dplist.h"
#include <ctype.h>


typedef struct sensor{
    sensor_id_t sensor_id;
    uint16_t room_id;
    sensor_ts_t timestamp;
    double running_avg;
    double values[RUN_AVG_LENGTH];
    int index;
} sensor_t;

#ifndef SET_MIN_TEMP
#define SET_MIN_TEMP 10
#endif

#ifndef SET_MAX_TEMP
#define SET_MAX_TEMP 20
#endif

char wmsg_datamgr[SIZE];
dplist_t *list;

void* element_copy(void * element);
void element_free(void ** element);
int element_compare(void * x, void * y);


void* element_copy(void * element){
    sensor_t* copy = malloc(sizeof (sensor_t));
    *copy = *((sensor_t *) element);
    return (void *) copy;
}

void element_free(void ** element){
    free((sensor_t*)*element);
    *element = NULL;
}

int element_compare(void * x, void * y){
    return ((((sensor_t*)x)->sensor_id < ((sensor_t*)y)->sensor_id) ? -1 : (((sensor_t*)x)->sensor_id == ((sensor_t*)y)->sensor_id) ? 0 : 1);
}

void datamgr_parse_sensor_files(FILE *fp_sensor_map){
    printf("reading files\n");

    // create dplist
    list = dpl_create(element_copy, element_free, element_compare);
    
    // uitlezen van room_sensor.map en in dplist stoppen
    const unsigned MAX_LENGTH = sizeof(*fp_sensor_map);
    char buffer[MAX_LENGTH];
    char room[4];
    char sensor[4];
    int i=0;
    int n=0;

    while (fgets(buffer, MAX_LENGTH, fp_sensor_map)){
        //printf("%s", buffer);
        while(buffer[i]!=' ' && buffer[i]!='\n'){
            room[i]=buffer[i];
            i++;
        }
        i++;
        while(buffer[i]!=' ' && buffer[i]!='\n'){
            sensor[n]=buffer[i];
            i++;
            n++;
        }
        sensor_t *element = malloc(sizeof(sensor_t));
        element->room_id=atoi(room); // convert array (char room[4]) to integer
        element->sensor_id=atoi(sensor); // convert array (char sensor[4]) to integer
        dpl_insert_at_index(list, element, 0, false);
        
        // reset, to start over again
        i=0;
        n=0;
    }
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> TESTING! >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
/*
    // check datamgr_parse_sensor_files
    printf("size of list = %d\n", dpl_size(list));
    for(int x=0; x<dpl_size(list); x++){
        sensor_t *element = dpl_get_element_at_index(list,x);
        printf("sensor_id %d = %d\n", x, element->sensor_id);
        printf("room_id %d = %d\n", x, element->room_id);
    }
*/
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< TESTING! <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
}

int datamgr_check_new_sensor(sensor_data_t sensor_data_sbuffer){
    sensor_t *sensor_data_dplist;
    for(int i=0; i<dpl_size(list); i++){
        sensor_data_dplist = dpl_get_element_at_index(list, i);
        if(sensor_data_dplist->sensor_id==sensor_data_sbuffer.id){
            return 0;
        }
    }
    sprintf(wmsg_datamgr, "Received sensor data with invalid sensor node ID %d", sensor_data_sbuffer.id);
    write(fd[WRITE_END], wmsg_datamgr, SIZE);

    return -1;
}

void datamgr_parse_sensor_data(sensor_data_t sensor_data_sbuffer){

    // uitlezen van sbuffer en id's van sensor_data_sbuffer en sensor_data_dplist linken
    sensor_t *sensor_data_dplist;
    for(int i=0; i<dpl_size(list); i++){
        sensor_data_dplist = dpl_get_element_at_index(list, i);
        if(sensor_data_dplist->sensor_id==sensor_data_sbuffer.id){
            break;
        }
    }

    // put timestamp van sensor_data_sbuffer in sensor_data_dplist->timestamp van dplist
    sensor_data_dplist->timestamp=sensor_data_sbuffer.ts;

    // put value van sensor_data_sbuffer in sensor_data_dplist->values[RUN_AVG_LENGTH] van dplist and fix index to get right position in values[RUN_AVG_LENGTH]
    sensor_data_dplist->values[sensor_data_dplist->index]=sensor_data_sbuffer.value;
    if(sensor_data_dplist->index >= RUN_AVG_LENGTH-1 || sensor_data_dplist->index < 0){
        sensor_data_dplist->index=0;
    }
    else sensor_data_dplist->index++;

    // put average of values[] in sensor_data_dplist->running_avg van dplist
    double sum=0;
    for(int i=0; i < RUN_AVG_LENGTH; i++){
        if(sensor_data_dplist->values[i]==0){
            sensor_data_dplist->running_avg=sum/(i);
            break;
        }
        sum += sensor_data_dplist->values[i];
        sensor_data_dplist->running_avg=sum/RUN_AVG_LENGTH;
    }

    // log-message running_avg to high or low
    if(sensor_data_dplist->running_avg<SET_MIN_TEMP){
        sprintf(wmsg_datamgr, "Sensor node %d reports it's too cold (avg temp = %f)", sensor_data_dplist->sensor_id, sensor_data_dplist->running_avg);
        write(fd[WRITE_END], wmsg_datamgr, SIZE);
    }
    else if(sensor_data_dplist->running_avg>SET_MAX_TEMP){
        sprintf(wmsg_datamgr, "Sensor node %d reports it's too hot (avg temp = %f)", sensor_data_dplist->sensor_id, sensor_data_dplist->running_avg);
        write(fd[WRITE_END], wmsg_datamgr, SIZE);
    }
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> TESTING! >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
/*
    // check datamgr_parse_sensor_data
    printf("size of list = %d\n", dpl_size(list));
    for(int x=0; x<dpl_size(list); x++){
        sensor_t *element = dpl_get_element_at_index(list,x);
        printf("timestamp %d = %ld\n", x, element->timestamp);
        printf("running_avg %d = %f\n", x, element->running_avg);
        for(int y=0; y<RUN_AVG_LENGTH; y++){
            double value = element->values[y];
            printf("values for sensor_id = %d are %f\n", element->sensor_id, value);
        }
    }
*/
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< TESTING! <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
}

void datamgr_free(){
    dpl_free(&list, true);
}

uint16_t datamgr_get_room_id(sensor_id_t sensor_id){
    sensor_t *sensor = dpl_get_element_at_index(list, sensor_id);
    return sensor->room_id;
}

sensor_value_t datamgr_get_avg(sensor_id_t sensor_id){
    sensor_t *sensor = dpl_get_element_at_index(list, sensor_id);
    return sensor->running_avg;
}

time_t datamgr_get_last_modified(sensor_id_t sensor_id){
    sensor_t *sensor = dpl_get_element_at_index(list, sensor_id);
    return sensor->timestamp;
}

int datamgr_get_total_sensors(){
    return dpl_size(list);
}

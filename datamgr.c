#include "datamgr.h"
#include "lib/dplist.h"
#include <ctype.h>
//#include "lib/dplist.c"


typedef struct sensor{
    sensor_id_t sensor_id;
    room_id_t room_id;
    sensor_ts_t timestamp;
    double running_avg;
    double values[RUN_AVG_LENGTH];
    int index;
} sensor_t;

dplist_t *list;

void* element_copy(void * element);
void element_free(void ** element);
int element_compare(void * x, void * y);


void* element_copy(void * element){
    return NULL;
}

void element_free(void ** element){
    //free((((sensor_t*)*element))->name);
    free(*element);
    *element = NULL;

}

int element_compare(void * x, void * y){
    return ((((sensor_t*)x)->sensor_id < ((sensor_t*)y)->sensor_id) ? -1 : (((sensor_t*)x)->sensor_id == ((sensor_t*)y)->sensor_id) ? 0 : 1);
}

void datamgr_parse_sensor_files(FILE *fp_sensor_map, FILE *fp_sensor_data){
    printf("reading files\n");

    list = dpl_create(element_copy, element_free, element_compare); // aanmaken bovenaan gezet om in alle functies te kunnen gebruiken.
    
    // (1) Uitlezen van room_sensor.map en in dplist stoppen
    const unsigned MAX_LENGTH = sizeof(*fp_sensor_map);
    char buffer[MAX_LENGTH];
    char room[10];
    char sensor[10];
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
        element->room_id=atoi(room); // convert array (char room[10]) to integer
        element->sensor_id=atoi(sensor); // convert array (char sensor[10]) to integer
        dpl_insert_at_index(list, element,0,false);
        
        // reset, to start over again
        i=0;
        n=0;
    }
    
    // Check van (1) -> OK
    printf("size of list = %d\n", dpl_size(list));
    for(int x=0; x<dpl_size(list); x++){
        sensor_t *element = dpl_get_element_at_index(list,x);
        printf("sensor_id %d = %d\n", x, element->sensor_id);
        printf("room_id %d = %d\n", x, element->room_id);
    }


    // (2) Uitlezen van sensor_data, linken en in dplist stoppen
    int size_sensor_data = 800;
    sensor_data_t data[size_sensor_data];
    fread(&data, sizeof(sensor_data_t),size_sensor_data,fp_sensor_data);
    for(int k=0; k < 800; k++){
        //sensor_t *element = malloc(sizeof(char));
        sensor_t *element;
        for(int l=0; l < dpl_size(list); l++){
            element = dpl_get_element_at_index(list, l);
            if(element->sensor_id==data[k].id){
                break;
            }
        }
        
        // Put timestamp in element->timestamp van dplist
        element->timestamp=data[k].ts;

        // Put value in element->values[RUN_AVG_LENGTH] van dplist and fix index to get right position in values[RUN_AVG_LENGTH]
        element->values[element->index]=data[k].value;
        if(element->index >= RUN_AVG_LENGTH-1 || element->index < 0){
            element->index=0;
        }
        else element->index++;

        // Put average of values[] in element->running_avg van dplist
        double sum=0;
        for(int l=0; l < RUN_AVG_LENGTH; l++){
            sum += element->values[l];
        }
        element->running_avg=sum/RUN_AVG_LENGTH;
    }
    

    // Check van (2)
    printf("size of list = %d\n", dpl_size(list));
    for(int x=0; x<dpl_size(list); x++){
        sensor_t *element = dpl_get_element_at_index(list,x);
        printf("timestamp %d = %ld\n", x, element->timestamp);
        printf("running_avg %d = %f\n", x, element->running_avg);
        for(int y=0; y<5; y++){
            double value = element->values[y];
            printf("values for sensor_id = %d are %f\n", element->sensor_id, value);
        }
    }

//###############################DRIWO#########################################
/*
    fread(&data,sizeof(sensor_data_t),size_sensor_data,fp_sensor_data);
    for(int k=0; k<800; k++){
        sensor_id_t sid = data[k].id;
        for(int j=0; j<dpl_size(list); j++){
            sensor_t *element = (sensor_t*)dpl_get_element_at_index(list,j);
            if(element->sensor_id==sid){
                element->timestamp=data[k].ts;
                element->values[element->index]=data[k].value;
                if(element->index >= RUN_AVG_LENGTH || element->index < 0){
                    element->index=0;
                }
                else element->index++;
                element->running_avg=get_running_average(element->values, RUN_AVG_LENGTH);
                if(element->running_avg > SET_MAX_TEMP){
                    printf("TOOOOO HOT in room %d at time %ld\n", element->room_id, element->ts);
                }
                dpl_remove_at_index(list, j, false);
                list=dpl_insert_at_index(list, element, 0, false);
            }
        }
    }
*/
//###############################DRIWO#########################################
    
    //********** yuri **********
    /*
    sensor_data_t sdt;
    sensor_value_t svt;
    sensor_ts_t stt;
    sensor_id_t sit;

    printf("sizeof(sensor_id_t) = %ld \n", sizeof(sit));
    printf("sizeof(sensor_value_t) = %ld \n", sizeof(svt));
    printf("sizeof(sensor_ts_t) = %ld \n", sizeof(stt));
    printf("sizeof(sensor_data_t) = %ld \n", sizeof(sdt));
    */
    //********** yuri **********
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
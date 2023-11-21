#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "lib/dplist.h"
#include "datamgr.h"
#include <time.h>

int main(){
    printf("Hello World\n");

    FILE * map = fopen("room_sensor.map", "r");
    FILE * data = fopen("sensor_data", "rb");

    if(map == NULL) return -1;
    if(data == NULL) return -1;

    datamgr_parse_sensor_files(map, data);

    printf("room_id = %d\n", datamgr_get_room_id(15));

    printf("Running average = %f\n", datamgr_get_avg(15));

    printf("Last modified: %ld\n", datamgr_get_last_modified(15));

    printf("Amount of sensors = %d\n", datamgr_get_total_sensors());

    datamgr_free();

    fclose(map);
    fclose(data);
}
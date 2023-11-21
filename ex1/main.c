#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "sensor_db.h"
#include <time.h>

int main(){
    printf("Hello World!\n");

    //FILE * data = fopen("test.csv", "w");
    // Opening modes for fopen("<filename>", "X");
    // --> X kan verschillende dingen zijn, zoals te zien op onderstaande website
    //https://www.programiz.com/c-programming/c-file-input-output

    //if(data == NULL) return -1;


    FILE *file = open_db("test.csv", true);
    insert_sensor(file, 0, 0.0 , 0);
    insert_sensor(file, 0, 0.0 , 0);
    close_db(file);
}

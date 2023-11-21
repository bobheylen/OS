/**
 * \author Bob Heylen
 */

#ifndef __SENSOR_DB_H__
#define __SENSOR_DB_H__

#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include <stdbool.h>

/**
 * This method opens a text file with a given name, and providing an indication:
 *  - if the file should be overwritten when the file already exists.
 *  - if the data should be appended when the file already exists.
*/
FILE * open_db(char * filename, bool append);

/**
 * This method appends a single sensor reading to the csv file.
*/
int insert_sensor(FILE * f, sensor_id_t id, sensor_value_t value, sensor_ts_t ts);

/**
 * This method closes the csv file.
*/
int close_db(FILE * f);

#endif //__SENSOR_DB_H__
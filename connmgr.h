/**
 * \author Bob Heylen
 */

#ifndef __CONMGR_H__
#define __CONMGR_H__

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <pthread.h>

#include "config.h"
#include "sbuffer.h"
#include "lib/tcpsock.h"

/**
 * Thread function thats listening on a given port.
 * When a sensor_node connect it writes data to the sbuffer.
*/
void *runner(void *arg);

/**
 * Starts up a TCP server.
 * Starts threads where sensor_nodes can connect.
*/
int test_server(int port_nr, sbuffer_t *sbuffer);

#endif  //__CONMGR_H__
/**
 * \author Bob Heylen
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include "config.h"
#include "lib/tcpsock.h"
#include "connmgr.h"

// state the max. number of connections the server will handle before exiting
#ifndef MAX_CONN
#define MAX_CONN 3
#endif

char wmsg_connmgr[SIZE];
extern bool available;          /**< indicator: still data available in sbuffer? */
tcpsock_t *server;
int bytes, result;

void *runner(void *arg)
{
    tcpsock_t *client;
    sensor_data_t data;
    sbuffer_t *sbuffer = arg;
    int wmsg_connmgr_once = 0;

    // Clients check if connenction with test server is possible 
        // if true printf("Incoming client connection") & conn_counter++
        // if false exit(EXIT_FAILURE)
    if (tcp_wait_for_connection(server, &client) != TCP_NO_ERROR) exit(EXIT_FAILURE);
    printf("Incoming client connection\n");
    // Starts reading sensor as long as result is OK
    do {
        // read sensor ID
        bytes = sizeof(data.id);
        result = tcp_receive(client, (void *) &data.id, &bytes);
        // read temperature
        bytes = sizeof(data.value);
        result = tcp_receive(client, (void *) &data.value, &bytes);
        // read timestamp
        bytes = sizeof(data.ts);
        result = tcp_receive(client, (void *) &data.ts, &bytes);
        if ((result == TCP_NO_ERROR) && bytes) {
            printf("sensor id = %" PRIu16 " - temperature = %g - timestamp = %ld\n", data.id, data.value,
                    (long int) data.ts);
            sbuffer_insert(sbuffer,&data);
            available = true;
            if(wmsg_connmgr_once==0){
                sprintf(wmsg_connmgr, "Sensor node %d has openend a new connection", data.id);
                write(fd[WRITE_END], wmsg_connmgr, SIZE);
                wmsg_connmgr_once++;
            }
        }
    } while (result == TCP_NO_ERROR);
    // >>>>>>>>>>>>>>>>> geeft info, maar niet echt heeft niet echt een functie >>>>>>>>>>>>>>>>>
    if (result == TCP_CONNECTION_CLOSED){
        printf("Peer has closed connection\n");
        sprintf(wmsg_connmgr, "Sensor node %d has closed the connection", data.id);
        write(fd[WRITE_END], wmsg_connmgr, SIZE);
    }
    else
        printf("Error occured on connection to peer\n");
    // <<<<<<<<<<<<<<<<< geeft info, maar niet echt heeft niet echt een functie <<<<<<<<<<<<<<<<<
    tcp_close(&client);
    pthread_exit(0);
}

int test_server(int port_nr, sbuffer_t* sbuffer) {

    // opening test server
    if (tcp_passive_open(&server, port_nr) != TCP_NO_ERROR) exit(EXIT_FAILURE);
    printf("Test server is started\n");

    // make thread variables
    pthread_t tid[MAX_CONN];
    pthread_attr_t attr;
    
    // get the default attributes
    pthread_attr_init(&attr);

    // creating new threads, with attributes specified by attr
    for(int i=0; i<MAX_CONN; i++){
        pthread_create(&tid[i], &attr, runner, sbuffer);
    }

    // waits for threads to join
    for(int i=0; i<MAX_CONN; i++){
        pthread_join(tid[i], NULL);
    }
    
    // insert end of data indicator
    sensor_data_t end_data;
    end_data.id=0;
    sbuffer_insert(sbuffer, &end_data);

    // closing test server
    if (tcp_close(&server) != TCP_NO_ERROR) exit(EXIT_FAILURE);
    printf("Test server is shutting down\n");
    
    return 0;
}

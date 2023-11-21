/**
 * \author Bob Heylen
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <pthread.h>
#include "config.h"
#include "lib/tcpsock.h"

//#define PORT 5678
int port;
#define MAX_CONN 3  // state the max. number of connections the server will handle before exiting

tcpsock_t *server;
sensor_data_t data;
int bytes, result;
int conn_counter = 0;

/**
 * Implements a sequential test server (only one connection at the same time)
 */

void *runner()
{
    tcpsock_t *client;

    // Check if connenction with test server is possible 
        // if true printf("Incoming client connection") & conn_counter++
        // if false exit(EXIT_FAILURE)
    if (tcp_wait_for_connection(server, &client) != TCP_NO_ERROR) exit(EXIT_FAILURE);
    printf("Incoming client connection\n");
    conn_counter++;
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
        }
    } while (result == TCP_NO_ERROR);
    // >>>>>>>>>>>>>>>>> geeft info, maar niet echt heeft niet echt een functie >>>>>>>>>>>>>>>>>
    if (result == TCP_CONNECTION_CLOSED)
        printf("Peer has closed connection\n");
    else
        printf("Error occured on connection to peer\n");
    // <<<<<<<<<<<<<<<<< geeft info, maar niet echt heeft niet echt een functie <<<<<<<<<<<<<<<<<
    tcp_close(&client);
    pthread_exit(0);
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Need to pass a portnumber larger than 1023, and smaller than 65000\n");
        exit(EXIT_SUCCESS);
        return -1;
    } else {
        data.id = atoi(argv[1]);
        port = atoi(argv[1]);
        printf("listening on port: %d\n",port);
    }

    // Opening test server
    if (tcp_passive_open(&server, port) != TCP_NO_ERROR) exit(EXIT_FAILURE);
    printf("Test server is started\n");

    //********************************************************************************
    /*
    if (argc != 2) {
        printf("Enter a TCP port higher than 1024: ");
        //scanf("%d", &port);
        exit(EXIT_SUCCESS);
    } else {
        // to do: user input validation!
        data.id = atoi(argv[1]);
        port = atoi(argv[2]);
    */
    //********************************************************************************

//+++++++++++++++++++++++++++++++ met attr +++++++++++++++++++++++++++++++

    pthread_t tid[MAX_CONN];
    pthread_attr_t attr;
    /* get the default attributes */
    pthread_attr_init(&attr);

    // creating new threads, with attributes specified by attr
    /* create the threads */
    for(int i=0; i<MAX_CONN; i++){
        pthread_create(&tid[i], &attr, runner, NULL);               // denk dat dit juist is door naar voorbeelden op slides te kijken
        //pthread_create(&tid[i], &attr, runner, argv[1]);          // werkt ook, maar weet niet wat het verschil is
        //pthread_create(&tid[i], &attr, runner, argv[MAX_CONN]);   // werkt ook, maar weet niet wat het verschil is
    }

    // waits for threads to terminate
    /* now join on each thread */
    for(int i=0; i<MAX_CONN; i++){
        pthread_join(tid[i], NULL);
    }

//+++++++++++++++++++++++++++++++ met attr +++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++ zonder attr ++++++++++++++++++++++++++++++
/*
    pthread_t tid[MAX_CONN];
    for(int i=0; i<MAX_CONN; i++){
        pthread_create(&tid[i], NULL, runner, NULL);
    }

    for(int i=0; i<MAX_CONN; i++){
        pthread_join(tid[i], NULL);
    }
*/
//+++++++++++++++++++++++++++++ zonder attr ++++++++++++++++++++++++++++++

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> staat hierboven >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    /*printf("Test server is started\n");
    if (tcp_passive_open(&server, port) != TCP_NO_ERROR) exit(EXIT_FAILURE);*/
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< staat hierboven <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// ->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->
//************************************** staat in runner **************************************
// ->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->
    /*do {
        // WAARSCHIJNLIJK VANAF HIER MOETE BEGINNEN MULTITHREADEN
        if (tcp_wait_for_connection(server, &client) != TCP_NO_ERROR) exit(EXIT_FAILURE);
        printf("Incoming client connection\n");
        conn_counter++;
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
            }
        } while (result == TCP_NO_ERROR);
        if (result == TCP_CONNECTION_CLOSED)
            printf("Peer has closed connection\n");
        else
            printf("Error occured on connection to peer\n");
        tcp_close(&client);
    } while (conn_counter < MAX_CONN);*/
// <-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-
//************************************** staat in runner **************************************
// <-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-

    if (tcp_close(&server) != TCP_NO_ERROR) exit(EXIT_FAILURE);
    printf("Test server is shutting down\n");
    return 0;
}

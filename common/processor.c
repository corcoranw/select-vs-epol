/*
 * NOTES:
 *
 * This file listens on a specific port for client connections.
 * Clients will connect to a thread running this queue and send
 * their log data to it.
 *
 * read udp packets and add to queue
 * defined logger function will act on separate thread and read/pop from queue and write to
 * a log file
 *
 * This queue will be in a shared memory space with another thread that writes from
 * the queue into a log file.
 */

#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <omp.h>

#include "processor.h"
#include "queue.h"

#define SERVER_UDP_PORT 	65535	// Default port
#define MAX_READ_LEN		100
#define TRUE			1

int main(void) {
    startLoggingServer();
    return 0;
}

void startLoggingServer() {
    int	    sd, n;
    socklen_t client_len;
	char	buf[MAX_STRING_LEN];
	struct	sockaddr_in server, client;

	// Create a datagram socket
	if ((sd = socket (AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror ("Can't create a socket"); 
		exit(1);
	}

	// Bind an address to the socket
	bzero((char *)&server, sizeof(server)); 
	server.sin_family = AF_INET; 
	server.sin_port = htons(SERVER_UDP_PORT); 
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind (sd, (struct sockaddr *)&server, sizeof(server)) == -1) {
		perror ("Can't bind name to socket");
		exit(1);
	}
    // Queue variables
    int head, tail;
    char queue[MAX_QUEUE_SIZE][MAX_STRING_LEN];
    init(&head, &tail);

    #pragma omp parallel shared(head, tail, queue) num_threads(2) //private(element)
    {
        #pragma omp sections // divides the team into sections
        { 
            #pragma omp section
            { 
                while(TRUE) {
                    client_len = sizeof(client);
                    if ((n = recvfrom (sd, buf, MAX_READ_LEN, 0, (struct sockaddr *)&client, &client_len)) < 0)
                    {
                        perror ("recvfrom error");
                        exit(1);
                    }
                    if(full(&tail, &head)){
                        printf("Queue is full\n");
                        continue;
                    }
                    enqueue(queue, &tail, &head, buf);
                    printf("enqueuing %s\n", buf);
                }
                printf("Exiting enqueuing loop.");
            }
            #pragma omp section
            { 
                // Log file
                FILE *fptr;
                while(TRUE) {
                    if((fptr = fopen("./log.txt", "a")) == NULL) {
                        perror("Error opening log file.\n");
                        exit(0);
                    }
                    char element[MAX_STRING_LEN];
                    if(empty(&tail, &head)) {
                        fclose(fptr);
                        continue;
                    }
                    if(!dequeue(queue, &tail, &head, element)) {
                        printf("Failed to dequeue element.");
                    } else {
                        fprintf(fptr, "%s", element);
                        printf("dequeueing %s", element);
                        // realloc(element, MAX_STRING_LEN);
                        fclose(fptr);
                    }
                }
            }
        }
    }
}

void *runLogging(void *args) {
    return 0;
}

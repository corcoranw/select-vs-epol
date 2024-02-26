#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include "clnt.h"

#define SERVER_TCP_PORT		7000	// Default port
#define BUFLEN			80  	// Buffer length
#define DEFAULT_ROUNDS          1

long delay (struct timeval t1, struct timeval t2);

int client (char *host, int port, char *msg, int rounds, int udpport, int id, FILE *fp)
{
    struct controller {
        int count;
        char bye[BUFLEN];
        int myPort;
        struct sockaddr_in my_addr, client;
        struct hostent *hp;
    } local;
    struct connection {
        int sd;
        struct sockaddr_in address;
    } tcpserver, udpprocessor;
    struct logging {
        int n, bytes_to_read;
        char *bp, rbuf[BUFLEN], str[BUFLEN], log[BUFLEN];
        struct timeval start, end;
    } data;
    int *l, *t, *u, *d;

    //allocate memory for local and tcpserver,udpprocessor
    l = malloc(sizeof(local));
    t = malloc(sizeof(tcpserver));
    u = malloc(sizeof(udpprocessor));
	// Create the socket
	if ((tcpserver.sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Cannot create socket");
		return(1);
	}
	bzero((char *)&tcpserver.address, sizeof(struct sockaddr_in));
	tcpserver.address.sin_family = AF_INET;
	tcpserver.address.sin_port = htons(port);
	inet_pton(AF_INET, host, &tcpserver.address.sin_addr);
	
	//Bind local address to the socket
	bzero((char *)&local.client, sizeof(local.client));
        local.client.sin_family = AF_INET;
	local.client.sin_addr.s_addr = htonl(INADDR_ANY);
	local.client.sin_port = htons(0);
	if (bind(tcpserver.sd, (struct sockaddr *)&local.client, sizeof(local.client)) == -1)
	{
		perror ("Can't bind name to socket");
		return(1);
	}
	// Connecting to the server
	if (connect (tcpserver.sd, (struct sockaddr *)&tcpserver.address, sizeof(tcpserver.address)) == -1)
	{
		fprintf(stderr, "Can't connect to server\n");
		perror("connect");
		return(1);
	}

	local.count = 0;
	while (local.count < rounds)
	{
	    d = malloc(sizeof(data));
		// Transmit data through the socket
		send (tcpserver.sd, msg, BUFLEN, 0);
		// Set timestamp1
		gettimeofday(&data.start, NULL);

		//printf("Receive:\n");
		data.bp = data.rbuf;
		data.bytes_to_read = BUFLEN;

		// client makes repeated calls to recv until no more data is expected to arrive.
		data.n = 0;
		while ((data.n = recv (tcpserver.sd, data.bp, data.bytes_to_read, 0)) < BUFLEN)
		{
			data.bp += data.n;
            data.bytes_to_read -= data.n;
		}
		gettimeofday(&data.end, NULL);
		fprintf(fp, "client id: %u  \"%s\", %lu useconds RTT\n", id, msg, (data.end.tv_sec - data.start.tv_sec) * 1000000 + data.end.tv_usec - data.start.tv_usec);
		++local.count;
		free(d);
	}
	strncpy(local.bye, "exit", 5);
	send(tcpserver.sd, local.bye, BUFLEN, 0);

	fflush(stdout);
	close (tcpserver.sd);
	free(l);
	free(t);
	free(u);
	return (0);
}

// Calculate delay in milliseconds
long delay (struct timeval t1, struct timeval t2)
{
	long d;

	d = (t2.tv_sec - t1.tv_sec) * 1000;
	d += ((t2.tv_usec - t1.tv_usec + 500) / 1000);
	return(d);
}

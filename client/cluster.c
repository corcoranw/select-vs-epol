#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <time.h>

#include "../common/processor.h"
#include "clnt.h"

#define BUFFLEN             80

int main (int argc, char *argv[])
{
	FILE *fptr;
	fptr = fopen("./output.txt","w");

	if(fptr == NULL)
	{
		printf("Error!");   
		exit(1);             
	}

	//long i;
	int rounds, clients, port, udpport, id;
	char msg[BUFFLEN], *serverhost;
	if (argc != 6)
	{
		fprintf(stderr, "Usage: %s serverhost port message rounds clients\n", argv[0]);
		exit(1);
	}
	udpport = PROCESSOR_PORT;

	serverhost = argv[1];
	port = atoi(argv[2]);
	strncpy(msg, argv[3], BUFFLEN);
	rounds = atoi(argv[4]);
	clients = atoi(argv[5]);
	srand(time(NULL));
	id = 1;
	#pragma omp parallel num_threads(clients) default(none) shared(serverhost, port, msg, rounds, udpport, id, fptr)
	{
		id = rand();
		client(serverhost, port, msg, rounds, udpport, id, fptr);
	}
}

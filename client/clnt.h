#ifndef CLNT_H
#define CLNT_H

int client(char *host, int port, char *msg, int rounds, int udpport, int id, FILE *fp);

long delay(struct timeval t1, struct timeval t2);

int sockfd_to_local_port(int sd);


#endif

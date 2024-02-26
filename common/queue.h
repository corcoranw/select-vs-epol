/*
 * File   : queue.h
 * Author : zentut.com
 * Purpose: stack header file
 */
#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#define MAX_QUEUE_SIZE 10000
#define MAX_STRING_LEN 80
 
void init(int *head, int *tail);
int enqueue(char q[MAX_QUEUE_SIZE][MAX_STRING_LEN],int *tail, int *head, char element[]);
int dequeue(char q[MAX_QUEUE_SIZE][MAX_STRING_LEN], int *tail, int *head, char *buf);
int empty(int *tail, int *head);
int full(int *tail, int *head);

#endif // QUEUE_H_INCLUDED

/*
 * File   : queue.c
 * Author : zentut.com
 * Purpose: stack header file
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
/*
    initialize queue pointers
*/
void init(int *head, int *tail)
{
    *head = *tail = 0;
}

/*
enqueue an element
precondition: queue is not full, element is character array of fixed size (80 characters)
*/
int enqueue(char q[MAX_QUEUE_SIZE][MAX_STRING_LEN], int *tail, int *head, char *element)
{
    if((*tail == (MAX_QUEUE_SIZE - 1) && *head == 0) || (*head == *tail + 1)) {
        printf("Queue is full, cannot insert element.");
        return 0;
    } else {
        // q[(*tail)++] = element;
        strcpy(q[(*tail)++], element);
        if(*tail == MAX_QUEUE_SIZE && *head != 0) {
            *tail = 0;
        }
    }
    return 1;
}

/*
    dequeue an element
    precondition: queue is not empty
*/
int dequeue(char q[MAX_QUEUE_SIZE][MAX_STRING_LEN], int *tail, int *head, char *buf)
{
    if((*tail == 0 && *head == 0) ||  (*head == *tail)) {
        printf("Queue is empty, unable to remove");
        return 0;
    } else {
        strcpy(buf, q[(*head)++]);
        if(*head == MAX_QUEUE_SIZE) {
            *head = 0;
        }
        return 1;
    }
}

/*
    return 1 if queue is full, otherwise return 0
*/
int full(int *tail, int *head)
{
    return ((*tail == (MAX_QUEUE_SIZE - 1) && *head == 0) || (*head == *tail + 1));
}

/*
return 1 if the queue is empty, otherwise return 0
*/
int empty(int *tail, int *head)
{
    return ((*tail == 0 && *head == 0) ||  (*head == *tail));
}

/*
display queue content
*/
// void display(int *q,int head,int tail)
// {
//     int i = tail - 1;
//     while(i >= head)
//         printf("%d ",q[i--]);
//     printf("\n");
// }
#pragma once


#ifndef QUEUE_H
#define QUEUE_H

/* Defines -------------------------------------------------------------------*/
#define MAX_QUEUE_SIZE      256

/* Type declarations ---------------------------------------------------------*/
typedef struct
{
    int front, rear;
    unsigned char queue[MAX_QUEUE_SIZE];
} QueueType;

typedef enum {QUEUE_SUCCESS = 0, QUEUE_ERROR = !QUEUE_SUCCESS} QueueStatus;

/* Function prototypes -------------------------------------------------------*/
void init_queue(QueueType *q);
int is_empty(QueueType *q);
int is_full(QueueType *q);
QueueStatus enqueue(QueueType *q, unsigned char item);
QueueStatus dequeue(QueueType *q, unsigned char *item);

#endif


#ifndef DROGI_PRIORITY_QUEUE_H
#define DROGI_PRIORITY_QUEUE_H

#include <stdbool.h>

typedef struct QueueElement
{
    int dist, year, parent;
} QueueElement;

typedef struct PriorityQueue // TODO przenieść do *.c
{
    QueueElement **content;
    int size;
    int filled;
} PriorityQueue;

PriorityQueue *newPriorityQueue();

void deletePriorityQueue(PriorityQueue *queue);

bool priorityQueuePush(PriorityQueue *queue, QueueElement *elem);

QueueElement *priorityQueuePop(PriorityQueue *queue);

bool isEmpty(PriorityQueue *queue);

#endif //DROGI_PRIORITY_QUEUE_H

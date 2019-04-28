#ifndef DROGI_PRIORITY_QUEUE_H
#define DROGI_PRIORITY_QUEUE_H

#include <stdbool.h>
#include "graph.h"

typedef struct QueueElement
{
    int dist, year;
    OrientedEdge *parent;
    int nodeId;
} QueueElement;

typedef struct PriorityQueue // TODO przenieść do *.c
{
    QueueElement **content;
    int size;
    int filled;
} PriorityQueue;

PriorityQueue *newPriorityQueue();

QueueElement *newQueueElement(int dist, int year, OrientedEdge *parent);

void deletePriorityQueue(PriorityQueue *queue);

bool priorityQueuePush(PriorityQueue *queue, QueueElement *elem);

QueueElement *priorityQueuePop(PriorityQueue *queue);

bool isEmpty(PriorityQueue *queue);

#endif //DROGI_PRIORITY_QUEUE_H

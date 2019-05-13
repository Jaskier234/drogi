#ifndef DROGI_PRIORITY_QUEUE_H
#define DROGI_PRIORITY_QUEUE_H

#include <stdbool.h>

typedef struct PriorityQueue
{
    void **content;
    int size;
    int filled;
    int(*compare)(void*, void*);
} PriorityQueue;

PriorityQueue *newPriorityQueue(int(*cmp)(void *, void *));

void deletePriorityQueue(PriorityQueue *queue);

bool priorityQueuePush(PriorityQueue *queue, void *elem);

void * priorityQueuePop(PriorityQueue *queue);

bool isEmpty(PriorityQueue *queue);

#endif //DROGI_PRIORITY_QUEUE_H

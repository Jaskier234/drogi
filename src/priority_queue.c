#include "priority_queue.h"

#include <stdlib.h>

PriorityQueue *newPriorityQueue(int(*cmp)(void *, void *)) // todo dodać funkcję compare
{
    unsigned int INITIAL_SIZE = 10;

    PriorityQueue *queue = calloc(1, sizeof(PriorityQueue)); // TODO obsługa nieudanych calloców
    queue->content = calloc(INITIAL_SIZE, sizeof(void*));
    queue->size = INITIAL_SIZE;
    queue->filled = 0;
    queue->compare = cmp;

    return queue;
}

void deletePriorityQueue(PriorityQueue *queue)
{
    free(queue->content);
    free(queue);
}

// dodaje element do kolejki priorytetowej. zwraca false, gdy to się nie uda
bool priorityQueuePush(PriorityQueue *queue, void *elem)
{
    if(queue->filled+1 >= queue->size)
    {
        queue->size *= 2;
        queue->content = realloc(queue->content, queue->size*sizeof(void*));

        if(queue->content == NULL)
            return false;
    }

    queue->content[queue->filled+1] = elem;

    int index = queue->filled+1;

    while( index/2 > 0 && queue->compare(queue->content[index], queue->content[index/2]) > 0 )
    {
        void *temp = queue->content[index];
        queue->content[index] = queue->content[index/2];
        queue->content[index/2] = temp;

        index /= 2;
    }

    queue->filled++;

    return true;
}

void * priorityQueuePop(PriorityQueue *queue)
{
    if(isEmpty(queue))
        return NULL;

    void *res = queue->content[1];

    void *temp = queue->content[1];
    queue->content[1] = queue->content[queue->filled];
    queue->content[queue->filled] = temp;

    int index = 1;

    while( true )
    {
        if(index*2 >= queue->filled) // brak synów
            break;

        int next;
        if(index*2+1 >= queue->filled) // jeden syn;
            next = index*2;
        else
        {
            if(queue->compare(queue->content[index*2], queue->content[index*2+1]) > 0)
                next = index*2;
            else
                next = index*2+1;
        }

        if(queue->compare(queue->content[next], queue->content[index]) > 0)
        {
            temp = queue->content[next];
            queue->content[next] = queue->content[index];
            queue->content[index] = temp;

            index = next;
        }
        else
            break;
    }

    queue->filled--;

    return res;
}

bool isEmpty(PriorityQueue *queue)
{
    return queue->filled == 0;
}
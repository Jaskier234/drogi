#include "priority_queue.h"

#include <stdlib.h>

PriorityQueue *newPriorityQueue()
{
    unsigned int INITIAL_SIZE = 10;

    PriorityQueue *queue = calloc(1, sizeof(PriorityQueue));
    queue->content = calloc(INITIAL_SIZE, sizeof(QueueElement*));// TODO (*)wskaźniki czy cała struktura?
    queue->size = INITIAL_SIZE;
    queue->filled = 0;

    return queue;
}

void deletePriorityQueue(PriorityQueue *queue)
{
    free(queue->content);
    free(queue);
}

// true jeżeli a < b tzn. a powinno być niżej w kopcu od b
// compare(a,a) == false
bool compare(QueueElement *a, QueueElement *b)
{
    if(a->dist == b->dist)
        return a->year > b->year;
    return a->dist < b->dist;
} // TODO czy to działa?

// dodaje element do kolejki priorytetowej. zwraca false, gdy to się nie uda
bool priorityQueuePush(PriorityQueue *queue, QueueElement *elem)
{
    if(queue->filled >= queue->size)
    {
        queue->content = realloc(queue->content, queue->size*2);
        queue->size *= 2;

        if(queue->content == NULL)
            return false;

//        for(int i=queue->filled ; i<queue->size; i++)
//            queue->content[i] = NULL;
    }

    queue->content[queue->filled] = elem;

    int index = queue->filled;

    while( compare(queue->content[index], queue->content[index/2]) ) // compare(a,a) == false, więc pętla się zatrzyma, gdy index dojdzie do 0
    {
        QueueElement *temp = queue->content[index];
        queue->content[index] = queue->content[index/2];
        queue->content[index/2] = temp;

        index /= 2;
    }

    queue->filled++; // TODO ew. połączyć takie inkrementacje w 1 linijkę

    return true;
}

QueueElement *min(QueueElement *a, QueueElement *b)
{
    if( compare(a,b) )
        return a;
    return b;
}

QueueElement *PriorityQueuePop(PriorityQueue *queue)
{
    QueueElement *res = queue->content[0];

    QueueElement *temp = queue->content[0];
    queue->content[0] = queue->content[queue->filled-1];
    queue->content[queue->filled-1] = temp;

    int index = 0;

    while( true )
    {
        if(index*2 >= queue->filled) // brak synów
            break;

        int next;
        if(index*2+1 >= queue->filled) // jeden syn;
            next = index*2;
        else
        {
            if(compare(queue->content[index*2], queue->content[index*2+1]))
                next = index*2;
            else
                next = index*2+1;
        }

        if(compare(queue->content[next], queue->content[index])) // swap
        {
            QueueElement *temp = queue->content[next];
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
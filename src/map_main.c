#include "map.h"
#include "list.h"
#include "hashtable.h"
#include "graph.h"
#include "priority_queue.h"
#include "string_ext.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#undef NDEBUG

#include <assert.h>

void mapBasic();
void listBasic();
void listForeach();
void hashtableBasic();
void hashtableTime();
void graphBasic();
//void queueBasic();
void dijkstraBasic();
void routeBasic();
void multipleRoutes();

int main() {

    Vector *command;
    while(true)
    {
        command = nextCommand();
        if(command == NULL)
            break;

        for(int i=0; i<command->filled; i++)
        {
            printf("%s ", (char*)command->tab[i]);
        }
        printf("\n");
//        fprintf(stderr, "ERROR\n");
    }

    // testy
//    listBasic();
//    listForeach();
//    hashtableBasic();
//    hashtableTime();
//    graphBasic();
//    mapBasic();
//    queueBasic();
//    dijkstraBasic();
//    routeBasic();
//    multipleRoutes();
    return 0;
}

void mapBasic()
{
    char const* str;

    Map* m = newMap();
    assert(m);

    assert(addRoad(m, "Alinów", "Bór", 1, 2020));
    assert(addRoad(m, "Bór", "Cielińsk-Niekłańsk", 2, 2020));
    assert(addRoad(m, "Bór", "Dąb Stary", 3, 2020));
    assert(addRoad(m, "Cielińsk-Niekłańsk", "Emiliew", 4, 2020));
    assert(addRoad(m, "Dąb Stary", "Emiliew", 5, 2020));
    assert(addRoad(m, "Emiliew", "Bór", 8, 2020));
    assert(addRoad(m, "Emiliew", "Fraźnik Nowy", 3, 2020));
    assert(!repairRoad(m, "Emiliew", "Cielińsk-Niekłańsk", 2019));
    assert(repairRoad(m, "Emiliew", "Cielińsk-Niekłańsk", 2021));
    assert(!repairRoad(m, "Emiliew", "Alinów", 2020));
    assert(addRoad(m, "Fraźnik Nowy", "Grzegrzewo", 4, 2020));
    assert(addRoad(m, "Alinów", "Grzegrzewo", 10, 2020));
    assert(addRoad(m, "Homilcin", "Grzegrzewo", 5, 2020));
    assert(addRoad(m, "Fraźnik Nowy", "Cielińsk-Niekłańsk", 2, 2020));
    assert(!addRoad(m, "Fraźnik Nowy", "Cielińsk-Niekłańsk", 2, 2020));
    assert(!addRoad(m, "Cielińsk-Niekłańsk", "Fraźnik Nowy", 2, 2020));
    assert(!repairRoad(m, "Emiliew", "Bór", 2018));
    assert(repairRoad(m, "Emiliew", "Cielińsk-Niekłańsk", 2021));
    assert(repairRoad(m, "Emiliew", "Fraźnik Nowy", 2023));
    assert(addRoad(m, "Homilcin", "Cielińsk-Niekłańsk", 3, 2020));
    assert(newRoute(m, 10, "Alinów", "Emiliew"));

    str = getRouteDescription(m, 10);
    assert(strcmp(str, "10;Alinów;1;2020;Bór;2;2020;Cielińsk-Niekłańsk;4;2021;Emiliew") == 0);
    free((void *)str);

    assert(extendRoute(m, 10, "Homilcin"));

    str = getRouteDescription(m, 10);
    assert(strcmp(str, "10;Alinów;1;2020;Bór;2;2020;Cielińsk-Niekłańsk;4;2021;Emiliew"
                       ";3;2023;Fraźnik Nowy;4;2020;Grzegrzewo;5;2020;Homilcin") == 0);
    free((void *)str);

    deleteMap(m);
    printf("map OK\n");
}

void listBasic()
{
    Memory *mem = newMemory();
    List *list = newList(mem);
    listInsert(list->end->prev, "asdf", mem);
    listInsert(list->end->prev, "qwerty", mem);
    listInsert(list->end->prev, "hkdsk", mem);

    assert(listSize(list) == 3);

    Element *elem = list->begin->next;

    assert(strcmp(elem->value, "asdf") == 0);
    elem = elem->next;
    assert(strcmp(elem->value, "qwerty") == 0);
    elem = elem->next;
    assert(strcmp(elem->value, "hkdsk") == 0);
    deleteMemory(mem);

    list = newList(NULL);

    listInsert(list->end->prev, "asdf", NULL);
    listInsert(list->end->prev, "qwerty", NULL);
    listInsert(list->end->prev, "hkdsk", NULL);

    assert(listSize(list) == 3);
    listRemove(list->begin->next);
    assert(listSize(list) == 2);
    listRemove(list->begin->next);
    assert(listSize(list) == 1);
    listRemove(list->begin->next);
    assert(listSize(list) == 0);

    deleteList(list,0);

    printf("lista OK\n");
}

void listForeach()
{
    List *list = newList(NULL);

    for(int i=0; i<10; i++)
    {
        int *num = malloc(sizeof(int));
        *num = i;
        listPushBack(list, num, NULL);
    }

    foreach(it, list)
    {
        int *num = it->value;
        printf("%d ", *num);
    }

    deleteList(list, true);

    printf("\n");
    printf("list foreach OK\n");
}

void hashtableBasic()
{
    Hashtable **hash = newHashtable(16, NULL);

    assert(hashtableInsert(hash, "klucz1", "asdf"));
    assert(hashtableInsert(hash, "klucz2", "hfkdghsjkdfg"));
    assert(hashtableInsert(hash, "klucz2", "qwerty"));
    assert(hashtableInsert(hash, "k", "krótki klucz"));

    assert(strcmp(hashtableGet(hash, "klucz1"), "asdf") == 0);
    assert(strcmp(hashtableGet(hash, "klucz2"), "qwerty") == 0);
    assert(strcmp(hashtableGet(hash, "k"), "krótki klucz") == 0);

    assert(hashtableGet(hash, "klucz_którego_nie_ma_w_tablicy") == NULL);
    printf("Hashtable OK\n");

    deleteHashtable(hash);
}

char *randString(int size)
{
    char * string = calloc(size+1, sizeof(char));

    for(int i=0; i<size; i++)
        string[i] = rand()%26 + 'A';

    return string;
}

void hashtableTime()
{
    Hashtable **hash = newHashtable(16, NULL);
    const int N=100000;
    srand(42);
    for(int i=0; i<N; i++)
    {
        hashtableInsert(hash, randString(100), NULL);
    }
    printf("Hashtable time OK\n");
    printf("    maksymalna długość listy: %d\n", maxListLength(hash));

    deleteHashtable(hash);
}

void graphBasic()
{
    Graph *graph = newGraph();

//    char *A = calloc(

    addNode(graph);
    addNode(graph);
    addEdge(graph, 0, 1, 2, 2019);
    addNode(graph);
    addEdge(graph, 0, 2, 4, 2019);

    addNode(graph);
    addNode(graph);
    addEdge(graph, 3, 4, 3, 2019);

    addEdge(graph, 3, 2, 7, 2019);
    addEdge(graph, 4, 2, 8, 2019);

    assert(getEdge(graph, 0, 1)->length == 2);
    assert(getEdge(graph, 0, 2)->length == 4);
    assert(getEdge(graph, 3, 4)->length == 3);
    assert(getEdge(graph, 2, 3)->length == 7);
    assert(getEdge(graph, 2, 4)->length == 8);

    removeEdge(graph, 2, 4);

    assert(getEdge(graph, 2, 4) == NULL);
    assert(getEdge(graph, 4, 2) == NULL);
    assert(getEdge(graph, 2, 0) != NULL);
    assert(getEdge(graph, 2, 0)->length == 4);
    assert(getEdge(graph, 3, 4) != NULL);
    assert(getEdge(graph, 3, 4)->length == 3);

    printGraph(graph);

    deleteGraph(graph);

    printf("Graph OK\n");
}

//void queueBasic()
//{
//    PriorityQueue *queue = newPriorityQueue();
//
//    priorityQueuePush(queue, newQueueElement(1, 2, 3, 0));
//    priorityQueuePush(queue, newQueueElement(10, 2, 3, 0));
//    priorityQueuePush(queue, newQueueElement(2, 2, 3, 0));
//    priorityQueuePush(queue, newQueueElement(13, 2, 3, 0));
//
//    assert(priorityQueuePop(queue)->dist == 1);
//    assert(priorityQueuePop(queue)->dist == 2);
//    assert(priorityQueuePop(queue)->dist == 10);
//    assert(priorityQueuePop(queue)->dist == 13);
//
//    assert(isEmpty(queue));
//
//    priorityQueuePush(queue, newQueueElement(2, 2019, 3, 0));
//    priorityQueuePush(queue, newQueueElement(2, 2010, 3, 0));
//    priorityQueuePush(queue, newQueueElement(2, 2013, 3, 0));
//    priorityQueuePush(queue, newQueueElement(2, 2000, 3, 0));
//
//    assert(!isEmpty(queue));
//
//    assert(priorityQueuePop(queue)->year == 2019);
//    assert(priorityQueuePop(queue)->year == 2013);
//    assert(priorityQueuePop(queue)->year == 2010);
//    assert(priorityQueuePop(queue)->year == 2000);
//
//    assert(isEmpty(queue));
//
//    deletePriorityQueue(queue);
//    printf("queue OK\n");
//}

void dijkstraBasic()
{
    Graph *graph = newGraph();

    addNode(graph); // 0
    addNode(graph); // 1
    addEdge(graph, 0, 1, 3, 2010);

    addNode(graph); // 2
    addEdge(graph, 1, 2,7, 2015);

    addNode(graph); // 3
    addEdge(graph, 0, 3, 10, 2000);
    addEdge(graph, 3, 1, 5, 2007);
    addEdge(graph, 2, 3, 2, 2008);

    addNode(graph); // 4
    addEdge(graph, 4, 3, 1, 2008);

    addNode(graph); // 5
    addEdge(graph, 5,3,5,2018);
    addEdge(graph, 5,4,5,2007);

    addNode(graph); // 6
    addEdge(graph, 6, 0, 1, 2011);
    addEdge(graph, 6, 4, 7, 2008);


    addNode(graph); // 7
    assert(bestPath(graph, 0, 7) == NULL);
    addEdge(graph, 2, 7, 8, 2003);
    addEdge(graph, 5, 7, 5, 2004);

    assert(bestPath(graph, 0, 7) == NULL);

    removeEdge(graph, 0, 1);

    List *lista = bestPath(graph, 0, 7);

    Element *elem = lista->begin->next;


    int i=0;

    while(elem != lista->end)
    {
        printf("%d ", ((OrientedEdge*)elem->value)->edge->builtYear);
        free(elem->value);
        elem = elem->next;
    }

    deleteList(lista,0);
    deleteGraph(graph);

    printf("\ndijkstra OK\n");
}

void routeBasic()
{
    Map *map = newMap();

    addRoad(map, "A", "B", 1, 2019);
    addRoad(map, "C", "B", 1, 2019);
    addRoad(map, "C", "D", 1, 2019);

    newRoute(map, 12, "A", "D");
    getRouteDescription(map, 12);

    addRoad(map, "B", "X", 1, 2019);
    addRoad(map, "C", "Y", 1, 2019);
    addRoad(map, "X", "Y", 1, 2019);

    removeRoad(map, "C", "B");
    getRouteDescription(map, 12);

    deleteMap(map);

    printf("route OK\n");
}

void multipleRoutes()
{
    Map *map = newMap();

    addRoad(map, "a", "b", 1, 2018);
    addRoad(map, "b", "d", 1, 2018);
    addRoad(map, "a", "c", 1, 2018);
    addRoad(map, "c", "d", 1, 2018);

    assert(!newRoute(map, 1, "a", "d"));

    printf("multiple routes OK\n");

    deleteMap(map);
}
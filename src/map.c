#include "map.h"

#include <stdlib.h>
//#include <stdbool.h>

#include "graph.h"
#include "list.h"

typedef struct Map
{
    Graph *graph;
    // TODO lista dróg krajowych
    List *routeList;
    Hashtable **labels;
} Map;

Map *newMap(void)
{
    Map *map = calloc(1, sizeof(Map));

    if(map == NULL)
        return NULL;

    map->graph = newGraph();
    map->routeList = newList(NULL); // może da radę memory?
    map->labels = newHashtable(16, newMemory());

    if(map->graph == NULL || map->routeList == NULL || map->labels == NULL)
        return NULL;

    return map;
}

void deleteMap(Map *map)
{
    if(map == NULL)
        return;

    deleteGraph(map->graph);
    deleteList(map->routeList);
    deleteHashtable(map->labels);
    free(map);
}

// zwraca indeks miasta o podanej nazwie, a jeśli takie miasto nie istnieje to je tworzy(NULL wpp)
// zakłada poprawność nazwy miasta
int *getCity(Map *map, const char *city)
{
    int *v = hashtableGet(map->labels, city);

    if( v != NULL) // miasto już istnieje
        return v;

    v = addNode(map->graph);
    if(v == NULL) // nie udało się zaalokować
        return NULL;

    if(hashtableInsert(map->labels, city, v))
        return v;
    return NULL; // nie udało się dodać do hashtable

}

bool isNameCorrect(char *cityName)
{
    while(*cityName != 0)
    {
        if(*cityName == ';' || (*cityName <= 31 && *cityName >= 0))
            return false;
        cityName++;
    }
    return true;
}

bool addRoad(Map *map, const char *city1, const char *city2, unsigned length, int builtYear)
{
    if(!isNameCorrect(city1) || !isNameCorrect(city2)) // niepoprawna nazwa któregoś z miast
        return false;

    int *v1 = getCity(map, city1); // TODO ew. pokminić jak zrobić, żeby liczyć hasz tylko raz(a nie 2 przy get i insert)
    int *v2 = getCity(map, city2);

    if(v1 == NULL || v2 == NULL) // nie udało się stworzyć wierzchołków
        return false;

    return addEdge(map->graph, *v1, *v2, length, builtYear);
}

bool repairRoad(Map *map, const char *city1 , const char *city2, int repairYear)
{
    if(!isNameCorrect(city1) || !isNameCorrect(city2)) // niepoprawna nazwa któregoś z miast
        return false;

    int *v1 = hashtableGet(map->labels, city1);
    int *v2 = hashtableGet(map->labels, city2);

    if(v1 == NULL || v2 == NULL) // któreś z miast nie istnieje
        return false;

    Edge *edge = getEdge(map->graph, *v1, *v2);

    if(edge == NULL) // droga pomiędzy miastami nie istnieje
        return false;

    if(edge->builtYear > repairYear || repairYear == 0) // niepoprawny rok remontu
        return false;

    edge->builtYear = repairYear;
    return true;
}

bool newRoute(Map *map, unsigned routeId, const char *city1, const char *city2)
{
    return true;
}

bool extendRoute(Map *map, unsigned routeId, const char *city)
{
    return true;
}

bool removeRoad(Map *map, const char *city1, const char *city2)
{
    return true;
}

char const* getRouteDescription(Map *map, unsigned routeId)
{
    return "asdf";
}
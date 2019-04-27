#include "map.h"

#include <stdlib.h>
//#include <stdbool.h>

typedef struct Map
{
    Graph *graph;
    // TODO lista dróg krajowych
    List *routeList;
} Map;

Map *newMap(void)
{
    Map *map = calloc(1, sizeof(Map));

    if(map == NULL)
        return NULL;

    map->graph = newGraph();
    map->routeList = newList();

    if(map->graph == NULL || map->routeList == NULL)
        return NULL;

    return map;
}

void deleteMap(Map *map)
{
    if(map == NULL)
        return;

    deleteGraph(map->graph);
    deleteList(map->routeList);
    free(map);
}

bool addRoad(Map *map, const char *city1, const char *city2,
             unsigned length, int builtYear)
{
    if(addEdge(map->graph, cityHash1, cityHash2, length, builtYear))
        return true;
    else
        return false;
}

bool repairRoad(Map *map, const char *city1 , const char *city2, int repairYear)
{
//    if(!isInGraph(map->graph, city1) || !isInGraph(map->graph, city2))
//        return false;
    Edge *edge = getEdge(map->graph, city1, city2);

    if(edge == NULL)
        return false;

    if(edge->builtYear > repairYear)
        return false;

    edge->builtYear = repairYear;

    return true;
}

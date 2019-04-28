#include "map.h"

#include <stdlib.h>
//#include <stdbool.h>

#include "graph.h"
#include "list.h"

typedef struct Map
{
    Graph *graph;
    // TODO lista dróg krajowych
    List **routeList;
    Hashtable **labels;
} Map;

typedef struct Change
{
    int routeId;
    Element *placeOfChange; // TODO nazwa
    List *path;
} Change;

Map *newMap(void)
{
    Map *map = calloc(1, sizeof(Map));

    if(map == NULL)
        return NULL;

    map->graph = newGraph();
    map->routeList = calloc(1000, sizeof(List*)); //newList(NULL); // może da radę memory? TODO przypisać cos
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

    for(int i=0; i<1000; i++)
    {
        deleteList(map->routeList[i]);
    }

    deleteHashtable(map->labels);
    free(map);
}

Change *newChange(int routeId, Element *place, List *path)
{
    Change *change = calloc(1, sizeof(change));

    change->routeId = routeId;
    change->placeOfChange = place;
    change->path = path;

    return change;
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
    if(map->routeList[routeId] != NULL) // istnieje już droga krajowa o podanym numerze
        return false;

    if(routeId >= 1000) // niepoprawny numer drogi
        return false;

    if(!isNameCorrect(city1) || !isNameCorrect(city2)) // niepoprawna nazwa miasta
        return false;

    int *v1 = hashtableGet(map->labels, city1);
    int *v2 = hashtableGet(map->labels, city2);

    if(v1 == NULL || v2 == NULL) // któreś z podanych miast nie istnieje
        return false;

    if(v1 == v2) // podane miasta są identyczne
        return false;

    map->routeList[routeId] = bestPath(map->graph, v1, v2);

    if(map->routeList[routeId] == NULL)
        return false;

    return true;
}

int lastCityId(Map *map, unsigned routeId)
{
    Element *elem = map->routeList[routeId]->end->prev;

    int v = ((OrientedEdge*)elem->value)->v;
    Edge *edge = ((OrientedEdge*)elem->value)->edge;

    if(edge->v1 == v)
        return edge->v1;
    return edge->v2;
}

// zakłada poprawość route na wejściu
void setRouteVisited(Map *map, unsigned routeId)
{
    Element *elem = map->routeList[routeId]->begin->next;
    while(elem != map->routeList[routeId]->end)
    {
        int nodeId = ((OrientedEdge*)elem->value)->v;

        map->graph->nodeTable[nodeId]->visited = true;

        elem = elem->next;
    }
    int last = lastCityId(map, routeId);

    map->graph->nodeTable[last]->visited = true;
}

int routeLength(List *route)
{
    int ans = 0;
    Element *elem = route->begin->next;
    while(elem != route->end)
    {
        ans += ((OrientedEdge*)elem->value)->edge->length;
    }

    return ans;
}

bool extendRoute(Map *map, unsigned routeId, const char *city)
{
    if(routeId >= 1000 || map->routeList[routeId] == NULL) // nie ma drogi krajowej o podanym id
        return false;

    if(!isNameCorrect(city))
        return false;

    int *v = hashtableGet(map->labels, city);

    if(v == NULL)
        return false;

    int firstCity = ((OrientedEdge*)map->routeList[routeId]->begin->next->value)->v;
    int lastCity = lastCityId(map, routeId);


    // wyłącz wierzchołki drogi krajowej
    setRouteVisited(map, routeId);
    map->graph->nodeTable[firstCity]->visited =false;
    List *path1 = bestPath(map->graph, *v, firstCity);

    int pathLength1 = 0;
    int pathYear1 = maxYear+1;
    Element *elem1 = path1->begin->next;
    while(elem1 != path1->end)
    {
        pathLength1 += ((OrientedEdge*)elem1->value)->edge->length;
        pathYear1 = min(pathYear1, ((OrientedEdge*)elem1->value)->edge->builtYear);
    }

    setRouteVisited(map, routeId);
    List *path2 = bestPath(map->graph, lastCity, *v);

    int pathLength2 = 0;
    int pathYear2 = maxYear + 1;
    Element *elem2 = path2->begin->next;
    while(elem2 != path2->end)
    {
        pathLength2 += ((OrientedEdge*)elem2->value)->edge->length;
        pathYear2 = min(pathYear2, ((OrientedEdge*)elem2->value)->edge->builtYear);
    }

    if(pathLength1 < pathLength2 || (pathLength1 == pathLength2 && pathYear1 > pathLength2))
    {
        listInsertList(map->routeList[routeId]->begin, path1);
    }
    else
    {
        listInsertList(map->routeList[routeId]->end->prev, path2);
    }

    return true;
}

bool removeRoad(Map *map, const char *city1, const char *city2)
{
    if(!isNameCorrect(city1) || !isNameCorrect(city2))
        return false;

    int *v1 = hashtableGet(map->labels, city1);
    int *v2 = hashtableGet(map->labels, city2);

    if(v1 == NULL || v2 == NULL)
        return false;

    Edge *removedEdge = getEdge(map->graph, *v1, *v2);

//    Change

    for(int i=0; i<1000; i++) // TODO ew. dodać do edge listę, która będzie przechowywać jakie drogi przechodzą po krawędzi
    {
        // szukamy krawędzi removedEdge w route o id i
        Element *elem = map->routeList[i]->begin->next;
        while(elem != map->routeList[i]->end)
        {
            OrientedEdge *road = elem->value;
            if(road->edge == removedEdge)
            {
                // znaleźliśmy
                int otherCityId;
                if(road->edge->v1 == road->v)
                    otherCityId = road->edge->v2;
                else
                    otherCityId = road->edge->v1;

                // odwiedzamy wierzchołki
                setRouteVisited(map, i);
                map->graph->nodeTable[otherCityId]->visited = false;
                List *path = bestPath(map->graph, road->v, otherCityId);

                if(path != NULL) // nie da się. usuwamy Change
                {

                }

                break;
            }
        }
    }

    return true;
}

char const* getRouteDescription(Map *map, unsigned routeId)
{
    return "asdf";
}
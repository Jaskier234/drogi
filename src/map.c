#define _GNU_SOURCE
#include "map.h"

#include <stdlib.h>
#include <stdio.h>

#include "list.h"
#include "hashtable.h"
#include "string_ext.h"
#include "memory.h"
#include "valid.h"

/**
 * Struktura przechowująca mapę dróg krajowych.
 */
typedef struct Map
{
    Graph *graph; ///< Graf przechowujący inforamcje o połączeniach drogowych
    ///< w mapie
    List **routeList; ///< Lista dróg krajowych. Elementem listy jest
    ///< OrientedEdge
    Hashtable **labels; ///< Tablica haszująca, przechowująca nazwy miast.
    ///< Zwraca indeks wierzchołka, który reprezentuje miasto o podanej nazwie.
    Vector *names; ///< Wektor przechowujący nazwy maist.
} Map;

Map *newMap(void)
{
    Map *map = calloc(1, sizeof(Map));

    if(map == NULL)
        return NULL;

    const int INIT_SIZE = 32;

    map->graph = newGraph();
    map->routeList = calloc(1000, sizeof(List*));
    map->labels = newHashtable(INIT_SIZE, newMemory());
    map->names = newVector();

    if(map->graph == NULL || map->routeList == NULL || map->labels == NULL || map->names == NULL)
    {
        deleteGraph(map->graph);
        free(map->routeList);
        deleteHashtable(map->labels);
        free(map->names);
        free(map);
        return NULL;
    }

    return map;
}

void deleteMap(Map *map)
{
    if(map == NULL)
        return;

    deleteGraph(map->graph);

    for(int i = 0; i < 1000; i++)
    {
        deleteList(map->routeList[i], true);
    }
    free(map->routeList);

    deleteHashtable(map->labels);

    for(int i = 0; i < map->names->filled; i++)
    {
        free(map->names->tab[i]);
    }
    deleteVector(map->names);

    free(map);
}

/**
 * Struktura przechowująca zmiany w strukturze dróg krajowych.
 * W funkcji @ref removeRoad zmiany najpierw dodawane są do struktury
 * @ref Change, a dopiero gdy wszystkie okażą się poprawne, są dodawane do
 * struktury dróg.
 */
typedef struct Change
{
    Element *positionOfChange; ///< element litsy dróg krajowych po którym należy
    ///< wstawić nowe krawędzie drogi
    List *path; ///< lista krawędzi, które należy wstawić do drogi
} Change;

/**
 * @brief Inicjalizuje i zwraca instancję struktury @ref Change
 * @param position - element litsy dróg krajowych po którym należy wstawić nowe
 * krawędzie drogi
 * @param path - lista krawędzi, które należy wstawić do drogi
 * @return wzkaźnik na strukturę Change, lub NULL, gdy nie udało się zaalokować pamięci
 */
static Change *newChange(Element *position, List *path)
{
    Change *change = calloc(1, sizeof(Change));

    change->positionOfChange = position;
    change->path = path;

    return change;
}

/**
 * Zwraca indeks miasta o podanej nazwie, a jeśli takie miasto nie istnieje
 * to je tworzy. Nie sprawdza poprawności nazwy miasta
 * @param map Mapa, z której zostanie zwrócony indeks miasta lub zostanie
 * utworzone nowe miasto.
 * @param city nazwa miasta.
 * @return Wskaźnik na indeks miasta lub NULL jeśli nie udało się zaalokować
 * pamięci.
 */
static int *getCity(Map *map, const char *city)
{
    int *v = hashtableGet(map->labels, city);

    if( v != NULL) // miasto już istnieje
        return v;

    char *cityCp = strdup(city);
    v = addNode(map->graph);
    if(v == NULL) // nie udało się zaalokować
    {
        free(cityCp);
        return NULL;
    }

    vectorPushBack(map->names, cityCp);

    if(hashtableInsert(map->labels, cityCp, v))
        return v;
    free(cityCp);
    return NULL;
}

bool addRoad(Map *map, const char *city1, const char *city2, unsigned length, int builtYear)
{
    if(!isNameCorrect(city1) || !isNameCorrect(city2)) // niepoprawna nazwa któregoś z miast
        return false;

    if(strcmp(city1, city2) == 0) // to samo miasto na wejściu
        return false;

    if(length == 0)
        return false;

    if(builtYear == 0)
        return false;

    int *v1 = getCity(map, city1);
    if(v1 == NULL)
        return false;

    int *v2 = getCity(map, city2);
    if(v2 == NULL)
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
    if(routeId <= 0 || routeId >= 1000) // zły numer drogi
        return false;

    if(map->routeList[routeId] != NULL) // istnieje już droga krajowa o podanym numerze
        return false;

    if(!isNameCorrect(city1) || !isNameCorrect(city2)) // niepoprawna nazwa miasta
        return false;

    int *v1 = hashtableGet(map->labels, city1);
    int *v2 = hashtableGet(map->labels, city2);

    if(v1 == NULL || v2 == NULL) // któreś z podanych miast nie istnieje
        return false;

    if(*v1 == *v2) // podane miasta są identyczne
        return false;

    map->routeList[routeId] = bestPath(map->graph, *v1, *v2);

    if(map->routeList[routeId] != NULL && map->routeList[routeId]->begin->value == map->graph->ambiguous)
    {
        map->routeList[routeId]->begin->value = NULL;
        deleteList(map->routeList[routeId], true);
        map->routeList[routeId] = NULL;
    }

    if(map->routeList[routeId] == NULL)
        return false;

    return true;
}

bool removeRoute(Map *map, unsigned routeId)
{
    if(routeId <= 0 || routeId >= 1000)
        return false;

    if(map->routeList[routeId] == NULL) // droga nie istnieje
        return false;

    deleteList(map->routeList[routeId]);
    map->routeList->[routeId] = NULL;

    return true;
}

/**
 * Zwraca id ostatniego miasta na drodze krajowej.
 * @param map Wskaźnik na mapę, w której jest droga, której ostatni wierzchołek
 * ma zostać zwrócony.
 * @param routeId Id drogi, której ostatni wierzchołek ma zostać zwrócony.
 * @return Id ostatniego wierzchołka na drodze krajowej.
 */
static int lastCityId(Map *map, unsigned routeId)
{
    Element *elem = map->routeList[routeId]->end->prev;

    int v = ((OrientedEdge*)elem->value)->v;
    Edge *edge = ((OrientedEdge*)elem->value)->edge;

    if(edge->v1 == v)
        return edge->v2;
    return edge->v1;
}

bool addRoute(Map *map, Vector *description)
{
    int routeId = stringToNum(description->tab[0]);
    if(map->routeList[routeId] != NULL) // droga już istnieje
        return false;

    List *visited = newList(NULL);

    for(int i = 4; i < description->filled; i += 3)
    {
        int *v1 = getCity(map, description->tab[i-3]); // hashtableGet(map->labels, description->tab[i-3]);
        int *v2 = getCity(map, description->tab[i]); //hashtableGet(map->labels, description->tab[i]);

        if(v1 == NULL || v2 == NULL)
        {
            foreach(it, visited)
            {
                int *nodeId = it->value;
                Node *node = map->graph->nodes->tab[*nodeId];
                node->visited = false;
            }
            deleteList(visited, false);
            return false;
        }

        Node *currentNode = map->graph->nodes->tab[*v2];
        if(currentNode->visited)
        {
            foreach(it, visited)
            {
                int *nodeId = it->value;
                Node *node = map->graph->nodes->tab[*nodeId];
                node->visited = false;
            }
            deleteList(visited, false);
            return false;
        }

        Edge *edge = getEdge(map->graph, *v1, *v2);
        if(edge != NULL)
        {
            if (stringToNum(description->tab[i - 2]) != edge->length)
            {
                foreach(it, visited)
                {
                    int *nodeId = it->value;
                    Node *node = map->graph->nodes->tab[*nodeId];
                    node->visited = false;
                }
                deleteList(visited, false);
                return false;
            }
            if (stringToNum(description->tab[i - 1]) < edge->builtYear)
            {
                foreach(it, visited)
                {
                    int *nodeId = it->value;
                    Node *node = map->graph->nodes->tab[*nodeId];
                    node->visited = false;
                }
                deleteList(visited, false);
                return false;
            }
        }

        Node *node1 = map->graph->nodes->tab[*v1];
        node1->visited = true;
        Node *node2 = map->graph->nodes->tab[*v2];
        node2->visited = true;

        if(i == 4)
            listPushBack(visited, v1, NULL);
        listPushBack(visited, v2, NULL);
    }

    foreach(it, visited)
    {
        int *nodeId = it->value;
        Node *node = map->graph->nodes->tab[*nodeId];
        node->visited = false;
    }
    deleteList(visited, false);

    List *route = newList(NULL);
    if(route == NULL)
        return false;

    for(int i = 4; i < description->filled; i += 3)
    {
        int *v1 = hashtableGet(map->labels, description->tab[i-3]);
        int *v2 = hashtableGet(map->labels, description->tab[i]);

        if(v1 == NULL || v2 == NULL || getEdge(map->graph, *v1, *v2) == NULL)
        {
            if(!addRoad(map, description->tab[i-3], description->tab[i], stringToNum(description->tab[i-2]), stringToNum(description->tab[i-1])))
            {
                deleteList(route, true);
                return false;
            }
            v1 = hashtableGet(map->labels, description->tab[i-3]);
            v2 = hashtableGet(map->labels, description->tab[i]);
        }

        Edge *edge = getEdge(map->graph, *v1, *v2); // Różne od NULL
        int64_t year = stringToNum(description->tab[i-1]);
        edge->builtYear = year;

        listPushBack(route, newOrientedEdge(edge, *v1), NULL);


    }
    map->routeList[routeId] = route;
    return true;
}


/**
 * Oznacza wierzchołki danej drogi krajowej jako odwiedzone, aby uniknąć powtórzeń
 * miast na drodze krajowej zakłada poprawość route na wejściu.
 * @param map Wskaźnik na mapę, w której jest droga.
 * @param routeId Id drogi krajowej.
 */
static void setRouteVisited(Map *map, unsigned routeId)
{
    foreach(it, map->routeList[routeId])
    {
        OrientedEdge *edge = it->value;
        Node *node = map->graph->nodes->tab[edge->v];
        node->visited = true;
    }

    int last = lastCityId(map, routeId);
    Node *node = map->graph->nodes->tab[last];
    node->visited = true;
}

bool extendRoute(Map *map, unsigned routeId, const char *city)
{
    if(routeId <= 0 || routeId >= 1000) // zły numer drogi
        return false;

    if(map->routeList[routeId] == NULL) // nie ma drogi krajowej o podanym id
        return false;

    if(!isNameCorrect(city))
        return false;

    int *v = hashtableGet(map->labels, city);

    if(v == NULL)
        return false;

    int firstCity = ((OrientedEdge*)map->routeList[routeId]->begin->next->value)->v;
    int lastCity = lastCityId(map, routeId);

    // sprawdzanie, czy wierzchołek znajduje się na drodze krajowej
    foreach(it, map->routeList[routeId])
    {
        OrientedEdge *edge = it->value;
        if(*v == edge->v)
            return false;
    }
    if(lastCity == *v)
        return false;

    // wyłącz wierzchołki drogi krajowej
    setRouteVisited(map, routeId);
    Node *node = map->graph->nodes->tab[firstCity];
    node->visited =false;

    List *path1 = bestPath(map->graph, *v, firstCity);

    int pathLength1 = 0;
    int pathYear1 = maxYear;
    if(path1 != NULL)
    {
        foreach(it, path1)
        {
            OrientedEdge *edge = it->value;
            pathLength1 += edge->edge->length;
            pathYear1 = min(pathYear1, edge->edge->builtYear);
        }
    }

    setRouteVisited(map, routeId);

    List *path2 = bestPath(map->graph, lastCity, *v);

    int pathLength2 = 0;
    int pathYear2 = maxYear;
    if(path2 != NULL)
    {
        foreach(it, path2)
        {
            OrientedEdge *edge = it->value;
            pathLength2 += edge->edge->length;
            pathYear2 = min(pathYear2, edge->edge->builtYear);
        }
    }

    // TODO dodać klamry w poniższych if-elsach

//    List *betterPath;


    if(path1 == NULL && path2 == NULL) // Nie udało się wyznaczyć żadnej ze ścieżek
        return false;

    if(path2 == NULL) // Wyznaczono jedną ze ścieżek
    {
        if(path1->begin->value == map->graph->ambiguous)
        {
            path1->begin->value = NULL;
            deleteList(path1, true);
            deleteList(path2, true);
            return false;
        }
        listInsertList(map->routeList[routeId]->begin, path1);
    }
    else if(path1 == NULL)
    {
        if(path2->begin->value == map->graph->ambiguous)
        {
            deleteList(path1, true);
            path2->begin->value = NULL;
            deleteList(path2, true);
            return false;
        }
        listInsertList(map->routeList[routeId]->end->prev, path2);
    }
    // Wyznaczono obie ścieżki, ale są niejednoznaczne
    else if( pathLength1 == pathLength2 && pathYear1 == pathYear2 )
    {
        path1->begin->value = NULL;
        deleteList(path1, true);
        path2->begin->value = NULL;
        deleteList(path2, true);
        return false;
    }
    // Wyznaczono obie ścieżki. Wybieramy lepszą.
    else if(pathLength1 < pathLength2 || (pathLength1 == pathLength2 && pathYear1 > pathYear2))
    {
        if(path1->begin->value == map->graph->ambiguous)
        {
            path1->begin->value = NULL;
            deleteList(path1, true);
            path2->begin->value = NULL;
            deleteList(path2, true);
            return false;
        }
        listInsertList(map->routeList[routeId]->begin, path1);
    }
    else
    {
        if(path2->begin->value == map->graph->ambiguous)
        {
            path1->begin->value = NULL;
            deleteList(path1, true);
            path2->begin->value = NULL;
            deleteList(path2, true);
            return false;
        }
        listInsertList(map->routeList[routeId]->end->prev, path2);
    }

    if(path1 != NULL)
        path1->begin->value = NULL;

    if(path2 != NULL)
        path2->begin->value = NULL;

    deleteList(path1, true);
    deleteList(path2, true);

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

    if(removedEdge == NULL)
        return false;

    removeEdge(map->graph, *v1, *v2);

    List *changes = newList(NULL);

    for(int i = 1; i < 1000; i++)
    {
        if(map->routeList[i] == NULL)
            continue;

        // szukamy krawędzi removedEdge w route o id i
        foreach(it, map->routeList[i])
        {
            OrientedEdge *road = it->value;
            if(eqEdges(road->edge, removedEdge))
            {
                // znaleźliśmy
                int otherCityId;
                if(road->edge->v1 == road->v)
                    otherCityId = road->edge->v2;
                else
                    otherCityId = road->edge->v1;

                // odwiedzamy wierzchołki
                setRouteVisited(map, i);
                Node *node = map->graph->nodes->tab[otherCityId];
                node->visited = false;

                List *path = bestPath(map->graph, road->v, otherCityId);

                if(path != NULL && path->begin->value == map->graph->ambiguous)
                {
                    path->begin->value = NULL;
                    deleteList(path, true);
                    path = NULL;
                }

                if(path != NULL) // dodajemy do Change
                {
                    listPushBack(changes, newChange(it->prev, path), NULL);
                }
                else // usuwamy całą zmianę
                {
                    foreach(elem, changes)
                    {
                        Change *change = elem->value;
                        deleteList(change->path, true);

                        free(elem->value);
                    }
                    deleteList(changes, 0);

                    Node *node1 = map->graph->nodes->tab[removedEdge->v1];
                    listPushBack(node1->edges, removedEdge, NULL);
                    Node *node2 = map->graph->nodes->tab[removedEdge->v2];
                    listPushBack(node2->edges, removedEdge, NULL);
                    return false;
                }

                break; // znaleźliśmy potrzbną krawędź. Nie trzeba dalej przeszukiwać listy krawędzi
            }
        }
    }

    free(removedEdge);

    foreach(it, changes)
    {
        Change *change = it->value;
        free(change->positionOfChange->next->value);
        listRemove(change->positionOfChange->next);
        listInsertList(change->positionOfChange, change->path);
        deleteList(change->path, false);
        free(change);
    }

    deleteList(changes, false);

    return true;
}

char const* getRouteDescription(Map *map, unsigned routeId)
{
    int size = 0;
    int allocated = 16;
    char *description = calloc(allocated, sizeof(char));

    if(routeId >= 1000 || routeId == 0 || map->routeList[routeId] == NULL) // droga nie istnieje
        return description;

    char *routeNr = intToString(routeId);
    description = concatenate(description, routeNr, &size, &allocated);
    free(routeNr);
    description = concatenate(description, ";", &size, &allocated);

    foreach(elem, map->routeList[routeId])
    {
        int id = ((OrientedEdge*)elem->value)->v;
        Edge *edge = ((OrientedEdge*)elem->value)->edge;

        description = concatenate(description, map->names->tab[id], &size, &allocated);
        description = concatenate(description, ";", &size, &allocated);

        char *roadLength = intToString(edge->length);
        description = concatenate(description, roadLength, &size, &allocated);
        free(roadLength);
        description = concatenate(description, ";", &size, &allocated);

        char *roadYear = intToString(edge->builtYear);
        description = concatenate(description, roadYear, &size, &allocated);
        free(roadYear);
        description = concatenate(description, ";", &size, &allocated);
    }

    description = concatenate(description, map->names->tab[lastCityId(map, routeId)], &size, &allocated);

    return description;
}

void printRoute(List *route)
{
    foreach(it, route)
    {
        OrientedEdge *edge = it->value;
        printf("%d ", edge->v);
    }

    OrientedEdge *last = route->end->prev->value;
    printf("%d\n", (last->edge->v2 == last->v)?last->edge->v1:last->edge->v2 );
}

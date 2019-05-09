#include "map.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "graph.h"
#include "list.h"

//typedef struct Map
//{
//    Graph *graph;
//    List **routeList;
//    Hashtable **labels;
//    const char **names;
//    int namesSize;
//} Map;

/**
 * Struktura przechowująca zmiany w strukturze dróg krajowych.
 * W funkcji @ref removeRoad zmiany najpierw dodawane są do struktury
 * @ref Change, a dopiero gdy wszystkie okarzą się poprawne, są dodawane do
 * struktury dróg.
 */
typedef struct Change
{
    Element *positionOfChange; ///< element litsy dróg krajowych po którym należy wstawić nowe krawędzie drogi
    List *path; ///< lista krawędzi, które należy wstawić do drogi
} Change;

Map *newMap(void)
{
    Map *map = calloc(1, sizeof(Map));

    if(map == NULL)
        return NULL;

    const int INIT_SIZE = 32;

    map->graph = newGraph();
    map->routeList = calloc(1000, sizeof(List*));
    map->labels = newHashtable(INIT_SIZE, newMemory());
    map->namesSize = INIT_SIZE;
    map->names = calloc(INIT_SIZE, sizeof(char*));

    if(map->graph == NULL || map->routeList == NULL || map->labels == NULL || map->names == NULL)
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
        if(map->routeList[i] != NULL)
        {
            Element *elem = map->routeList[i]->begin->next;
            while(elem != map->routeList[i]->end)
            {
                free(elem->value);

                elem = elem->next;
            }
        }
        deleteList(map->routeList[i], 0);
    }

    deleteHashtable(map->labels);
    free(map);
}

/**
 * @brief Inicjalizuje i zwraca instancję struktury @ref Change
 * @param position - element litsy dróg krajowych po którym należy wstawić nowe krawędzie drogi
 * @param path - lista krawędzi, które należy wstawić do drogi
 * @return wzkaźnik na strukturę Change, lub NULL, gdy nie udało się zaalokować pamięci
 */
Change *newChange(Element *position, List *path)
{
    Change *change = calloc(1, sizeof(Change));

    change->positionOfChange = position;
    change->path = path;

    return change;
}

// Tworzy kopię napisu i zwraca wskaźnik na nią
char *strdup(const char *s)
{
    char* p = calloc(strlen(s)+1, sizeof(char));
    if (p) strcpy(p, s);
    return p;
}

// zwraca indeks miasta o podanej nazwie, a jeśli takie miasto nie istnieje to je tworzy
// zwraca NULL, gdy nie udało się zaalokować pamięci
// zakłada poprawność nazwy miasta
int *getCity(Map *map, const char *city)
{
    int *v = hashtableGet(map->labels, city);

    if( v != NULL) // miasto już istnieje
        return v;

    char *cityCp = strdup(city); // TODO usunąć duplikat nazwy z pamięci
    v = addNode(map->graph);
    if(v == NULL) // nie udało się zaalokować
    {
        free(cityCp);
        return NULL;
    }

    map->names[*v] = cityCp;

    if(hashtableInsert(map->labels, cityCp, v))
        return v;
    free(cityCp);
    return NULL; // nie udało się dodać do hashtable // trzeba usunąć wierzchołek z grafu
}

// Sprawdza poprawność nazwy miasta
bool isNameCorrect(const char *cityName)
{
    if(*cityName == 0) // pusty napis
        return false;

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

    if(strcmp(city1, city2) == 0) // to samo miasto na wejściu
        return false;

    int *v1 = getCity(map, city1);
    int *v2 = getCity(map, city2);

    if(v1 == NULL || v2 == NULL) // nie udało się stworzyć wierzchołków
        return false;

    if(length == 0)
        return false;

    if(builtYear == 0)
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
    if(routeId >= 1000 || map->routeList[routeId] != NULL) // istnieje już droga krajowa o podanym numerze
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

    if(map->routeList[routeId] == NULL)
        return false;

    return true;
}

// Zwraca id ostatniego miasta na drodze krajowej
int lastCityId(Map *map, unsigned routeId)
{
    Element *elem = map->routeList[routeId]->end->prev;

    int v = ((OrientedEdge*)elem->value)->v;
    Edge *edge = ((OrientedEdge*)elem->value)->edge;

    if(edge->v1 == v)
        return edge->v2;
    return edge->v1;
}

// Oznacza wierzchołki danej drogi krajowej jako odwiedzone, aby uniknąć powtórzeń
// miast na drodze krajowej
// zakłada poprawość route na wejściu
void setRouteVisited(Map *map, unsigned routeId)
{
    Element *elem = map->routeList[routeId]->begin->next;
    while(elem != map->routeList[routeId]->end)
    {
        int nodeId = ((OrientedEdge*)elem->value)->v;

        map->graph->nodes[nodeId]->visited = true;

        elem = elem->next;
    }
    int last = lastCityId(map, routeId);

    map->graph->nodes[last]->visited = true;
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

    // sprawdzanie, czy wierzchołek znajduje się na drodze krajowej
    Element *elem = map->routeList[routeId]->begin->next;
    while(elem != map->routeList[routeId]->end)
    {
        if( *v == ((OrientedEdge*)elem->value)->v )
            return false;
        elem = elem->next;
    }
    if(lastCity == *v)
        return false;

    // wyłącz wierzchołki drogi krajowej
    setRouteVisited(map, routeId);
    map->graph->nodes[firstCity]->visited =false;
    List *path1 = bestPath(map->graph, *v, firstCity);

    int pathLength1 = 0;
    int pathYear1 = maxYear;
    if(path1 != NULL)
    {
        Element *elem1 = path1->begin->next;
        while(elem1 != path1->end)
        {
            pathLength1 += ((OrientedEdge*)elem1->value)->edge->length;
            pathYear1 = min(pathYear1, ((OrientedEdge*)elem1->value)->edge->builtYear);

            elem1 = elem1->next;
        }
    }

    setRouteVisited(map, routeId);
    List *path2 = bestPath(map->graph, lastCity, *v);

    int pathLength2 = 0;
    int pathYear2 = maxYear + 1;
    if(path2 != NULL)
    {
        Element *elem2 = path2->begin->next;
        while(elem2 != path2->end)
        {
            pathLength2 += ((OrientedEdge*)elem2->value)->edge->length;
            pathYear2 = min(pathYear2, ((OrientedEdge*)elem2->value)->edge->builtYear);

            elem2 = elem2->next;
        }
    }

    if(path1 == NULL && path2 == NULL) return false;
    if(path2 == NULL) listInsertList(map->routeList[routeId]->begin, path1);
    else if(path1 == NULL) listInsertList(map->routeList[routeId]->end->prev, path2);
    else if(pathLength1 < pathLength2 || (pathLength1 == pathLength2 && pathYear1 > pathLength2))
        listInsertList(map->routeList[routeId]->begin, path1);
    else
        listInsertList(map->routeList[routeId]->end->prev, path2);

    deleteList(path1, true);
    deleteList(path2, true);

    return true;
}

bool eqEdges(Edge *edge1, Edge *edge2)
{
    if(edge1->builtYear != edge2->builtYear)
        return false;

    if(edge1->length != edge2->length)
        return false;

    if( (edge1->v1 == edge2->v1 && edge1->v2 == edge2->v2) || (edge1->v1 == edge2->v2 && edge1->v2 == edge2->v1) )
        return true;
    return false;
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

    for(int i=0; i<1000; i++)
    {
        if(map->routeList[i] == NULL)
            continue;

        // szukamy krawędzi removedEdge w route o id i
        Element *elem = map->routeList[i]->begin->next;
        while(elem != map->routeList[i]->end)
        {
            OrientedEdge *road = elem->value;
            if(eqEdges(road->edge, removedEdge)) // robocza poprawka
            {
                // znaleźliśmy
                int otherCityId;
                if(road->edge->v1 == road->v)
                    otherCityId = road->edge->v2;
                else
                    otherCityId = road->edge->v1;

                // odwiedzamy wierzchołki
                setRouteVisited(map, i);
                map->graph->nodes[otherCityId]->visited = false;
                List *path = bestPath(map->graph, road->v, otherCityId);

                if(path != NULL) // dodajemy do Change
                {
                    listPushBack(changes, newChange(elem->prev, path), NULL);
//                    listRemove(elem);
                }
                else // usuwamy całą zmianę
                {
                    Element *elem = changes->begin->next;
                    while(elem != changes->end)
                    {
//                        Change *change = elem->value;

//                        listInsertList(change->positionOfChange, change->path);

                        free(elem->value);

                        elem = elem->next;
                    }
                    deleteList(changes, 0);
//                    changes = NULL;
                    addEdge(map->graph, *v1, *v2, removedEdge->length, removedEdge->builtYear);
                    return false;
                }

                break; // znaleźliśmy potrzbną krawędź. Nie trzeba dalej przeszukiwać listy krawędzi
            }
            elem = elem->next;
        }
    }

    free(removedEdge);

    if(changes != NULL)
    {
        Element *elem = changes->begin->next;
        while(elem != changes->end)
        {
            Change *change = (Change*)elem->value;
            listRemove(change->positionOfChange->next);
            listInsertList(change->positionOfChange, change->path);
            free(elem->value);
            elem = elem->next;
        }
    }

    deleteList(changes, 0);

    return true;
}

// Konwertuje liczbę na napis
char *intToString(int a)
{
    char *string = calloc(20, sizeof(char));
    int size=0;

    bool minus = (a<0);
    if(a<0)
        a *= -1;

    while(a > 0)
    {
        string[size] = a%10 + '0';
        size++;
        a /= 10;
    }

    if(minus)
    {
        string[size] = '-';
        size++;
    }

    for(int i=0; i<size/2; i++)
    {
        char temp = string[i];
        string[i] = string[size-1-i];
        string[size-1-i] = temp;
    }

    return string;
}

// Łączy dwa napisy
char *concatenate(char *string1, const char *string2, int *size, int *allocated)
{
    while(*string2 != 0)
    {
        string1[*size] = *string2;
        (*size)++;

        if( *size+1 >= *allocated )
        {
            (*allocated) *= 2;
            string1 = realloc(string1, *allocated);
            if(string1 == NULL)
                return NULL;
        }

        string2++;
    }
    string1[*size] = 0;

    return string1;
}

char const* getRouteDescription(Map *map, unsigned routeId)
{
    int size = 0;
    int allocated = 16;
    char *description = calloc(allocated, sizeof(char));

    if(routeId >= 1000 || map->routeList[routeId] == NULL) // droga nie istnieje
        return description;

    char *routeNr = intToString(routeId);
    description = concatenate(description, routeNr, &size, &allocated);
    free(routeNr);
    description = concatenate(description, ";", &size, &allocated);

    Element *elem = map->routeList[routeId]->begin->next;

    while(elem != map->routeList[routeId]->end)
    {
        int id = ((OrientedEdge*)elem->value)->v;
        Edge *edge = ((OrientedEdge*)elem->value)->edge;

        description = concatenate(description, map->names[id], &size, &allocated);
        description = concatenate(description, ";", &size, &allocated);

        char *roadLength = intToString(edge->length);
        description = concatenate(description, roadLength, &size, &allocated);
        free(roadLength);
        description = concatenate(description, ";", &size, &allocated);

        char *roadYear = intToString(edge->builtYear);
        description = concatenate(description, roadYear, &size, &allocated);
        free(roadYear);
        description = concatenate(description, ";", &size, &allocated);

        elem = elem->next;
    }

    description = concatenate(description, map->names[lastCityId(map, routeId)], &size, &allocated);

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
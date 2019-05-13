#define _GNU_SOURCE
#include "map.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "graph.h"
#include "list.h"
#include "hashtable.h"

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

/**
 * Struktura przechowująca zmiany w strukturze dróg krajowych.
 * W funkcji @ref removeRoad zmiany najpierw dodawane są do struktury
 * @ref Change, a dopiero gdy wszystkie okażą się poprawne, są dodawane do
 * struktury dróg.
 */

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

    for(int i=0; i<1000; i++)
    {
        deleteList(map->routeList[i], true);
    }
    free(map->routeList);

    deleteHashtable(map->labels);

    for(int i=0; i<map->names->filled; i++)
    {
        free(map->names->tab[i]);
    }
    deleteVector(map->names);

    free(map);
}

/**
 * @brief Inicjalizuje i zwraca instancję struktury @ref Change
 * @param position - element litsy dróg krajowych po którym należy wstawić nowe
 * krawędzie drogi
 * @param path - lista krawędzi, które należy wstawić do drogi
 * @return wzkaźnik na strukturę Change, lub NULL, gdy nie udało się zaalokować pamięci
 */

typedef struct Change
{
    Element *positionOfChange; ///< element litsy dróg krajowych po którym należy
    ///< wstawić nowe krawędzie drogi
    List *path; ///< lista krawędzi, które należy wstawić do drogi
} Change;

static Change *newChange(Element *position, List *path)
{
    Change *change = calloc(1, sizeof(Change));

    change->positionOfChange = position;
    change->path = path;

    return change;
}

// todo moduł ext_string

// Zwraca indeks miasta o podanej nazwie, a jeśli takie miasto nie istnieje to je tworzy.
// Zwraca NULL, gdy nie udało się zaalokować pamięci.
// Zakłada poprawność nazwy miasta.
/**
 * Zwraca indeks miasta o podanej nazwie, a jeśli takie miasto nie istnieje
 * to je tworzy.
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
    return NULL; // nie udało się dodać do hashtable // trzeba usunąć wierzchołek z grafu
    // todo usuwanie ostatniego wierzchołka
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

    if(length == 0)
        return false;

    if(builtYear == 0)
        return false;

    int *v1 = getCity(map, city1); // TODO co jeśli nie uda się druga alokacja?
    if(v1 == NULL)
        return false;

    int *v2 = getCity(map, city2);
    if(v2 == NULL)
        return false;

//    if(v1 == NULL || v2 == NULL) // nie udało się stworzyć wierzchołków
//    {
//
//        return false;
//    }

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

    if(map->routeList[routeId] == map->graph->ambiguous)
        map->routeList[routeId] = NULL;

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

    // Któraś ze ścieżek jest niejednoznaczna
    if(path1 == map->graph->ambiguous)
        return false;
    if(path2 == map->graph->ambiguous)
        return false;

    if(path1 == NULL && path2 == NULL) // Nie udało się wyznaczyć żadnej ze ścieżek
        return false;

    if(path2 == NULL) // Wyznaczono jedną ze ścieżek
        listInsertList(map->routeList[routeId]->begin, path1);
    else if(path1 == NULL)
        listInsertList(map->routeList[routeId]->end->prev, path2);
    // Wyznaczono obie ścieżki, ale są niejednoznaczne
    else if( pathLength1 == pathLength2 && pathYear1 == pathYear2 )
        return false;
    // Wyznaczono obie ścieżki. Wybieramy lepszą.
    else if(pathLength1 < pathLength2 || (pathLength1 == pathLength2 && pathYear1 > pathLength2))
        listInsertList(map->routeList[routeId]->begin, path1);
    else
        listInsertList(map->routeList[routeId]->end->prev, path2);

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

    for(int i=0; i<1000; i++)
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

                if(path == map->graph->ambiguous)
                    path = NULL;

                if(path != NULL) // dodajemy do Change
                {
                    listPushBack(changes, newChange(it->prev, path), NULL);
                }
                else // usuwamy całą zmianę
                {
                    foreach(elem, changes)
                    {
                        Change *change = it->value;
                        deleteList(change->path, true);

                        free(it->value);
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
char *intToString(int a) // TODO naprawić dla 0
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
// todo do ext_string
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

        elem = elem->next;
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
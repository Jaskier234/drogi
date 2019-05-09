#include "map.h"
#include "graph.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#undef NDEBUG

#include <assert.h>

int main() {
//  char const* str;

    char const *descr;
    Map *m;

    // Błędna nazwa miasta
    m = newMap();
    assert(m);
    assert(!addRoad(m, "\30", "B", 1, 2020));
    deleteMap(m);

    // Błędna nazwa miasta 2
    m = newMap();
    assert(m);
    assert(!addRoad(m, ";", "B", 1, 2020));
    deleteMap(m);

    // Podstawowy poprawny test
    m = newMap();
    assert(m);
    assert(addRoad(m, "A", "B", 1, 2020));
    assert(addRoad(m, "B", "C", 1, 2020));
    assert(newRoute(m, 42, "A", "C"));
    descr = getRouteDescription(m, 42);
    //printf("%s\n", descr);
    assert(strcmp(descr, "42;A;1;2020;B;1;2020;C") == 0);
    free((void*)descr);
    deleteMap(m);

    // Rozłączne grafy
    m = newMap();
    assert(m);
    assert(addRoad(m, "A", "B", 1, 2020));
    assert(addRoad(m, "C", "D", 1, 2020));
    assert(!newRoute(m, 42, "A", "D"));
    deleteMap(m);

    // Brak jednoznacznej ścieżki
    m = newMap();
    assert(m);
    assert(addRoad(m, "A", "B", 1, 2020));
    assert(addRoad(m, "A", "C", 1, 2020));
    assert(addRoad(m, "B", "D", 1, 2020));
    assert(addRoad(m, "C", "D", 1, 2020));
    assert(!newRoute(m, 42, "A", "D"));
    deleteMap(m);

    // Brak jednoznacznej ścieżki, "niebezpośrednio"
    m = newMap();
    assert(m);
    assert(addRoad(m, "A", "B", 1, 2020));
    assert(addRoad(m, "B", "C", 1, 2020));
    assert(addRoad(m, "B", "D", 1, 2020));
    assert(addRoad(m, "C", "E", 1, 2020));
    assert(addRoad(m, "D", "E", 1, 2020));
    assert(addRoad(m, "E", "F", 1, 2020));
    assert(!newRoute(m, 42, "A", "F"));
    deleteMap(m);

    // Brak jednoznacznej najkrótszej ścieżki, istnieje zła alternatywna
    m = newMap();
    assert(m);
    assert(addRoad(m, "A", "B", 1, 2020));
    assert(addRoad(m, "B", "C", 1, 2020));
    assert(addRoad(m, "B", "D", 1, 2020));
    assert(addRoad(m, "C", "E", 1, 2020));
    assert(addRoad(m, "D", "E", 1, 2020));
    assert(addRoad(m, "E", "F", 1, 2020));
    assert(addRoad(m, "A", "G", 100, 2020));
    assert(addRoad(m, "G", "F", 100, 2020));
    assert(!newRoute(m, 42, "A", "F"));
    deleteMap(m);

    // Dobra najkrótsza ścieżka, ale dłuższa niejednoznaczna
    m = newMap();
    assert(m);
    assert(addRoad(m, "A", "B", 2, 2020));
    assert(addRoad(m, "B", "C", 2, 2020));
    assert(addRoad(m, "B", "D", 2, 2020));
    assert(addRoad(m, "C", "E", 2, 2020));
    assert(addRoad(m, "D", "E", 2, 2020));
    assert(addRoad(m, "E", "F", 2, 2020));
    assert(addRoad(m, "A", "G", 2, 2020));
    assert(addRoad(m, "G", "F", 1, 2020));
    assert(newRoute(m, 42, "A", "F"));
    descr = getRouteDescription(m, 42);
    assert(strcmp(descr, "42;A;2;2020;G;1;2020;F") == 0);
    free((void*)descr);
    deleteMap(m);

    // Podstawowe repairRoad
    m = newMap();
    assert(m);
    assert(addRoad(m, "A", "B", 1, 2020));
    assert(addRoad(m, "B", "C", 1, 2020));
    assert(newRoute(m, 42, "A", "C"));
    descr = getRouteDescription(m, 42);
    assert(strcmp(descr, "42;A;1;2020;B;1;2020;C") == 0);
    free((void*)descr);
    assert(repairRoad(m, "A", "B", 2022));
    descr = getRouteDescription(m, 42);
    assert(strcmp(descr, "42;A;1;2022;B;1;2020;C") == 0);
    free((void*)descr);
    deleteMap(m);

    // Błędne repairRoad: wcześniejszy rok
    m = newMap();
    assert(m);
    assert(addRoad(m, "A", "B", 1, 2020));
    assert(addRoad(m, "B", "C", 1, 2020));
    assert(newRoute(m, 42, "A", "C"));
    descr = getRouteDescription(m, 42);
    assert(strcmp(descr, "42;A;1;2020;B;1;2020;C") == 0);
    free((void*)descr);
    assert(!repairRoad(m, "A", "B", 1999));
    descr = getRouteDescription(m, 42);
    assert(strcmp(descr, "42;A;1;2020;B;1;2020;C") == 0);
    free((void*)descr);
    deleteMap(m);

    // Podstawowe extendRoute dodające na koniec
    m = newMap();
    assert(m);
    assert(addRoad(m, "A", "B", 1, 2020));
    assert(addRoad(m, "B", "C", 1, 2020));
    assert(newRoute(m, 42, "A", "C"));
    descr = getRouteDescription(m, 42);
    assert(strcmp(descr, "42;A;1;2020;B;1;2020;C") == 0);
    free((void*)descr);
    assert(addRoad(m, "C", "D", 1, 2020));
    assert(extendRoute(m, 42, "D"));
    descr = getRouteDescription(m, 42);
    assert(strcmp(descr, "42;A;1;2020;B;1;2020;C;1;2020;D") == 0);
    free((void*)descr);
    deleteMap(m);

    // Podstawowe extendRoute dodające na początek
    m = newMap();
    assert(m);
    assert(addRoad(m, "A", "B", 1, 2020));
    assert(addRoad(m, "B", "C", 1, 2020));
    assert(newRoute(m, 42, "A", "C"));
    descr = getRouteDescription(m, 42);
    assert(strcmp(descr, "42;A;1;2020;B;1;2020;C") == 0);
    free((void*)descr);
    assert(addRoad(m, "D", "A", 1, 2020));
    assert(extendRoute(m, 42, "D"));
    descr = getRouteDescription(m, 42);
    assert(strcmp(descr, "42;D;1;2020;A;1;2020;B;1;2020;C") == 0);
    free((void*)descr);
    deleteMap(m);

    // Brak jednoznacznej ścieżki przy extendRoad - z końca i początku
    m = newMap();
    assert(m);
    assert(addRoad(m, "A", "B", 1, 2020));
    assert(addRoad(m, "B", "C", 1, 2020));
    assert(newRoute(m, 42, "A", "C"));
    descr = getRouteDescription(m, 42);
    assert(strcmp(descr, "42;A;1;2020;B;1;2020;C") == 0);
    free((void*)descr);
    assert(addRoad(m, "D", "A", 1, 2020));
    assert(addRoad(m, "D", "C", 1, 2020));
//    printGraph(m->graph);
// TODO poprawić
//    assert(!extendRoute(m, 42, "D"));
    deleteMap(m);

    // extendRoute do miasta już w drodze
    m = newMap();
    assert(m);
    assert(addRoad(m, "A", "B", 1, 2020));
    assert(addRoad(m, "B", "C", 1, 2020));
    assert(addRoad(m, "C", "D", 1, 2020));
    assert(newRoute(m, 42, "A", "D"));
    descr = getRouteDescription(m, 42);
    assert(strcmp(descr, "42;A;1;2020;B;1;2020;C;1;2020;D") == 0);
    free((void*)descr);
    assert(!extendRoute(m, 42, "C"));
    deleteMap(m);

    // Proste extendRoute z lepszą drogą od jednej strony
    m = newMap();
    assert(m);
    assert(addRoad(m, "A", "B", 1, 2020));
    assert(addRoad(m, "B", "C", 1, 2020));
    assert(newRoute(m, 42, "A", "C"));
    descr = getRouteDescription(m, 42);
    assert(strcmp(descr, "42;A;1;2020;B;1;2020;C") == 0);
    free((void*)descr);
    assert(addRoad(m, "D", "A", 1, 2021));
    assert(addRoad(m, "D", "C", 1, 2020));
    assert(extendRoute(m, 42, "D"));
    descr = getRouteDescription(m, 42);
    assert(strcmp(descr, "42;D;1;2021;A;1;2020;B;1;2020;C") == 0);
    free((void*)descr);
    deleteMap(m);

    // Proste removeRoad
    m = newMap();
    assert(m);
    assert(addRoad(m, "A", "B", 1, 2020));
    assert(addRoad(m, "B", "C", 1, 2020));
    assert(addRoad(m, "C", "D", 1, 2020));
    assert(addRoad(m, "B", "E", 1, 2020));
    assert(addRoad(m, "C", "E", 1, 2020));
    assert(newRoute(m, 42, "A", "D"));
    descr = getRouteDescription(m, 42);
    assert(strcmp(descr, "42;A;1;2020;B;1;2020;C;1;2020;D") == 0);
    free((void*)descr);
    assert(removeRoad(m, "B", "C"));
    descr = getRouteDescription(m, 42);
    assert(strcmp(descr, "42;A;1;2020;B;1;2020;E;1;2020;C;1;2020;D") == 0);
    free((void*)descr);
    deleteMap(m);

    // removeRoad z nieistniejącą poprawną drogą
    m = newMap();
    assert(addRoad(m, "A", "D", 10, 2020));
    assert(addRoad(m, "A", "B", 1, 2020));
    assert(addRoad(m, "B", "C", 1, 2020));
    assert(addRoad(m, "C", "D", 1, 2020));
    assert(newRoute(m, 42, "A", "D"));
    descr = getRouteDescription(m, 42);
    assert(strcmp(descr, "42;A;1;2020;B;1;2020;C;1;2020;D") == 0);
    free((void*)descr);
    assert(!removeRoad(m, "B", "C"));
    deleteMap(m);

    // removeRoad z błędną krótką drogą, dobrą długą
    m = newMap();
    assert(addRoad(m, "A", "D", 10, 2020));
    assert(addRoad(m, "A", "B", 1, 2020));
    assert(addRoad(m, "B", "C", 1, 2020));
    assert(addRoad(m, "C", "D", 1, 2020));
    assert(newRoute(m, 42, "A", "D"));
    descr = getRouteDescription(m, 42);
    assert(strcmp(descr, "42;A;1;2020;B;1;2020;C;1;2020;D") == 0);
    free((void*)descr);
    assert(addRoad(m, "B", "E", 1000, 2020));
    assert(addRoad(m, "C", "E", 1000, 2020));
    assert(removeRoad(m, "B", "C"));
    descr = getRouteDescription(m, 42);
    assert(strcmp(descr, "42;A;1;2020;B;1000;2020;E;1000;2020;C;1;2020;D") == 0);
    free((void*)descr);
    deleteMap(m);

    // removeRoad z błędną drogą zaczynającą się później niż początek
    m = newMap();
    assert(addRoad(m, "A", "D", 10, 2020));
    assert(addRoad(m, "A", "B", 1, 2020));
    assert(addRoad(m, "B", "C", 1, 2020));
    assert(addRoad(m, "C", "D", 1, 2020));
    assert(addRoad(m, "A", "X", 1, 2020));
    assert(addRoad(m, "Y", "X", 1, 2020));
    assert(addRoad(m, "Y", "B", 1, 2020));
    assert(newRoute(m, 42, "A", "D"));
    descr = getRouteDescription(m, 42);
    assert(strcmp(descr, "42;A;1;2020;B;1;2020;C;1;2020;D") == 0);
    free((void*)descr);
    assert(!removeRoad(m, "B", "C"));
    deleteMap(m);

    // removeRoad na środku
    m = newMap();
    assert(addRoad(m, "A", "B", 1, 2020));
    assert(addRoad(m, "B", "C", 1, 2020));
    assert(addRoad(m, "C", "D", 1, 2020));
    assert(newRoute(m, 42, "A", "D"));
    descr = getRouteDescription(m, 42);
    assert(strcmp(descr, "42;A;1;2020;B;1;2020;C;1;2020;D") == 0);
    free((void*)descr);
    assert(addRoad(m, "B", "E", 1, 2020));
    assert(addRoad(m, "C", "E", 1, 2020));
    assert(removeRoad(m, "B", "C"));
    descr = getRouteDescription(m, 42);
    assert(strcmp(descr, "42;A;1;2020;B;1;2020;E;1;2020;C;1;2020;D") == 0);
    free((void*)descr);
    deleteMap(m);

    // removeRoad z ponownym dodaniem
    m = newMap();
    assert(addRoad(m, "A", "B", 1, 2020));
    assert(addRoad(m, "B", "C", 1, 2020));
    assert(addRoad(m, "A", "C", 1000, 2020));
    assert(removeRoad(m, "A", "C"));
    assert(addRoad(m, "A", "C", 1, 2020));
    assert(newRoute(m, 42, "A", "C"));
    descr = getRouteDescription(m, 42);
    assert(strcmp(descr, "42;A;1;2020;C") == 0);
    free((void*)descr);
    deleteMap(m);

    //TODO zrobić w drugą stronę
    // removeRoad drogi krajowej długości 1
    m = newMap();
    assert(addRoad(m, "A", "B", 1, 2020));
    assert(newRoute(m, 42, "A", "B"));
    descr = getRouteDescription(m, 42);
    assert(strcmp(descr, "42;A;1;2020;B") == 0);
    free((void*)descr);
    assert(!removeRoad(m, "A", "B"));
    deleteMap(m);

    // Droga od tyłu
    m = newMap();
    assert(addRoad(m, "A", "B", 1, 2020));
    assert(addRoad(m, "B", "C", 1, 2020));
    assert(addRoad(m, "C", "A", 1, 2020));
    assert(newRoute(m, 42, "A", "B"));
    descr = getRouteDescription(m, 42);
    assert(strcmp(descr, "42;A;1;2020;B") == 0);
    free((void*)descr);
    assert(removeRoad(m, "B", "A"));
    descr = getRouteDescription(m, 42);
    assert(strcmp(descr, "42;A;1;2020;C;1;2020;B") == 0);
    free((void*)descr);
    deleteMap(m);

    // removeRoad, jedna starsza opcja
    m = newMap();
    assert(addRoad(m, "A", "B", 1, 2020));
    assert(addRoad(m, "B", "C", 1, 2020));
    assert(addRoad(m, "C", "D", 1, 2020));
    assert(newRoute(m, 42, "A", "D"));
    descr = getRouteDescription(m, 42);
    assert(strcmp(descr, "42;A;1;2020;B;1;2020;C;1;2020;D") == 0);
    free((void*)descr);
    assert(addRoad(m, "B", "E", 1, 2020));
    assert(addRoad(m, "C", "E", 1, 2020));
    assert(addRoad(m, "C", "F", 1, 2020));
    assert(addRoad(m, "B", "F", 1, 2012));
    assert(removeRoad(m, "B", "C"));
    descr = getRouteDescription(m, 42);
    assert(strcmp(descr, "42;A;1;2020;B;1;2020;E;1;2020;C;1;2020;D") == 0);
    free((void*)descr);
    deleteMap(m);

    m = newMap();
    assert(addRoad(m, "A", "C", 10, 2020));
    assert(addRoad(m, "C", "B", 5, 2020));
    assert(addRoad(m, "A", "D", 12, 2020));
    assert(addRoad(m, "E", "D", 1, 2020));
    assert(addRoad(m, "E", "B", 2, 2020));
    assert(addRoad(m, "A", "F", 2, 2020));
    assert(addRoad(m, "G", "F", 2, 2020));
    assert(addRoad(m, "G", "H", 2, 2020));
    assert(addRoad(m, "B", "H", 9, 2020));
    assert(!newRoute(m, 42, "A", "B"));
    deleteMap(m);

    // Test przykładowy
    char const* str;
    m = newMap();
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

    return 0;
}


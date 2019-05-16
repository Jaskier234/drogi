#include "map.h"
#include "string_ext.h"
#include "valid.h"

#include <stdlib.h>
#include <stdio.h>

#undef NDEBUG

int main()
{
    Map *map = newMap();

    if(map == NULL)
        exit(0);

    Vector *command;
    while(true)
    {
        command = nextCommand();
        if(command == NULL)
            break;

        // TODO stworzyć stałe(jeszcze error?)
        if(strcmp(command->tab[0], ADD_ROAD) == 0)
        {
            if(!addRoad(map, command->tab[1], command->tab[2], stringToNum(command->tab[3]), stringToNum(command->tab[4])))
                fprintf(stderr, "ERROR %d\n", lineNr);
        }
        else if(strcmp(command->tab[0], REPAIR_ROAD) == 0)
        {
            if(!repairRoad(map, command->tab[1], command->tab[2], stringToNum(command->tab[3])))
                fprintf(stderr, "ERROR %d\n", lineNr);
        }
        else if(strcmp(command->tab[0], GET_ROUTE_DESRIPTION) == 0)
        {
            const char *description = getRouteDescription(map, stringToNum(command->tab[1]));
            if(description == NULL)
                fprintf(stderr, "ERROR %d\n", lineNr);
            else
                printf("%s\n", description);

            free((char*)description);
        }
        else
        {
            if(!addRoute(map, command))
                fprintf(stderr, "ERROR %d\n", lineNr);
        }
        free(command->tab[0]);
        deleteVector(command);
    }

    deleteMap(map);

    return 0;
}


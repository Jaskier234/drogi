#include "valid.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

const char *MAX_UINT64 = "18446744073709551615";
const char *MAX_UNSIGNED = "4294967295";
const char *MAX_SIGNED = "2147483647";
const char *MAX_ROUTE_ID = "999";

const char *ADD_ROAD = "addRoad";
const char *REPAIR_ROAD = "repairRoad";
const char *GET_ROUTE_DESRIPTION = "getRouteDescription";
const char *ERROR = "ERROR";

static char *readString(char **input)
{
    if(**input == '\0' || **input == '\n')
        return NULL;

    if(**input == ';')
    {
        **input = '\0';
        (*input)++;
    }

    char *begin = *input;

    while(**input != ';' && **input != '\n' && **input != '\0')
    {
        (*input)++;
    }

    return begin;
}

int correct(char *input, Vector *args)
{
    assert(args->filled == 0);
    // Ignored lines
    if(*input == '#' || *input == '\n')
    {
        return 2;
    }

    // If in line is a single characher diffrent than '#' or '\n' it, incorrect
    // line
    int inputSize = strlen(input);
    if(inputSize == 1)
    {
        return 0;
    }

    if(*input == ';')
        return 0;

    char *param;

    while(true)
    {
        param = readString(&input);
        if(param == NULL)
            break;
        vectorPushBack(args, param);
    }

    if(*input != '\n')
    {
        return 0;
    }
    *input = '\0';

    if(strcmp(args->tab[0], ADD_ROAD) == 0)
    {
        if(args->filled != 5)
            return 0;

        if(!isNameCorrect(args->tab[1]) || !isNameCorrect(args->tab[2]))
            return 0;

        if(!correctInt(args->tab[3], MAX_UNSIGNED, false))
            return 0;

        if(!correctInt(args->tab[4], MAX_SIGNED, true))
            return 0;

        if(strcmp(args->tab[3], "0") == 0 || strcmp(args->tab[4], "0") == 0)
            return 0;
    }
    else if(strcmp(args->tab[0], REPAIR_ROAD) == 0)
    {
        if(args->filled != 4)
            return 0;

        if(!isNameCorrect(args->tab[1]) || !isNameCorrect(args->tab[2]))
            return 0;

        if(!correctInt(args->tab[3], MAX_SIGNED, true))
            return 0;

        if(strcmp(args->tab[3], "0") == 0)
            return 0;
    }
    else if(strcmp(args->tab[0], GET_ROUTE_DESRIPTION) == 0)
    {
        if(args->filled != 2)
            return 0;

        if(!correctInt(args->tab[1], MAX_ROUTE_ID, false))
            return 0;
    }
    else if(correctInt(args->tab[0], MAX_ROUTE_ID, false))
    {
        int it=1;
        while(it+2 < args->filled)
        {
            if(!isNameCorrect(args->tab[it]))
                return 0;

            if(!correctInt(args->tab[it+1], MAX_UNSIGNED, false))
                return 0;

            if(!correctInt(args->tab[it+2], MAX_SIGNED, true))
                return 0;

            if(strcmp(args->tab[it+1], "0") == 0 || strcmp(args->tab[it+2], "0") == 0)
                return 0;

            it += 3;
        }

        if(args->filled == 2)
            return 0;

        if(args->filled != it+1)
            return 0;

        if(!isNameCorrect(args->tab[it])) {
            return 0;
        }
    }
    else
    {
        return 0;
    }

    return 1;
}

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

// Checks if string is convertable to integer
bool correctInt(char *number, const char *maxNumber, bool isSigned)
{
    if(isSigned)
    {
        if(*number == '-')
            number++;
    }

    // ignorujemy zera wiodące
    while(*number == '0')
        number++;

    if(strcmp(number,"") == 0)
        return false;

    unsigned int size = strlen(number);
    unsigned int maxNrSize = strlen(maxNumber);
    if(size > maxNrSize)
        return false;

    for(unsigned int i = 0; i < size; i++)
    {
        if(number[i] - '0' < 0 || number[i] - '0' > 9)
            return false;
    }

    if(size < maxNrSize)
        return true;

    for(unsigned int i = 0; i < size; i++)
    {
        if(number[i] > maxNumber[i])
            return false;
    }


    return true; // poprawić!!!;
}

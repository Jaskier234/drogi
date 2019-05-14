#define _GNU_SOURCE
#include "string_ext.h"
#include "vector.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "valid.h"

char *intToString(int a)
{
    char *string = calloc(20, sizeof(char));
    int size=0;

    if(a == 0)
    {
        *string = '0';
        return string;
    }

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

// Returns next correct command or NULL when there is no more commands
Vector *nextCommand()
{
    // Memory for getline
    size_t size = 16;
    char *input = malloc(size);

    // Pointer passed to correct witch contains array of command parameters
    // splitted into separate words
    Vector *splittedInput = newVector();
    int isCorrect = 0;
    int result;

//    if(input == NULL || splittedInput == NULL) // TODO sprawdzić czy się udało zaalokować, ale nie exit(1);
//        exit(1);

    do
    {
//        for(int i=0; i<5; i++) // TODO vectorClear
//            splittedInput[i] = NULL;
        result = getline(&input, &size, stdin);
        if(result != -1)
        {
            isCorrect = correct(input, splittedInput); // TODO przerobić correct tak by używało wektora
            if(isCorrect == 0)
                fprintf( stderr, "ERROR\n");
        }
    } while(result != -1 && isCorrect != 1);

    if(result == -1)
    {
        free(input);
        free(splittedInput);
        return NULL;
    }
    else
        return splittedInput;
}

// Converts string to uint64_t
uint64_t stringToNum(char *string)
{
    uint64_t number = 0;
    int size = strlen(string);
    for(int i=0; i<size; i++)
    {
        number *= 10;
        number += string[i] - '0';
    }
    return number;
}

#include "string_ext.h"

#include <stdbool.h>
#include <stdlib.h>

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

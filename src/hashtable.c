#include "hashtable.h"

#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "memory.h"

typedef struct Hashtable
{
    int MOD;
    List **table;
    int filled;
    Memory *memory;

} Hashtable;

typedef struct HashElem
{
    void *key, *value;
    int hash;
} HashElem;


HashElem *newPair(void *key, void *value, int hash, Memory *memory) // TODO refaktor po parach na hashelem
{
    HashElem *pair = getMemory(memory, sizeof(HashElem));
    pair->key = key;
    pair->value = value;
    pair->hash = hash;

    return pair;
}

Hashtable **newHashtable(int size, Memory *memory)
{
    Hashtable **hashtable = calloc(1, sizeof(Hashtable*));
    *hashtable = calloc(1, sizeof(Hashtable));
    (*hashtable)->MOD = size;//100003;//1024*1024;
    (*hashtable)->table = calloc((*hashtable)->MOD, sizeof(List*));
    if(memory == NULL)
        (*hashtable)->memory = newMemory();
    else
        (*hashtable)->memory = memory;

    return hashtable;
}

void deleteHashtableContent(Hashtable **hash)
{
//    for(int i=0; i<(*hash)->MOD; i++)
//    {
//        if((*hash)->table[i] != NULL)
//        {
//            Element *elem = (*hash)->table[i]->begin->next;
//            while(elem != (*hash)->table[i]->end)
//            {
//                free(((HashElem*)elem->value)->key);
//                free(((HashElem*)elem->value)->value);
//
//                elem = elem->next;
//            }
//        }
//    }
    deleteMemory((*hash)->memory);
    free((*hash)->table);
    free(*hash);
}

void deleteHashtable(Hashtable **hash)
{
    deleteHashtableContent(hash);
    free(hash);
}

bool changeSize(Hashtable **hash, int newSize)
{
    Hashtable **newHash = newHashtable(newSize, NULL);
    // TODO sprawdzić czy się udało

    for(int i=0; i<(*hash)->MOD; i++)
    {
        if((*hash)->table[i] != NULL)
        {
            Element *elem = (*hash)->table[i]->begin->next;
            while(elem->value != NULL)
            {
                int index = ((HashElem*)elem->value)->hash % (*newHash)->MOD;

                if((*newHash)->table[index] == NULL)
                    (*newHash)->table[index] = newList((*newHash)->memory);

                void *key = ((HashElem*)elem->value)->key;
                void *value = ((HashElem*)elem->value)->value;
                int hash = ((HashElem*)elem->value)->hash;

                listPushBack((*newHash)->table[index], newPair(key, value, hash, (*newHash)->memory), (*newHash)->memory);

                elem = elem->next;
            }
        }
    }
    (*newHash)->filled = (*hash)->filled;

    deleteHashtableContent(hash);

    *hash = *newHash;

    free(newHash);


    return true;
}

long long hash(char *string)
{
    const long long MOD = 1e9+7;

    long long hashValue = 0;
    long long exp = 1;
    while(*string != 0)
    {
        hashValue += (*string) * exp;
        hashValue %= MOD;
        exp *= 31;
        exp %= MOD;
        string++;
    }

    return hashValue;
}

bool hashtableInsert(Hashtable **self, char *key, void *value)
{
    int hashValue = hash(key);
    int index = hashValue % (*self)->MOD;

    if((*self)->table[index] == NULL)
        (*self)->table[index] = newList((*self)->memory);

    Element *elem = (*self)->table[index]->begin->next;

    while(elem->next != NULL && strcmp(key, ((HashElem *)elem->value)->key) != 0)
        elem = elem->next;

    if(elem->next == NULL)
        listPushBack((*self)->table[index], newPair(key, value, hashValue, (*self)->memory), (*self)->memory);
    else
        elem->value = newPair(key, value, hashValue, (*self)->memory);

    (*self)->filled++;
    if((*self)->filled * 2 > (*self)->MOD)
        changeSize(self, (*self)->MOD * 2 );

    return true;
}

void *hashtableGet(Hashtable **self, char *key)
{
    int hashValue = hash(key);
    int index = hashValue%(*self)->MOD;

    if((*self)->table[index] == NULL)
        return NULL;

    Element *elem = (*self)->table[index]->begin->next;

    while(elem->next != NULL && strcmp(key, ((HashElem *)elem->value)->key) != 0)
        elem = elem->next;

    if(elem->next != NULL)
        return ((HashElem *)elem->value)->value;// TODO naprawić kolizję nazw value
    else
        return NULL;
}

int max(int a, int b)
{
    if(a > b) return a;
    return b;
}

int maxListLength(Hashtable **hash)
{
    int maxSize=0;
    for(int i=0; i<(*hash)->MOD; i++)
    {
        maxSize = max(maxSize, listSize((*hash)->table[i]));
    }
    return maxSize;
}


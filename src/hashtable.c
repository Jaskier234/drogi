#include "hashtable.h"

#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "memory.h"

/**
 * Struktura tablicy haszującej
 */
typedef struct Hashtable
{
    int MOD; ///< Aktualna wielkość tablicy. Także wartość modulo
    ///< użyta podczas decydowania, w której liście umieścić element.
    List **table; ///< Tablica list przechowująca elementy tablicy haszującej
    int filled; ///< ilość elementów w tablicy.
    Memory *memory; ///< Wskaźnik na memory używane w tablicy.

} Hashtable;

/**
 * Struktura będąca elementem tablicy.
 */
typedef struct HashElem
{
    void *key; ///< Wskaźnik na napis będący kluczem.
    void *value; ///< Wskaźnik na przechowywaną wartość.
    int hash; ///< wartość funkcji haszującej dla elementu.
} HashElem;


static HashElem *newPair(void *key, void *value, int hash, Memory *memory)
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
    if(hashtable == NULL)
    {
        deleteHashtable(hashtable);
        return NULL;
    }

    *hashtable = calloc(1, sizeof(Hashtable));
    if(*hashtable == NULL)
    {
        deleteHashtable(hashtable);
        return NULL;
    }

    (*hashtable)->MOD = size;
    (*hashtable)->table = calloc((*hashtable)->MOD, sizeof(List*));

    if((*hashtable)->table == NULL)
    {
        deleteHashtable(hashtable);
        return NULL;
    }

    if(memory == NULL)
        (*hashtable)->memory = newMemory();
    else
        (*hashtable)->memory = memory;

    if((*hashtable)->memory == NULL)
    {
        deleteHashtable(hashtable);
        return NULL;
    }

    return hashtable;
}

/**
 * Zwalnia zawartość tablicy haszującej.
 * @param hash Wskaźnik na tablicę do zwonienia.
 */
static void deleteHashtableContent(Hashtable **hash)
{
    if(hash == NULL || *hash == NULL)
        return;

    deleteMemory((*hash)->memory);
    free((*hash)->table);
    free(*hash);
}

void deleteHashtable(Hashtable **hash)
{
    deleteHashtableContent(hash);
    free(hash);
}

/**
 * Zmienia rozmiar tablicy haszującej.
 * @param hash Wskaźnik na tablicę.
 * @param newSize Nowy rozmiar tablicy.
 * @return @p true jeśli udało sie zmienić rozmiar lub false gdy nie udało
 * się zaalokować pamięci.
 */
static bool changeSize(Hashtable **hash, int newSize)
{
    Hashtable **newHash = newHashtable(newSize, NULL);
    if(newHash == NULL)
        return false;

    for(int i = 0; i < (*hash)->MOD; i++)
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

/**
 * Funkcja haszująca. Liczy wartość modulo @p e9+7, która potem jest modulowana
 * ponownie modulo @p hash->mod
 * @param string Wskaźnik na napis, którego hasz ma zostać policzony.
 * @return Wartość funkcji haszującej.
 */
long long hash(const char *string)
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

    if(hashValue < 0)
        return hashValue + MOD;
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
        if(!changeSize(self, (*self)->MOD * 2 ))
            return false;

    return true;
}

void *hashtableGet(Hashtable **self, const char *key)
{
    int hashValue = hash(key);
    int index = hashValue%(*self)->MOD;

    if((*self)->table[index] == NULL)
        return NULL;

    Element *elem = (*self)->table[index]->begin->next;

    while(elem->next != NULL && strcmp(key, ((HashElem *)elem->value)->key) != 0)
        elem = elem->next;

    if(elem->next != NULL)
        return ((HashElem *)elem->value)->value;
    else
        return NULL;
}

static int max(int a, int b)
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


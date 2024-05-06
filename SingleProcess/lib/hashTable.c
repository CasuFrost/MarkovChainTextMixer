#include "../headers/hashTable.h"

unsigned int hash(char word[WORD_LENGHT])
{
    unsigned int hashVal;
    int c;

    for (int i = 0; i < WORD_LENGHT; i++)
    {
        hashVal = word[i] + 31 * hashVal;
    }
    return hashVal % HASHSIZE;
    // printf("%d\n", hashVal % HASHSIZE);
}
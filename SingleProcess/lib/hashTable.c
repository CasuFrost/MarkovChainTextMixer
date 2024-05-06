#include "../headers/hashTable.h"
#define HASHSIZE 100
unsigned int hash(char word[WORD_LENGHT])
{
    unsigned int hashVal;
    int c;

    for (int i = 0; i < WORD_LENGHT; i++)
    {
        hashVal = word[i] + 31 * hashVal;
    }
    printf("%d\n", hashVal % HASHSIZE);
}
#include "../headers/hashTable.h"

unsigned int hash(char word[WORD_LENGHT])
{

    // int p = 31;

    unsigned int hashVal = 0;

    for (int i = 0; i < strlen(word); i++)
    {
        hashVal = word[i] + 31 * hashVal;
        // p = p * 31;
    }
    return hashVal % HASHSIZE;
    // printf("%d\n", hashVal % HASHSIZE);
}

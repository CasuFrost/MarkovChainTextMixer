#include "../headers/hashTable.h"

unsigned int hash(char word[WORD_LENGHT]) /*Una semplice funzione hash che associa ad ogni stringa un intero positivo fra 0 e HASHSIZE*/
{
    unsigned int hashVal = 0;

    for (int i = 0; i < strlen(word); i++)
    {
        hashVal = word[i] + 31 * hashVal;
    }
    return hashVal % HASHSIZE;
}

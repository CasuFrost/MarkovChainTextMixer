#include "../headers/hashTable.h"

unsigned int hash(char word[WORD_LENGHT])
{

    unsigned int hashVal = 0;

    for (int i = 0; i < strlen(word); i++)
    {
        hashVal = word[i] + 31 * hashVal;
    }
    return hashVal % HASHSIZE;
    // printf("%d\n", hashVal % HASHSIZE);
}

unsigned int FNV(char word[WORD_LENGHT], unsigned int h)
{
    // See: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp
    h ^= 2166136261UL;

    for (int i = 0; i < WORD_LENGHT; i++)
    {
        h ^= word[i];
        h *= 16777619;
    }
    return h % HASHSIZE;
}

unsigned int MurmurOAAT_32(char word[WORD_LENGHT])
{
    int h = 587;
    // One-byte-at-a-time hash based on Murmur's mix
    // Source: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp
    for (int i = 0; i < WORD_LENGHT; i++)
    {
        h ^= word[i];
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
    return h % HASHSIZE;
}
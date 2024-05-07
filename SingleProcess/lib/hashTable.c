#include "../headers/hashTable.h"

unsigned int haash(char word[WORD_LENGHT])
{

    int p = 31;

    unsigned int hashVal = 0;

    for (int i = 0; i < strlen(word); i++)
    {
        hashVal = word[i] + p * hashVal;
        p = p * 31;
    }
    return hashVal % HASHSIZE;
    // printf("%d\n", hashVal % HASHSIZE);
}

int p_power[31] = {31};

void calcPpower()
{
    for (int i = 1; i < 31; i++)
    {
        p_power[i] = 31 * p_power[i - 1];
    }
}

long long
hash(char word[WORD_LENGHT])
{
    const int p = 31;
    const int m = 1e9 + 9;
    long long hash_value = 0;
    long long p_pow = 1;
    for (int i = 0; i < strlen(word); i++)
    {
        // printf("%d\n", p_power[i]);
        hash_value = (hash_value + (word[i] - 'a' + 1) * p_power[i]) % m;
        // p_pow = (p_pow * p) % m;
    }
    return hash_value % HASHSIZE;
}
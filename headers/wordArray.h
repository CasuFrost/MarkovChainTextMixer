/* Questo file si occupa di gestire un array di parole di lunghezza massima 30 byte.
Il termine array, è in realtà improprio, in quanto tale struttura sarà
un insieme.*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define WORD_LENGHT 32 /*30 byte per la parola, 1 byte per un eventuale apostrofo, il byte finale per '\0' */

// #include "structures.h"

char *words;
int wordsLenght = 0;
int wordSize = 0;

void wordsInit()
{ // Inizializza la struttura che conterrà le parole del file
    words = malloc(0);
}

void addWord(char *s)
{

    wordsLenght++;
    wordSize = wordsLenght * WORD_LENGHT;

    int offset = (wordsLenght - 1) * WORD_LENGHT;
    words = realloc(words, wordsLenght * WORD_LENGHT);

    for (int i = 0; i < strlen(s); i++)
    {
        words[offset + i] = s[i];
    }

    // words[offset + strlen(s)] = '\0';
}

void printWords()
{
    for (int i = 0; i < wordSize; i++)
    {
        printf("%c", words[i]);
    }
}

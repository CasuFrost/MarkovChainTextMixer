#include <string.h>
#include <stdio.h>

#include "wordArray.h"
/*Questo file contiene alcune funzioni sul controllo e la modifica delle stringhe o dei caratteri*/

void minuscolaStringa(char *s)
{
    for (int i = 0; i < WORD_LENGHT; i++)
    {
        if (s[i] > 64 && s[i] < 91)
        { // c è MAIUSCOLO
            s[i] += 32;
        }
    }
}

void prettyPrintWords(char **array_parole, int size)
{
    printf("-----\n");
    for (int i = 0; i < size; i++)
    {
        printf("%s\n----------------------------\n", array_parole[i]);
    }
}

int checkIfWordInArray(char **array_parole, int size, char word[WORD_LENGHT]) /*ritorna 1 se la parola 'word' si trova
 nell'array 'array_parole'*/
{
    for (int i = 0; i < size; i++)
    {
        if (strcmp(array_parole[i], word) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int getWordArrayId(char **array_parole, int size, char word[WORD_LENGHT]) /*ritorna l'id della parola nell'array*/
{

    for (int i = 0; i < size; i++)
    {
        if (strcmp(array_parole[i], word) == 0)
        {
            return i;
        }
    }
    return -1;
}

void addWordToWordArray(char **array_parole, int *wordsCounter, char *tmp)
{
    // printf("\n%d", array_parole);
    *wordsCounter = *wordsCounter + 1;
    array_parole = realloc(array_parole, *wordsCounter * sizeof(char *)); // Aggiungo una stringa all'array di stringhe
    array_parole[*wordsCounter - 1] = malloc(WORD_LENGHT);                // creo la stringa

    strcpy(array_parole[*wordsCounter - 1], tmp); // Gli assegno il valore della parola letta
}
#include "../headers/rootHeader.h"
#include "../headers/stringOpertion.h"

/*Questo file contiene alcune funzioni sul controllo e la modifica delle stringhe o dei caratteri*/

void lowerCase(char *s)
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
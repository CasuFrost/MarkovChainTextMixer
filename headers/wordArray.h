/* Questo file si occupa di gestire un array di parole di lunghezza massima 30 byte.
Il termine array, è in realtà improprio, in quanto tale struttura sarà
un insieme.*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ioOperation.h"

#define WORD_LENGHT 32 /*30 byte per la parola, 1 byte per un eventuale apostrofo, il byte finale per '\0' */

/*La struttura delle frequenze seguirà il seguente schema :
Ogni parola sarà identificata da una struttura dati 'Nodo Parola', essa conterrà una stringa, relativa alla parola
stessa, e un insieme di puntatori ad una struttura 'Nodo frequenza'.

Il 'Nodo frequenza' non è altro che una struttura contenente una stringa ed un carattere float.

*/

int *matrix;
int n;

void printMatrix(int wordsCounter)
{
    for (int i = 0; i < wordsCounter; i++)
    {
        for (int j = 0; j < wordsCounter; j++)
        {
            matrix[i * wordsCounter + j];
            printf("| %d |", matrix[i * wordsCounter + j]);
        }
        printf("\n");
    }
}

void initMatrix(int words)
{
    n = words;
    matrix = (int *)malloc(n * n * sizeof(int));
    matrix[1] = 98;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matrix[i * words + j] = 0;
            // printf("| %d |", matrix[i * words + j]);
        }
        // printf("\n");
    }
    // printf("\n\n");
}

int getMatrixValue(int i, int j)
{
    return matrix[i * n + j];
}

/* matrix[i,j]=x implica che
array_parole[j] si trova dopo array_parole[i] con una frequenza x
*/

/*
struct nodoFrequenza
{
    char parola[WORD_LENGHT];
    float frequenza;
};

struct nodoParola
{
    char parola[WORD_LENGHT];
    struct nodoFrequenza *successivi;
    int numSuccessivi; // Quante parole sono successive a questa
};

struct nodoParola *parole;
int numParole = 0;

void init()
{
    parole = malloc(0);
}

void addParola(struct nodoParola p)
{
    numParole++;
    parole = realloc(parole, numParole * sizeof(struct nodoParola));
    parole[numParole - 1] = p;
}*/

int punteggiaturaDaScartare(char c)
{
    if (((int)c >= 34 && (int)c <= 38) || ((int)c >= 40 && (int)c <= 45) ||
        ((int)c >= 58 && (int)c <= 62) || ((int)c >= 91 && (int)c <= 96) ||
        ((int)c >= 123 && (int)c <= 126))
    {
        return 1;
    }
    return 0;
}

void fillMatrixWithWord(char *fileName, char **array_parole, int wordsCounter)
{
    int fileSize;
    char *src = putFileInBuffer(fileName, &fileSize);
    int j = 0;
    char currentWord[WORD_LENGHT];
    char previousWord[WORD_LENGHT] = ".\0";

    int currentId;
    int previousId;

    for (int i = 0; i < fileSize; i++)
    {
        if (punteggiaturaDaScartare(src[i]))
        {
            // C'è della punteggiatura da scartare
            if (j != 0)
            {
                // PAROLA LETTA &&&&&&&&&&&&&&&&&&&&&&
                currentWord[j] = '\0'; /* A questo punto 'tmp' contiene la parola letta, quindi aggiungo il simbolo di fine stringa*/
                minuscolaStringa(currentWord);
                currentId = getWordArrayId(array_parole, wordsCounter, currentWord);
                previousId = getWordArrayId(array_parole, wordsCounter, previousWord);
                matrix[currentId * n + previousId]++;
                strcpy(previousWord, currentWord);
                j = 0;
            }
            continue;
        }
        if ((int)src[i] == 39) // C'è un apostrofo
        {
            if (j != 0)
            {
                // PAROLA LETTA &&&&&&&&&&&&&&&&&&&&&&
                currentWord[j] = src[i];
                currentWord[j + 1] = '\0';
                minuscolaStringa(currentWord);

                currentId = getWordArrayId(array_parole, wordsCounter, currentWord);
                previousId = getWordArrayId(array_parole, wordsCounter, previousWord);
                matrix[currentId * n + previousId]++;
                strcpy(previousWord, currentWord);

                j = 0;
            }
            continue;
        }
        if (src[i] == '!' || src[i] == '.' || src[i] == '?') // Gestione dei punti
        {
            if (j == 0)
            {
                // PAROLA LETTA &&&&&&&&&&&&&&&&&&&&&&
                /*trovato punto da solo*/
                currentWord[0] = src[i];
                currentWord[1] = '\0'; /* A questo punto 'tmp' contiene la parola letta, quindi aggiungo il simbolo di fine stringa*/

                currentId = getWordArrayId(array_parole, wordsCounter, currentWord);
                previousId = getWordArrayId(array_parole, wordsCounter, previousWord);
                matrix[currentId * n + previousId]++;
                strcpy(previousWord, currentWord);

                continue;
            }
            else
            {
                // PAROLA LETTA &&&&&&&&&&&&&&&&&&&&&&
                //  La parola attuale va salvata nel buffer
                currentWord[j] = '\0'; /* A questo punto 'tmp' contiene la parola letta, quindi aggiungo il simbolo di fine stringa*/
                minuscolaStringa(currentWord);

                currentId = getWordArrayId(array_parole, wordsCounter, currentWord);
                previousId = getWordArrayId(array_parole, wordsCounter, previousWord);
                matrix[currentId * n + previousId]++;
                strcpy(previousWord, currentWord);

                j = 0;

                // Ora salvo il punto
                currentWord[0] = src[i];
                currentWord[1] = '\0';

                currentId = getWordArrayId(array_parole, wordsCounter, currentWord);
                previousId = getWordArrayId(array_parole, wordsCounter, previousWord);
                matrix[currentId * n + previousId]++;
                strcpy(previousWord, currentWord);

                continue;
            }
        }
        // printf("%d ", (int)src[i]);
        if ((int)src[i] == 10 || (int)src[i] == 32 || (int)src[i] == 9) // Viene letto uno spazio o un accapo
        {
            if (j != 0) /*è stato letto uno spazio e c'è una parola nel buffer, va salvata la parola e svuotato il buffer*/
            {

                // parola finita
                currentWord[j] = '\0'; /* A questo punto 'tmp' contiene la parola letta, quindi aggiungo il simbolo di fine stringa*/
                minuscolaStringa(currentWord);

                currentId = getWordArrayId(array_parole, wordsCounter, currentWord);
                previousId = getWordArrayId(array_parole, wordsCounter, previousWord);
                matrix[currentId * n + previousId]++;

                strcpy(previousWord, currentWord);
                j = 0;
            }
        }
        else /* La parola continua, metto il carattere nel buffer 'tmp'*/
        {
            currentWord[j] = src[i];
            j++;
        }
    } // Fine del ciclo 'for'

    if (j != 0) /*Se l'ultima lettera è un carattere, la parola nel buffer non è stata ancora aggiunta all'array*/
    {
        currentWord[j] = '\0'; /* A questo punto 'tmp' contiene la parola letta, quindi aggiungo il simbolo di fine stringa*/
        minuscolaStringa(currentWord);

        currentId = getWordArrayId(array_parole, wordsCounter, currentWord);
        previousId = getWordArrayId(array_parole, wordsCounter, previousWord);
        matrix[currentId * n + previousId]++;
    }
    if (strcmp(currentWord, ".") != 0)
    {
        /*Se l'ultima parola non è il punto, bisogna collegare la prima e l'ultima parola per evitare situazioni
        in cui il programma non sa quale altra parola usare*/

        matrix[0 * n + wordsCounter - 1]++; // Prima del punto, c'è l'ultima parola
    }
}

void printFrequence(char **array_parole, char *fileName) /*Questa funzione legge la matrice e stampa sul file*/
{
    FILE *fp;

    fp = fopen(fileName, "w+");

    for (int i = 0; i < n; i++) /*Scorro ogni riga della matrice*/
    {
        // Faccio la somma della riga attuale
        int rawSum = 0;
        for (int j = 0; j < n; j++) /*Scorro ogni riga della matrice*/
        {
            rawSum += matrix[i * n + j];
        }
        fprintf(fp, "%s", array_parole[i]);
        for (int j = 0; j < n; j++) /*Scorro ogni riga della matrice*/
        {
            if (matrix[i * n + j] != 0)
            {
                float f = (float)matrix[i * n + j] / (float)rawSum;
                fprintf(fp, ",%s,%f", array_parole[j], f);
            }
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}
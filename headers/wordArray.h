/* Questo file si occupa di gestire un array di parole di lunghezza massima 30 byte.
Il termine array, è in realtà improprio, in quanto tale struttura sarà
un insieme.*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ioOperation.h"

#define WORD_LENGHT 32 /*30 byte per la parola, 1 byte per un eventuale apostrofo, il byte finale per '\0' */

void addWord(char ***array_parole, int *counter, char word[WORD_LENGHT]) /* Questa funzione prende in input un array di parole (con la sua
dimensione) ed una parola, ed aggiunge la parola nell'array (esclusivamente se non vi è già presente), questo array simula quindi un SET*/
{

    int k = *counter;
    minuscolaStringa(word);
    if (!checkIfWordInArray(*array_parole, k, word)) /*Controlla se la parola non è già nell'array*/
    {
        // printf("parola[%d]=%s\n", *counter, word);
        k++;
        *counter = k;

        *array_parole = realloc(*array_parole, k * sizeof(char *));
        (*array_parole)[k - 1] = malloc(WORD_LENGHT);
        strcpy((*array_parole)[k - 1], word);
    }
}

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

/* matrix[i,j]=x implica che
array_parole[j] si trova dopo array_parole[i] con una frequenza x
*/

int punteggiaturaDaScartare(char c)
{
    if (((int)c >= 34 && (int)c <= 38) || ((int)c >= 40 && (int)c <= 45) ||
        ((int)c >= 58 && (int)c <= 62) || ((int)c >= 91 && (int)c <= 96) ||
        ((int)c >= 123 && (int)c <= 126) || c == '/')
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
                matrix[previousId * n + currentId]++;
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
                matrix[previousId * n + currentId]++;
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
                matrix[previousId * n + currentId]++;
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
                matrix[previousId * n + currentId]++;
                strcpy(previousWord, currentWord);

                j = 0;

                // Ora salvo il punto
                currentWord[0] = src[i];
                currentWord[1] = '\0';

                currentId = getWordArrayId(array_parole, wordsCounter, currentWord);
                previousId = getWordArrayId(array_parole, wordsCounter, previousWord);
                matrix[previousId * n + currentId]++;
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
                matrix[previousId * n + currentId]++;

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
        matrix[previousId * n + currentId]++;
    }
    if (strcmp(currentWord, ".") != 0 && wordsCounter >= 2)
    {
        /*Se l'ultima parola non è il punto, bisogna collegare la prima e l'ultima parola per evitare situazioni
        in cui il programma non sa quale altra parola usare*/

        matrix[(wordsCounter - 1) * n + 1]++; // Prima del punto, c'è l'ultima parola
    }
}

void printFrequence(char **array_parole, char *fileName) /*Questa funzione legge la matrice e stampa sul file*/
{
    FILE *fp;

    fp = fopen(fileName, "w+"); // Creo il puntatore al file

    for (int i = 0; i < n; i++) /*Scorro ogni riga della matrice*/
    {
        // Faccio la somma della riga attuale
        int rawSum = 0;             /* Questa variabile salverà per ogni riga la somma delle occorrenze delle parole a seguito di quella controllata correntemente*/
        for (int j = 0; j < n; j++) /*Scorro ogni riga della matrice*/
        {
            rawSum += matrix[i * n + j];
        }

        fprintf(fp, "%s", array_parole[i]); // Scrivo la parola corrente

        for (int j = 0; j < n; j++) /*Scorro le parole che la seguono*/
        {
            if (matrix[i * n + j] != 0) /*Se le occorrenze sono diverse da zero, devo scrivere la frequenza*/
            {
                float f = (float)matrix[i * n + j] / (float)rawSum;
                fprintf(fp, ",%s,%.4f", array_parole[j], f);
            }
        }
        if (i != n - 1)
        {

            fprintf(fp, "\n"); // Vado a capo nel file CSV
        }
    }

    fclose(fp);
}
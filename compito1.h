#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

// Headers personali
#include "compito2.h"

char **getWordFromFile(char *fileName, int *numberOfWords) /* Questa funzione prende come input il nome di un file, e restituisce
un array di stringhe contenete le parole lette nel file.  prende come input anche un intero, che verrà
aggiornato e conterrà il numero di parole*/
{
    int wordsCounter = 0;
    int fileSize;
    char tmp[WORD_LENGHT];

    FILE *fp = fopen(fileName, "r");

    if (fp == NULL)
    {
        printf("file %s non esistente\n", fileName);
        exit(1);
    }

    int j = 0; /* Sarà l'index relativo della parola letta, se 0, vuol dire che si sta attendendo di leggere la prima lettera di una parola*/

    char **array_parole = malloc(0); // Questo array conterrà tutte le parole lette

    /*Come prima parola, aggiungo il punto*/
    char punto[WORD_LENGHT] = ".";

    addWord(&array_parole, &wordsCounter, punto); // Aggiungo la parola all'array
    char c;
    int i = 0;

    while ((c = fgetc(fp)) != EOF)
    {
        if ((int)c > 128 || (int)c < 0)
        {
        }
        if (j > WORD_LENGHT - 1)
        {
            printf("Il testo contiene una parola più lunga di 30 caratteri!\n");
            exit(1);
        }

        if (punteggiaturaDaScartare(c))
        {
            // C'è della punteggiatura da scartare
            if (j != 0)
            {
                tmp[j] = '\0'; /* A questo punto 'tmp' contiene la parola letta, quindi aggiungo il simbolo di fine stringa*/

                addWord(&array_parole, &wordsCounter, tmp); // Aggiungo la parola all'array

                j = 0;
            }
            continue;
        }

        if ((int)c == 39) // C'è un apostrofo
        {
            if (j != 0)
            {
                tmp[j] = c;
                tmp[j + 1] = '\0';

                addWord(&array_parole, &wordsCounter, tmp); // Aggiungo la parola all'array
                j = 0;
            }
            continue;
        }

        if (c == '!' || c == '.' || c == '?') // Gestione dei punti
        {
            if (j == 0)
            {
                /*trovato punto da solo*/
                tmp[0] = c;
                tmp[1] = '\0'; /* A questo punto 'tmp' contiene la parola letta, quindi aggiungo il simbolo di fine stringa*/

                addWord(&array_parole, &wordsCounter, tmp); // Aggiungo la parola all'array
                continue;
            }
            else
            {
                // La parola attuale va salvata nel buffer
                tmp[j] = '\0'; /* A questo punto 'tmp' contiene la parola letta, quindi aggiungo il simbolo di fine stringa*/

                addWord(&array_parole, &wordsCounter, tmp); // Aggiungo la parola all'array

                j = 0;

                // Ora salvo il punto
                tmp[0] = c;
                tmp[1] = '\0';
                addWord(&array_parole, &wordsCounter, tmp); // Aggiungo la parola all'array
                continue;
            }
        }

        if ((int)c >= 0 && (int)c <= 32) // Viene letto uno spazio o un accapo
        {
            if (j != 0) /*è stato letto uno spazio e c'è una parola nel buffer, va salvata la parola e svuotato il buffer*/
            {
                // parola finita
                tmp[j] = '\0'; /* A questo punto 'tmp' contiene la parola letta, quindi aggiungo il simbolo di fine stringa*/

                addWord(&array_parole, &wordsCounter, tmp); // Aggiungo la parola all'array

                j = 0;
            }
        }
        else /* La parola continua, metto il carattere nel buffer 'tmp'*/
        {
            tmp[j] = c;
            j++;
        }
    }

    fclose(fp);
    // FINE DEL CICLO FOR

    if (j != 0) /*Se l'ultima lettera è un carattere, la parola nel buffer non è stata ancora aggiunta all'array*/
    {
        tmp[j] = '\0'; /* A questo punto 'tmp' contiene la parola letta, quindi aggiungo il simbolo di fine stringa*/

        addWord(&array_parole, &wordsCounter, tmp); // Aggiungo la parola all'array
    }

    *numberOfWords = wordsCounter;

    return array_parole;
}

int compito1(char *input, char *output)
{

    int wordsCounter = 0;                                        /* Questa variabile conterrà il numero di parole del file */
    char **array_parole = getWordFromFile(input, &wordsCounter); /* Conta le parole del file */
    // printf("%d", wordsCounter);
    FILE *fp;
    fp = fopen("text.txt", "w+");

    // printf("%d", wordsCounter);
    initMatrix(wordsCounter);                              // Inizializzo la matrice con numero di righe e colonne identico al numero delle parole distinte lette nel file
    fillMatrixWithWord(input, array_parole, wordsCounter); // Riempio i campi della matrice

    printFrequence(array_parole, output); // Scrivo il file

    // Libero la memoria
    free(matrix);
    free(array_parole);
    printf("programma andato a buon fine!\n");
    exit(0);
}
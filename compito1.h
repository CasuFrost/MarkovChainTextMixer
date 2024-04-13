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
    char *src = putFileInBuffer(fileName, &fileSize);
    int j = 0; /* Sarà l'index relativo della parola letta, se 0, vuol dire che si sta attendendo di leggere la prima lettera di una parola*/

    char **array_parole = malloc(0); // Questo array conterrà tutte le parole lette

    /*Come prima parola, aggiungo il punto*/
    char punto[WORD_LENGHT] = ".";
    wordsCounter++;                                                      /* Aumento una parola*/
    array_parole = realloc(array_parole, wordsCounter * sizeof(char *)); // Aggiungo una stringa all'array di stringhe

    array_parole[wordsCounter - 1] = malloc(WORD_LENGHT); // creo la stringa

    strcpy(array_parole[wordsCounter - 1], punto); // Gli assegno il valore della parola letta

    for (int i = 0; i < fileSize; i++)
    {
        if (punteggiaturaDaScartare(src[i]))
        {
            // C'è della punteggiatura da scartare
            if (j != 0)
            {
                tmp[j] = '\0'; /* A questo punto 'tmp' contiene la parola letta, quindi aggiungo il simbolo di fine stringa*/
                minuscolaStringa(tmp);

                if (!checkIfWordInArray(array_parole, wordsCounter, tmp)) /* Voglio aggiungere la parola esclusivamente se nuova */
                {
                    wordsCounter++;                                                      // Aumento una parola
                    array_parole = realloc(array_parole, wordsCounter * sizeof(char *)); // Aggiungo una stringa all'array di stringhe

                    array_parole[wordsCounter - 1] = malloc(WORD_LENGHT); // creo la stringa

                    strcpy(array_parole[wordsCounter - 1], tmp); // Gli assegno il valore della parola letta
                }

                j = 0;
            }
            continue;
        }
        if ((int)src[i] == 39) // C'è un apostrofo
        {
            if (j != 0)
            {
                tmp[j] = src[i];
                tmp[j + 1] = '\0';
                minuscolaStringa(tmp);
                if (!checkIfWordInArray(array_parole, wordsCounter, tmp)) /* Voglio aggiungere la parola esclusivamente se nuova */
                {

                    wordsCounter++;                                                      // Aumento una parola
                    array_parole = realloc(array_parole, wordsCounter * sizeof(char *)); // Aggiungo una stringa all'array di stringhe

                    array_parole[wordsCounter - 1] = malloc(WORD_LENGHT); // creo la stringa
                    strcpy(array_parole[wordsCounter - 1], tmp);          // Gli assegno il valore della parola letta
                }
                j = 0;
            }
            continue;
        }
        if (src[i] == '!' || src[i] == '.' || src[i] == '?') // Gestione dei punti
        {
            if (j == 0)
            {
                /*trovato punto da solo*/
                tmp[0] = src[i];
                tmp[1] = '\0'; /* A questo punto 'tmp' contiene la parola letta, quindi aggiungo il simbolo di fine stringa*/

                if (!checkIfWordInArray(array_parole, wordsCounter, tmp)) /* Voglio aggiungere la parola esclusivamente se nuova */
                {

                    wordsCounter++;                                                      // Aumento una parola
                    array_parole = realloc(array_parole, wordsCounter * sizeof(char *)); // Aggiungo una stringa all'array di stringhe

                    array_parole[wordsCounter - 1] = malloc(WORD_LENGHT); // creo la stringa
                    strcpy(array_parole[wordsCounter - 1], tmp);          // Gli assegno il valore della parola letta
                }
                continue;
            }
            else
            {
                // La parola attuale va salvata nel buffer
                tmp[j] = '\0'; /* A questo punto 'tmp' contiene la parola letta, quindi aggiungo il simbolo di fine stringa*/
                minuscolaStringa(tmp);

                if (!checkIfWordInArray(array_parole, wordsCounter, tmp)) /* Voglio aggiungere la parola esclusivamente se nuova */
                {

                    wordsCounter++;                                                      // Aumento una parola
                    array_parole = realloc(array_parole, wordsCounter * sizeof(char *)); // Aggiungo una stringa all'array di stringhe

                    array_parole[wordsCounter - 1] = malloc(WORD_LENGHT); // creo la stringa
                    strcpy(array_parole[wordsCounter - 1], tmp);          // Gli assegno il valore della parola letta
                }

                j = 0;

                // Ora salvo il punto
                tmp[0] = src[i];
                tmp[1] = '\0';
                if (!checkIfWordInArray(array_parole, wordsCounter, tmp)) /* Voglio aggiungere la parola esclusivamente se nuova */
                {

                    wordsCounter++;                                                      // Aumento una parola
                    array_parole = realloc(array_parole, wordsCounter * sizeof(char *)); // Aggiungo una stringa all'array di stringhe

                    array_parole[wordsCounter - 1] = malloc(WORD_LENGHT); // creo la stringa
                    strcpy(array_parole[wordsCounter - 1], tmp);          // Gli assegno il valore della parola letta
                }
                continue;
            }
        }
        // printf("%d ", (int)src[i]);
        if ((int)src[i] == 10 || (int)src[i] == 32 || (int)src[i] == 9) // Viene letto uno spazio o un accapo
        {
            if (j != 0) /*è stato letto uno spazio e c'è una parola nel buffer, va salvata la parola e svuotato il buffer*/
            {
                // parola finita
                tmp[j] = '\0'; /* A questo punto 'tmp' contiene la parola letta, quindi aggiungo il simbolo di fine stringa*/
                minuscolaStringa(tmp);

                if (!checkIfWordInArray(array_parole, wordsCounter, tmp)) /* Voglio aggiungere la parola esclusivamente se nuova */
                {

                    wordsCounter++;                                                      // Aumento una parola
                    array_parole = realloc(array_parole, wordsCounter * sizeof(char *)); // Aggiungo una stringa all'array di stringhe

                    array_parole[wordsCounter - 1] = malloc(WORD_LENGHT); // creo la stringa

                    strcpy(array_parole[wordsCounter - 1], tmp); // Gli assegno il valore della parola letta
                }

                j = 0;
            }
        }
        else /* La parola continua, metto il carattere nel buffer 'tmp'*/
        {
            tmp[j] = src[i];
            j++;
        }
    } // Fine del ciclo 'for'

    if (j != 0) /*Se l'ultima lettera è un carattere, la parola nel buffer non è stata ancora aggiunta all'array*/
    {
        tmp[j] = '\0'; /* A questo punto 'tmp' contiene la parola letta, quindi aggiungo il simbolo di fine stringa*/
        minuscolaStringa(tmp);
        if (!checkIfWordInArray(array_parole, wordsCounter, tmp))
        {

            wordsCounter++;                                                      /* Aumento una parola*/
            array_parole = realloc(array_parole, wordsCounter * sizeof(char *)); // Aggiungo una stringa all'array di stringhe

            array_parole[wordsCounter - 1] = malloc(WORD_LENGHT); // creo la stringa

            strcpy(array_parole[wordsCounter - 1], tmp); // Gli assegno il valore della parola letta
        }
    }

    *numberOfWords = wordsCounter;

    return array_parole;
}

int compito1(char *input, char *output)
{

    int wordsCounter = 0; /* Questa variabile conterrà il numero di parole del file */

    char **array_parole = getWordFromFile(input, &wordsCounter); /* Conta le parole del file */

    initMatrix(wordsCounter);
    fillMatrixWithWord(input, array_parole, wordsCounter);

    printFrequence(array_parole, output);
    free(array_parole);
    exit(0);
}
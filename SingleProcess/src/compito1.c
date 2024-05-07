// Headers personali
#include "../headers/stringOpertion.h" /* contiene funzioni riguardo il controllo delle stringhe*/
#include "../headers/compito1.h"
#include "../headers/wordArray.h"

char **getWordFromFile(char *fileName, int *numberOfWords) /* Questa funzione prende come input il nome di un file, e restituisce
un array di stringhe contenete le parole lette nel file.  prende come input anche un intero, che verrà
aggiornato e conterrà il numero di parole*/
{
    calcPpower();
    int wordsCounter = 0;
    int fileSize;
    char tmp[WORD_LENGHT];

    char *src = putFileInBuffer(fileName, &fileSize);

    int j = 0; /* Sarà l'index relativo della parola letta, se 0, vuol dire che si sta attendendo di leggere la prima lettera di una parola*/

    char **array_parole = malloc(0); // Questo array conterrà tutte le parole lette

    /*Come prima parola, aggiungo il punto*/
    char punto[WORD_LENGHT] = ".";

    addWord(&array_parole, &wordsCounter, punto); // Aggiungo la parola all'array
    char c;

    for (int i = 0; i < fileSize; i++)
    {

        if (j > WORD_LENGHT - 1)
        {
            printf("Il testo contiene una parola più lunga di 30 caratteri, oppure contiene una parola, in cui \nè presente un carattere non ASCII standard, tale parola è stata tagliata!\n");
            j = 0;
            continue;
        }

        if (punteggiaturaDaScartare(src[i]))
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

        if ((int)src[i] == 39) // C'è un apostrofo
        {
            if (j != 0)
            {
                tmp[j] = src[i];
                tmp[j + 1] = '\0';

                addWord(&array_parole, &wordsCounter, tmp); // Aggiungo la parola all'array
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
                tmp[0] = src[i];
                tmp[1] = '\0';
                addWord(&array_parole, &wordsCounter, tmp); // Aggiungo la parola all'array
                continue;
            }
        }
        if (((int)src[i] >= 0 && (int)src[i] <= 32)) // Viene letto uno spazio o un accapo
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
            tmp[j] = src[i];
            j++;
        }
    }

    //  FINE DEL CICLO FOR

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

    clock_t start, end;
    start = clock();
    double cpu_time_used;

    int wordsCounter = 0; /* Questa variabile conterrà il numero di parole del file */

    char **array_parole = getWordFromFile(input, &wordsCounter); /* Conta e salva le parole del file */

    initMatrix(wordsCounter); // Inizializzo la matrice con numero di righe e colonne identico al numero delle parole distinte lette nel file

    fillMatrixWithWord(input, array_parole, wordsCounter); // Riempio i campi della matrice

    printFrequence(array_parole, output); // Scrivo il file

    printf("parole distinte : %d\n", wordsCounter); /* Per scrivere a schermo statistiche relative all'HashTable */

    // Libero la memoria
    freeHashMap();
    freeMatrix();
    free(array_parole);

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Programma andato a buon fine in %.2f secondi!\n", cpu_time_used);

    exit(0);
}
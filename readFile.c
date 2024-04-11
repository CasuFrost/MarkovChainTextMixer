#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

// Headers personali
#include "headers/wordArray.h"

void prettyPrintWords(char **array_parole, int size)
{
    printf("-----\n");
    for (int i = 0; i < size; i++)
    {
        printf("| %s |\n", array_parole[i]);
        printf("-----\n");
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

char *putFileInBuffer(char *fileName, int *size)
{ /*Questa funzione prende come input il path di un file, e restituisce un puntatore di caratteri contenente
il file letto, inoltre, prende come input anche un intero, che verrà aggiornato e conterrà le dimensioni
in byte del file.*/
    int fdIn = open(fileName, O_RDONLY);

    if (fdIn == -1)
    { // Viene aperto il file input in sola lettura
        printf("errore nell'apertura di %s\n", fileName);
        exit(1);
    }

    struct stat sbuf; // Definisco la struttura in cui saranno presenti le informazioni del file aperto.

    if (fstat(fdIn, &sbuf) == -1)
    { // Leggo le informazioni del file
        printf("errore nella lettura delle informazioni del file %s\n", fileName);
        exit(1);
    }

    *size = sbuf.st_size; // Le dimensioni del file in input
    if (sbuf.st_size == 0)
    {
        char *src = malloc(0);
        return src;
    }
    char *src = mmap(0, sbuf.st_size, PROT_READ, MAP_SHARED, fdIn, 0);

    if (src == MAP_FAILED)
    { // Eseguo il mapping del file nel buffer 'src'
        printf("errore nel mapping del file \n");
        exit(1);
    }
    close(fdIn);
    /* A questo punto del codice, all'interno del buffer 'src' è presente il file */
    return src;
}

char **getWordFromFile(char *fileName, int *numberOfWords) /* Questa funzione prende come input il nome di un file, e restituisce
un array di stringhe contenete le parole lette nel file.  prende come input anche un intero, che verrà
aggiornato e conterrà il numero di parole*/
{
    int wordsCounter = 0;
    int fileSize;
    char tmp[WORD_LENGHT];
    char *src = putFileInBuffer(fileName, &fileSize);
    int j = 0; // relative index

    char **array_parole = malloc(0); // Questo array conterrà tutte le parole lette

    for (int i = 0; i < fileSize; i++)
    {
        // printf("%d ", (int)src[i]);
        if ((int)src[i] == 10 || (int)src[i] == 32) // Viene letto uno spazio o un accapo
        {
            if (j == 0)
            {
                // parola iniziata
            }
            else
            {
                // parola finita
                tmp[j] = '\0'; // A questo punto 'tmp' contiene la parola letta

                if (!checkIfWordInArray(array_parole, wordsCounter, tmp))
                {

                    wordsCounter++;                                                      // Aumento una parola
                    array_parole = realloc(array_parole, wordsCounter * sizeof(char *)); // Aggiungo una stringa all'array di stringhe

                    array_parole[wordsCounter - 1] = malloc(WORD_LENGHT); // creo la stringa
                    strcpy(array_parole[wordsCounter - 1], tmp);          // Gli assegno il valore della parola letta
                }

                j = 0;
            }
        }
        else
        {
            tmp[j] = src[i];
            j++;
        }
    }

    if (j != 0)
    {
        if (!checkIfWordInArray(array_parole, wordsCounter, tmp))
        {

            wordsCounter++;                                                      /* Aumento una parola*/
            array_parole = realloc(array_parole, wordsCounter * sizeof(char *)); // Aggiungo una stringa all'array di stringhe

            array_parole[wordsCounter - 1] = malloc(WORD_LENGHT); // creo la stringa
            strcpy(array_parole[wordsCounter - 1], tmp);          // Gli assegno il valore della parola letta
        }
    }

    *numberOfWords = wordsCounter;

    return array_parole;
}

int main(int argc, char *argv[])
{

    int wordsCounter = 0; /* Questa variabile conterrà il numero di parole del file */

    char **array_parole = getWordFromFile(argv[1], &wordsCounter); /* Conta le parole del file */

    printf("il file contiene %d parole distinte.\n", wordsCounter);

    // prettyPrintWords(array_parole, wordsCounter);

    exit(0);
}

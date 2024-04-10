#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

// Headers personali
#include "headers/ioOperation.h"
#include "headers/wordArray.h"

int main(int argc, char *argv[])
{

    char *fileName = argv[1]; // Il file di testo in input per il compito 1

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

    int fileSize = sbuf.st_size; // Le dimensioni del file in input

    char *src = mmap(0, fileSize, PROT_READ, MAP_SHARED, fdIn, 0);

    if (src == MAP_FAILED)
    { // Eseguo il mapping del file nel buffer 'src'
        printf("errore nel mapping del file \n");
        exit(1);
    }
    close(fdIn);

    /* A questo punto del codice, all'interno del buffer 'src' è presente il file */

    wordsInit();

    char tmp[WORD_LENGHT];
    int j = 0;

    for (int i = 0; i < fileSize; i++)
    { // Test printing file

        if ((src[i] == ' ' && src[i + 1] == '\n') ||
            src[i] == ' ' || src[i] == '\n')
        {
            // END WORD
            tmp[WORD_LENGHT - 1] == '\0';
            j = 0;
            if (tmp[0] != ' ')
            {
                printf("%s\n", tmp);
            }
            addWord(tmp);
            for (int k = 0; k < strlen(tmp); k++)
            {
                tmp[k] = ' ';
            }
        }
        else
        {
            tmp[j] = src[i];
            j++;
        }
    }
    if (tmp[0] != ' ')
    {
        printf("%s\n", tmp);
    }
    addWord(tmp);

    // printWords();
    printf("\n%d\n", wordsLenght);

    exit(0);
}
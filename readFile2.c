#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

// Headers personali
#include "headers/wordArray.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Deve essere fornito il path di un file.\n");
        exit(1);
    }
    char *fileName = argv[1]; // Il file di testo in input per il compito 1

    FILE *fp = fopen(fileName, "r");
    ;

    if (fp == NULL)
    { // Viene aperto il file input in sola lettura
        printf("errore nell'apertura di %s\n", fileName);
        exit(1);
    }

    char tmpChar;
    tmpChar = fgetc(fp);

    char tmpWord[WORD_LENGHT];
    int offset = 0;

    while (tmpChar != EOF)
    {
        if (tmpChar != ' ') // continua la parola
        {
            printf("%c", tmpChar);
            tmpWord[offset] = tmpChar;
        }
        else
        {
            printf("\n");
            // FINE PAROLA
            for (int i = 0; i < WORD_LENGHT; i++)
            { // Resetta la tmpWord
                tmpWord[i] = ' ';
            }
        }

        // Fine ciclo
        offset++;
        tmpChar = fgetc(fp);
    }
}
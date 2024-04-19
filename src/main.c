#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

#include "../headers/rootHeader.h"
#include "../headers/compito2.h"
#include "../headers/compito1.h"

int main(int argc, char *argv[])
{
    // testGraph();
    if (argc < 4) // è necessario passare almeno un file in input ed uno in output
    {
        printf("è necessario fornire un file input, un file output,\n e specificare se si vuole eseguire il compito 1 o 2\n");
        exit(1);
    }
    if (strcmp(argv[3], "1") == 0)
    {
        compito1(argv[1], argv[2]);
    }
    else if (strcmp(argv[3], "2") == 0)
    {
        if (argc < 5)
        {
            printf("è necessario fornire un numero di parole da generare!\n");
            exit(1);
        }

        char punto[WORD_LENGHT] = ".";
        if (argc < 6)
        {
            compito2(argv[2], argv[1], argv[4], punto);
        }
        else
        {
            compito2(argv[2], argv[1], argv[4], argv[5]);
        }
    }
    else
    {
    }
}

int randomStart(char *input) /* Questa funzione seleziona un punto casuale dalla quale partire fra '?',  '.'  e '!' */
{
}
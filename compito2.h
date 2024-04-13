#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

// Headers personali
#include "headers/stringOpertion.h" /* contiene funzioni riguardo il controllo delle stringhe*/

int compito2(char *input, char *output, char *numParole)
{
    int fileSize;
    char *src = putFileInBuffer(input, &fileSize);
    if (atoi(numParole) < 1)
    {
        printf("inserire numero parole corretto\n");
        exit(1);
    }
}
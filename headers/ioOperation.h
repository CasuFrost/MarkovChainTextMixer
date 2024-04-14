#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

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
        printf("Il file %s passato in input è vuoto!\n", fileName);
        exit(1);
        char *src = malloc(0);
        return src;
    }

    char *src = mmap(NULL, sbuf.st_size, PROT_READ, MAP_SHARED, fdIn, 0);

    if (src == MAP_FAILED)
    { // Eseguo il mapping del file nel buffer 'src'
        printf("errore nel mapping del file \n");
        exit(1);
    }

    close(fdIn);
    /* A questo punto del codice, all'interno del buffer 'src' è presente il file */

    return src;
}
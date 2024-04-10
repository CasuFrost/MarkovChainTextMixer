#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

char *putFileInBuffer(char *fileName) /*Questa funzione riceve in input il path di un file, e restituisce un
 puntatore ad una stringa contenente il file.*/
{
    void *src;
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

    src = mmap(0, fileSize, PROT_READ, MAP_SHARED, fdIn, 0);

    if (src == MAP_FAILED)
    { // Eseguo il mapping del file nel buffer 'src'
        printf("errore nel mapping del file \n");
        exit(1);
    }
    close(fdIn);

    return (char *)src;
    /* A questo punto del codice, all'interno del buffer 'src' è presente il file */
}
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>

#define MAX_LINE_LENGHT 150000
#define WORD_LENGHT 62 /*30 byte per la parola, 1 byte per un eventuale apostrofo, il byte finale per '\0' */
//

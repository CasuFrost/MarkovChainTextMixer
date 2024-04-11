#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

// Headers personali
// #include "headers/ioOperation.h"

#include "headers/structures.h"

int main()
{
    struct words a;

    printf("%d", sizeof(a));
}
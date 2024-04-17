#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <errno.h>

#define WORD_LENGHT 62 /*30 byte per la parola, 1 byte per un eventuale apostrofo, il byte finale per '\0' */

struct Node
{
    char word[WORD_LENGHT];
    int nearSize;
    struct Edge *edges;
};

struct Edge
{
    struct Node *node;
    float weight;
};

struct Node *nodes;

void addNear(struct Node toAdd, struct Node neighbor, float w)
{
    /* crea un edge fra due nodi */
}

void testGraph()
{
    nodes = malloc(sizeof(struct Node) * 3);

    struct Node a, b, c;

    strcpy(a.word, "ciao");

    strcpy(b.word, "sono");
    strcpy(c.word, "marco");

    nodes[0] = a;
    nodes[1] = b;
    nodes[2] = c;

    // addNear(a, b, 1);

    //  printf("%s\n", nodes[0].word);
    //    printf("%s\n", nodes[0].edges[0]);

    exit(0);
}

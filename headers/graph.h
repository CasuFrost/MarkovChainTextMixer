#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <time.h>
#define MAX_LINE_LENGHT 150000
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

int nodesSize = 0;
struct Node *nodes;

void addNear(struct Node *nodeFrom, struct Node *nodeTo, float w)
{
    nodeFrom->nearSize++;
    if (nodeFrom->nearSize == 1)
    {
        nodeFrom->edges = calloc(1, (sizeof(struct Edge)));
    }
    else
    {
        nodeFrom->edges = realloc(nodeFrom->edges, nodeFrom->nearSize * (sizeof(struct Edge)));
    }

    struct Edge e = {nodeTo, w};
    e.node = nodeTo;
    e.weight = w;

    nodeFrom->edges[nodeFrom->nearSize - 1] = e;
}

void createEdge(struct Node *node1, struct Node *node2, float w)
{
    addNear(node1, node2, w);
    // addNear(node2, node1, w);
}

void printGraph()
{
    for (int i = 0; i < nodesSize; i++)
    {
        printf("\nnodo : %s vicini :", nodes[i].word);

        for (int j = 0; j < nodes[i].nearSize; j++)
        {
            printf("%s - %f, ", (nodes[i]).edges[j].node->word, (nodes[i]).edges[j].weight);
        }
    }
}

void createNode(char word[WORD_LENGHT])
{
    struct Node newNode;
    strcpy(newNode.word, word);
    newNode.nearSize = 0;

    if (nodesSize == 0)
    {
        nodes = (struct Node *)calloc(1, sizeof(struct Node));
    }
    else
    {
        nodes = (struct Node *)realloc(nodes, (nodesSize + 1) * sizeof(struct Node));
    }

    if (nodes == NULL)
    {
        printf("errore sulla malloc\n");
        exit(1);
    }

    nodes[nodesSize] = newNode;
    nodesSize++;
}

int searchIdFromWord(char word[WORD_LENGHT])
{
    for (int i = 0; i < nodesSize; i++)
    {
        if (strcmp(nodes[i].word, word) == 0)
        {
            return i;
        }
    }
    return -1;
}

void createEdgeFromWord(char word1[WORD_LENGHT], char word2[WORD_LENGHT], float w)
{
    int id1 = searchIdFromWord(word1);
    int id2 = searchIdFromWord(word2);

    if (id1 == -1 || id2 == -1)
    {
        return;
    }
    createEdge(&nodes[id1], &nodes[id2], w);
}

void createGraphFromFile(char *fileName)
{
    FILE *fp;
    fp = fopen(fileName, "r"); /*Apro il file di input*/
    char line[MAX_LINE_LENGHT];

    /* Nel primo ciclo while creo tutti i nodi*/

    while (fgets(line, MAX_LINE_LENGHT, fp)) // Leggo il file riga per riga
    {

        char tmp[WORD_LENGHT]; // Controllo la prima parola in questione

        int i = 0;

        while (i < strlen(line)) // Seleziono la prima parola della linea
        {
            if (line[i] == ',')
            {
                tmp[i] = '\0';
                break;
            }
            tmp[i] = line[i];
            i++;
        }

        // 'tmp' contiene la prima parola
        createNode(tmp);
    }

    /* Nel secondo ciclo while creo tutti gli archi */
    fseek(fp, 0, SEEK_SET); /* Torno all'inizio del file */

    char nodeTo[WORD_LENGHT]; // Controllo la prima parola in questione
    char tmp[WORD_LENGHT];
    char freq[WORD_LENGHT];

    while (fgets(line, MAX_LINE_LENGHT, fp)) // Leggo il file riga per riga
    {

        int i = 0;

        while (i < strlen(line)) // Seleziono la prima parola della linea
        {
            if (line[i] == ',')
            {
                nodeTo[i] = '\0';
                break;
            }
            nodeTo[i] = line[i];
            i++;
        }
        // printf("%s\n", nodeTo);

        i++;

        int j = 0;

        // Tutti gli archi partiranno da 'nodeTo'

        while (i < strlen(line))
        {
            if (line[i] == ',')
            {
                tmp[j] = '\0';
                // Dentro 'tmp' ho la parola
                j = 0;
                i++;
                while (line[i] != ',' && i < strlen(line))
                {
                    freq[j] = line[i];
                    i++;
                    j++;
                }
                freq[j] = '\0';
                j = 0;

                createEdgeFromWord(nodeTo, tmp, atof(freq));
                // printf("%s %s\n", tmp, freq);
            }
            else
            {
                tmp[j] = line[i];

                j++;
            }
            i++;
        }
    }
}

int selectNearId(char word[WORD_LENGHT])
{
    int id = searchIdFromWord(word);
    if (id == -1)
    {
        printf("parola nel grafo non trovata : %s", word);
        exit(1);
        return;
    }

    float random = (float)rand() / (float)RAND_MAX; /* Simulo una variabile aleatoria continua uniforme in [0,1] */

    float prev = 0;
    int k = nodes[id].nearSize;
    for (int j = 0; j < k; j++)
    {
        if ((random >= prev) && (random <= prev + (nodes[id]).edges[j].weight))
        {
            return searchIdFromWord((nodes[id]).edges[j].node->word);
        }
        prev += (nodes[id]).edges[j].weight;
    }

    return searchIdFromWord((nodes[id]).edges[k - 1].node->word);
}

void writeOnFile(char *fileName, int words, char start[WORD_LENGHT])
{
    FILE *fp;
    fp = fopen(fileName, "w+"); /*Apro il file di input*/
    int k = words;
    int id = selectNearId(start);

    int maiusc = 1;
    char tmp[WORD_LENGHT];
    while (words > 0)
    {
        strcpy(tmp, nodes[id].word);

        if (maiusc == 1)
        {
            if (tmp[0] > 96 && tmp[0] < 123)
            {
                tmp[0] -= 32;
            }
            maiusc = 0;
        }

        fprintf(fp, "%s ", tmp);

        id = selectNearId(nodes[id].word);
        if (strcmp(tmp, ".") == 0 || strcmp(tmp, "!") == 0 || strcmp(tmp, "?") == 0)
        {
            /* Se la parola attuale è un punto, la prossima avrà la iniziale maiuscola */
            maiusc = 1;
        }
        if (words % 20 == 0 && words != k)
        {
            fprintf(fp, "\n");
        }
        words--;
    }

    fclose(fp);
}

void testGraph()
{
    createNode("ciao");
    createNode("cane");
    createNode("pisello");

    createEdge(&nodes[0], &nodes[1], 0.5);
    createEdge(&nodes[2], &nodes[0], 0.5);

    // printf("parola : %s\n", (nodes[1]).edges[0].node->word);
    printGraph();
    exit(0);
}

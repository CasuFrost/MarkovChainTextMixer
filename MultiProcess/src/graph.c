
#include "../headers/graph.h"

struct Node
{
    char word[WORD_LENGHT];
    int nearSize;
    int nodeId;
    struct Edge *edges;
};

struct Edge
{
    struct Node *node;
    float weight;
};

int nodesSize = 0;
struct Node *nodes;

struct Node *getGraph()
{
    struct Node *n = nodes;
    return n;
}

void addNearId(int nodeFrom, int nodeTo, float w)
{
    nodes[nodeFrom].nearSize++;
    if (nodes[nodeFrom].nearSize == 1)
    {
        nodes[nodeFrom].edges = calloc(1, (sizeof(struct Edge)));
    }
    else
    {
        nodes[nodeFrom].edges = realloc(nodes[nodeFrom].edges, nodes[nodeFrom].nearSize * (sizeof(struct Edge)));
    }

    if (nodes[nodeFrom].edges == NULL)
    {
        printf("errore nell'allocazione della memoria\n");
        exit(1);
    }

    struct Edge e = {&(nodes[nodeTo]), w};

    nodes[nodeFrom].edges[nodes[nodeFrom].nearSize - 1] = e;
}

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

    if (nodeFrom->edges == NULL)
    {
        printf("errore nell'allocazione della memoria\n");
        exit(1);
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
    // return;
    for (int i = 0; i < nodesSize; i++)
    {
        printf("\nnodo : %s vicini :", nodes[i].word);

        for (int j = 0; j < nodes[i].nearSize; j++)
        {
            printf("%s - %f, ", (nodes[i]).edges[j].node->word, (nodes[i]).edges[j].weight);
        }
    }
    // printf("\ngrafo printato\n");
}

void createNode(char word[WORD_LENGHT])
{
    struct Node newNode;
    strcpy(newNode.word, word);
    newNode.nearSize = 0;
    newNode.nodeId = nodesSize;

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
        printf("errore nell'allocazione della memoria\n");
        exit(1);
    }

    nodes[nodesSize] = newNode;
    nodesSize++;
}

int searchIdFromWord(char word[WORD_LENGHT]) /* Questa funzione  data una parola restituisce l'ID (la posizione nell'array) del nodo con quella parola*/
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

void createEdgeFromWord(int id1, char word2[WORD_LENGHT], float w)
{

    int id2 = searchIdFromWord(word2);

    if (id1 == -1 || id2 == -1)
    {
        return;
    }
    createEdge(&nodes[id1], &nodes[id2], w);
}

void createGraphFromFile(char *fileName)
{
    int wordCounter = 0;
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

                createEdgeFromWord(wordCounter, tmp, atof(freq));
            }
            else
            {
                tmp[j] = line[i];

                j++;
            }
            i++;
        }
        wordCounter++;
    }
}

int selectNearId(int id) /* dato un nodo del grafo, seleziona il prossimo nodo in maniera casuale in base ai pesi sui grafi */
{
    if (id == -1)
    {
        printf("parola nel grafo non trovata : %s", nodes[id].word);
        exit(1);
    }

    float random = (float)rand() / (float)RAND_MAX; /* Simulo una variabile aleatoria continua uniforme in [0,1] */

    float prev = 0;
    int k = nodes[id].nearSize;

    for (int j = 0; j < k; j++)
    {
        if ((random >= prev) && (random <= prev + (nodes[id]).edges[j].weight))
        {
            return (nodes[id]).edges[j].node->nodeId;
        }
        prev += (nodes[id]).edges[j].weight;
    }

    return (nodes[id]).edges[k - 1].node->nodeId; /* Dati gli errori di approssimazione, la somma di tutte le frequenze potrebbe essere minore di 1 */
}

void writeOnFile(char *fileName, int words, char start[WORD_LENGHT]) /*Questa funzione legge il grafo contenente le parole e le frequenze, ed eseguendo una passeggiata
sul grafo, scrive il contenuto sul file*/
{
    FILE *fp;
    fp = fopen(fileName, "w+"); /*Apro il file di output*/

    if (fp == NULL)
    {
        printf("Errore nell'apertura dei file.\n");
        exit(1);
    }

    int firstWord = 0; /* Questa variabile serve esclusivamente ad indicare quando o no andare a capo*/

    if (searchIdFromWord(start) == -1) /* Se il punto di partenza non è presente, si parte dal punto */
    {
        if ((strcmp(start, "!") == 0) || (strcmp(start, "?") == 0))
        {
            strcpy(start, ".");
        }
        else
        {
            printf("La parola che hai inserito, non è presente nel testo!\n");
            exit(1);
        }
    }

    int id = selectNearId(searchIdFromWord(start));

    int maiusc = 1;
    char tmp[WORD_LENGHT];

    while (words > 0)
    {
        strcpy(tmp, nodes[id].word);

        if (maiusc == 1) /* Controllo se la parola deve avere la prima lettera maiuscola */
        {
            if (tmp[0] > 96 && tmp[0] < 123)
            {
                tmp[0] -= 32;
            }
            maiusc = 0;
        }

        fprintf(fp, "%s ", tmp); /* Scrivo sul file la parola ed uno spazio */

        id = selectNearId(id); /* Seleziono la prossima parola tramite uno dei nodi adiacenti al nodo attuale */

        if (strcmp(tmp, ".") == 0 || strcmp(tmp, "!") == 0 || strcmp(tmp, "?") == 0)
        {
            /* Se la parola attuale è un punto, la prossima avrà la iniziale maiuscola */
            maiusc = 1;
        }

        if (words % 20 == 0 && firstWord == 1) /* Ogni 20 parole, vado a capo */
        {
            fprintf(fp, "\n");
        }

        firstWord = 1;
        words--;
    }

    freeGraphStructures(); /* Libero le strutture del grafo che ho allocato */
    fclose(fp);
}

void freeGraphStructures()
{
    for (int i = 0; i < nodesSize; i++)
    {
        free(nodes[i].edges);
    }
    free(nodes);
}

void writeOnFileGraph(char *fileName, int words, char start[WORD_LENGHT]) /*Questa funzione legge il grafo contenente le parole e le frequenze, ed eseguendo una passeggiata
sul grafo, scrive il contenuto sul file*/
{
    FILE *fp;
    fp = fopen(fileName, "w+"); /*Apro il file di output*/

    if (fp == NULL)
    {
        printf("Errore nell'apertura dei file.\n");
        exit(1);
    }

    int firstWord = 0; /* Questa variabile serve esclusivamente ad indicare quando o no andare a capo*/

    if (searchIdFromWord(start) == -1) /* Se il punto di partenza non è presente, si parte dal punto */
    {
        if ((strcmp(start, "!") == 0) || (strcmp(start, "?") == 0))
        {
            strcpy(start, ".");
        }
        else
        {
            printf("La parola che hai inserito, non è presente nel testo!\n");
            exit(1);
        }
    }

    int id = selectNearId(searchIdFromWord(start));

    int maiusc = 1;
    char tmp[WORD_LENGHT];

    while (words > 0)
    {
        strcpy(tmp, nodes[id].word);

        if (maiusc == 1) /* Controllo se la parola deve avere la prima lettera maiuscola */
        {
            if (tmp[0] > 96 && tmp[0] < 123)
            {
                tmp[0] -= 32;
            }
            maiusc = 0;
        }

        fprintf(fp, "%s ", tmp); /* Scrivo sul file la parola ed uno spazio */

        id = selectNearId(id); /* Seleziono la prossima parola tramite uno dei nodi adiacenti al nodo attuale */

        if (strcmp(tmp, ".") == 0 || strcmp(tmp, "!") == 0 || strcmp(tmp, "?") == 0)
        {
            /* Se la parola attuale è un punto, la prossima avrà la iniziale maiuscola */
            maiusc = 1;
        }

        if (words % 20 == 0 && firstWord == 1) /* Ogni 20 parole, vado a capo */
        {
            fprintf(fp, "\n");
        }

        firstWord = 1;
        words--;
    }

    freeGraphStructures(); /* Libero le strutture del grafo che ho allocato */
    fclose(fp);
}

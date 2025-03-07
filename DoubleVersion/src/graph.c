#include "../headers/rootHeader.h"
#include "../headers/graph.h"
#include "../headers/stringOpertion.h"
#include "../headers/hashTable.h"

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

struct WordId
{
    char word[WORD_LENGHT];
    int wordId;
    struct WordId *next;
};

struct WordId *hashTable[HASHSIZE];

int collisionsCounter = 0;
int nodesSize = 0;
struct Node *nodes;

void printHashTable() /*Per scrivere a schermo la hash table*/
{
    for (int i = 0; i < HASHSIZE; i++)
    {
        if (hashTable[i] != NULL)
        {
            printf("%s : %d", hashTable[i]->word, i);
            if (hashTable[i]->next != NULL)
            {
                struct WordId *tmp = hashTable[i];
                while (tmp->next != NULL)
                {
                    tmp = tmp->next;
                    printf("--> %s", tmp->word);
                }
            }
            printf("\n");
        }
    }
}

void addNear(struct Node *nodeFrom, struct Node *nodeTo, float w) /*Questa funzione crea un arco pesato diretto fra i due nodi dati in input*/
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

void createNode(char word[WORD_LENGHT]) /*Questa funzione, data una parola in input, crea un nodo, che ha come chiave proprio quella parola*/
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

    // Add word in hashMap
    int hashVal = hash(word);
    if (hashTable[hashVal] == NULL) /*Tramite l'hashmap, si controlla che la parola non sia già nel grafo*/
    {
        hashTable[hashVal] = malloc(sizeof(struct WordId));
        if (hashTable[hashVal] == NULL)
        {
            printf("errore nella malloc()\n");
            exit(1);
        }
        hashTable[hashVal]->wordId = nodesSize;
        hashTable[hashVal]->next = NULL;
        strcpy(hashTable[hashVal]->word, word);
    }
    else
    {
        collisionsCounter++;
        struct WordId *tmp = hashTable[hashVal];
        while (tmp->next != NULL)
        {
            tmp = tmp->next;
        }
        tmp->next = malloc(sizeof(struct WordId));
        if (tmp->next == NULL)
        {
            printf("errore nella malloc()\n");
            exit(1);
        }
        (tmp->next)->wordId = nodesSize;
        (tmp->next)->next = NULL;
        strcpy((tmp->next)->word, word);
    }

    nodesSize++;
}

int getIdFromWordHasMap(char word[WORD_LENGHT]) /*Questa funzione data una parole, ne restituisce l'ID*/
{
    int hashVal = hash(word);
    if (hashTable[hashVal] == NULL)
    {
        printf("errore su %s indice : %d\n", word, hashVal);
        return -1;
    }
    else
    {
        if (strcmp(hashTable[hashVal]->word, word) == 0)
        {
            return hashTable[hashVal]->wordId;
        }
        struct WordId *tmp = hashTable[hashVal];
        while (tmp->next != NULL)
        {
            tmp = tmp->next;
            if (strcmp(tmp->word, word) == 0)
            {
                return tmp->wordId;
            }
        }
    }
    return -1;
}

void createEdgeFromWord(int id1, char word2[WORD_LENGHT], float w) /*Dato l'ID di un nodo ed una parola rappresentante un nodo, viene creato un arco*/
{

    int id2 = getIdFromWordHasMap(word2);

    if (id1 == -1 || id2 == -1)
    {
        return;
    }
    addNear(&nodes[id1], &nodes[id2], w);
}

void createGraphFromFile(char *fileName) /*Dato in input un file CSV, crea un grafo dove
i vertici sono le parole del file CSV
c'è un arco di peso 'w' da 'x' ad 'y' se la parola 'y' segue 'x' con una frequenza 'w'

*/
{

    int wordCounter = 0;
    FILE *fp;
    fp = fopen(fileName, "r"); /*Apro il file di input*/
    if (fp == NULL)
    {
        printf("file input inesistente!\n");
        exit(1);
    }
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

    float prev = 0;
    int k = nodes[id].nearSize;

    float sup = 0;
    for (int j = 0; j < k; j++)
    {
        sup += (nodes[id]).edges[j].weight;
    }

    float random = (float)rand() / (float)(RAND_MAX / sup); /* Simulo una variabile aleatoria continua uniforme in [0,1] */

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

    lowerCase(start);

    int firstWord = 0; /* Questa variabile serve esclusivamente ad indicare quando o no andare a capo*/

    if (getIdFromWordHasMap(start) == -1) /* Se il punto di partenza non è presente, si parte dal punto */
    {
        if ((strcmp(start, "!") == 0) || (strcmp(start, "?") == 0))
        {
            strcpy(start, ".");
        }
        else
        {
            printf("La parola che hai inserito, ossia %s non è presente nel testo!\n", start);
            exit(1);
        }
    }

    printf("parola iniziale :  %s\n", start);

    int maiusc = 1;

    if (!((strcmp(start, "!") == 0) || (strcmp(start, "?") == 0) || (strcmp(start, ".") == 0)))
    {
        char tmp[WORD_LENGHT];
        start[0] -= 32;
        fprintf(fp, "%s ", start);
        start[0] += 32;
        maiusc = 0;
    }

    int id = selectNearId(getIdFromWordHasMap(start));

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
    // printHashTable();
    printf("collisioni : %d\n", collisionsCounter);
    freeGraphStructures(); /* Libero le strutture del grafo che ho allocato */
    fclose(fp);
}

void writeOnFile_multi(int words, char start[WORD_LENGHT], int outputPipe, int endPipe) /*Questa funzione legge il grafo contenente le parole e le frequenze, ed eseguendo una passeggiata
 sul grafo, invia ad un altro processo le parole che dovrà scrivere sul file txt*/
{

    lowerCase(start);

    int firstWord = 0; /* Questa variabile serve esclusivamente ad indicare quando o no andare a capo*/

    if (getIdFromWordHasMap(start) == -1) /* Se il punto di partenza non è presente, si parte dal punto */
    {
        if ((strcmp(start, "!") == 0) || (strcmp(start, "?") == 0))
        {
            strcpy(start, ".");
        }
        else
        {
            printf("La parola che hai inserito, ossia %s non è presente nel testo!\n", start);
            exit(1);
        }
    }

    printf("parola iniziale :  %s\n", start);

    int maiusc = 1;

    if (!((strcmp(start, "!") == 0) || (strcmp(start, "?") == 0) || (strcmp(start, ".") == 0)))
    {
        char tmp[WORD_LENGHT];
        start[0] -= 32;

        write(outputPipe, start, WORD_LENGHT);
        start[0] += 32;
        maiusc = 0;
    }

    int id = selectNearId(getIdFromWordHasMap(start));

    char tmp[WORD_LENGHT];
    char accapo[WORD_LENGHT] = "\n";
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
        write(outputPipe, tmp, WORD_LENGHT);

        id = selectNearId(id); /* Seleziono la prossima parola tramite uno dei nodi adiacenti al nodo attuale */

        if (strcmp(tmp, ".") == 0 || strcmp(tmp, "!") == 0 || strcmp(tmp, "?") == 0)
        {
            /* Se la parola attuale è un punto, la prossima avrà la iniziale maiuscola */
            maiusc = 1;
        }

        if (words % 20 == 0 && firstWord == 1) /* Ogni 20 parole, vado a capo */
        {

            write(outputPipe, accapo, WORD_LENGHT);
        }

        firstWord = 1;
        words--;
    }

    printf("collisioni : %d\n", collisionsCounter);
    freeGraphStructures(); /* Libero le strutture del grafo che ho allocato */

    char endMsg[10] = "fine";
    write(endPipe, endMsg, 10);
}

void freeGraphStructures()
{

    for (int i = 0; i < nodesSize; i++)
    {
        free(nodes[i].edges);
    }
    free(nodes);
}

#include "../headers/compito2.h"

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

void compito2(char *input, char *output, char *numParole, char start[WORD_LENGHT]) /* Esistono due versioni del programma, la prima, opera direttamente sul file (efficente per creare poche
 parole), la seconda, crea un grafo pesato leggendo il CSV, e poi opera sul grafo, l'operazione di creazione del grafo è dispendiosa, ma poi sarà rapido scrivere sul file di output,
 per questo se le parole son poche verrà effettuato il processo con la prima versione, se invece son tante, verrà creato il grafo.*/
{

    int Input_Graph_Pipe[2];
    int nextStep_Pipe[2];
    int endPipe[2];

    pid_t pid;

    pipe(Input_Graph_Pipe); // Questa pipe verrà utilizzata per i dati
    pipe(nextStep_Pipe);    // Questa pipe verrà per coordinare figlio e padre
    pipe(endPipe);          // Questa pipe verrà utilizzata per decretare la fine della lettura

    if ((pid = fork()) == -1)
    {
        printf("errore nel fork");
        exit(1);
    }

    if (pid != 0)
    {
        // Figlio
        char readbuffer[80];

        close(Input_Graph_Pipe[0]);

        int wordCounter = 0;
        FILE *fp;
        fp = fopen(input, "r"); /*Apro il file di input*/
        char line[MAX_LINE_LENGHT];
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
            write(Input_Graph_Pipe[1], tmp, (strlen(tmp) + 1));
            write(endPipe[1], "aaa", 3);

            read(nextStep_Pipe[0], readbuffer, sizeof(readbuffer)); // Il figlio attende l'ordine del padre per la prossima parola
        }
        write(endPipe[1], "end", 3);

        // A questo punto del codice, il padre ha letto tutte le parole distinte nel file CSV, e le ha inviate al figlio
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

                    char buffer[2 * WORD_LENGHT];
                    sprintf(buffer, "%d %s %s\0", wordCounter, tmp, freq);
                    write(endPipe[1], "aaa", 3);
                    // printf("Invio la stringa : %s\n", buffer);
                    write(Input_Graph_Pipe[1], buffer, (strlen(buffer) + 1));
                    read(nextStep_Pipe[0], readbuffer, sizeof(readbuffer)); // Il figlio attende l'ordine del padre per la prossima parola
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
        printf("scrivo fine\n");
        write(endPipe[1], "end\0", 5);
        write(Input_Graph_Pipe[1], "end", 3);
    }
    else
    {
        // Padre
        close(Input_Graph_Pipe[1]);
        // fcntl(endPipe[0], F_SETFL, O_NONBLOCK);
        char readbuffer[WORD_LENGHT];
        char endBuffer[80];

        while (1)
        {
            read(endPipe[0], endBuffer, sizeof(endBuffer));
            if (strcmp(endBuffer, "end") == 0)
            {
                break;
            }
            read(Input_Graph_Pipe[0], readbuffer, sizeof(readbuffer));

            createNode(readbuffer);

            write(nextStep_Pipe[1], "nulla", 1); // Il padre notifica al figlio che può scrivere la prossima parola sulla PIPE
        }
        // printGraph();
        // printf("\n");

        // A questo punto del codice, il figlio ha ottenuto le parole distinte e creato i nodi del grafo (non gli archi)
        while (1)
        {
            read(endPipe[0], endBuffer, sizeof(endBuffer));
            if (strcmp(endBuffer, "end") == 0)
            {
                printf("\nfine\n");
                exit(0);
            }
            read(Input_Graph_Pipe[0], readbuffer, sizeof(readbuffer));

            int j = 0;
            int i = 0;
            char num[WORD_LENGHT];
            char tmp[WORD_LENGHT];
            char freq[WORD_LENGHT];

            while (i < strlen(readbuffer))
            {
                if (readbuffer[i] == 32)
                {
                    num[j] = '\0';
                    j = 0;
                    i++;
                    break;
                }
                num[j] = readbuffer[i];
                i++;
                j++;
            }
            while (i < strlen(readbuffer))
            {
                if (readbuffer[i] == 32)
                {
                    tmp[j] = '\0';
                    j = 0;
                    i++;
                    break;
                }
                tmp[j] = readbuffer[i];
                i++;
                j++;
            }
            while (i < strlen(readbuffer))
            {
                freq[j] = readbuffer[i];
                i++;
                j++;
            }
            freq[j] = '\0';
            printf("\n\nho ricevuto : %s %s %s\n\n", num, tmp, freq);
            // createEdgeFromWord(num, tmp, atof(freq));

            write(nextStep_Pipe[1], "nulla", 1); // Il padre notifica al figlio che può scrivere la prossima parola sulla PIPE
        }
    }
}

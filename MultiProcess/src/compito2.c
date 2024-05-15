#include "../headers/compito2.h"
#include "../headers/graph.h"
#include "../headers/fileReader2.h"

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

void readStringReciviedFromPipe(char readbuffer[WORD_LENGHT])
{

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

    createEdgeFromWord(atoi(num), tmp, atof(freq));
    // printf("*** || %s *** ||\n", readbuffer);
    //  printf("*** || %d %s %f *** ||\n", atoi(num), tmp, atof(freq));
}

void compito2(char *input, char *output, char *numParole, char start[WORD_LENGHT]) /* Esistono due versioni del programma, la prima, opera direttamente sul file (efficente per creare poche
 parole), la seconda, crea un grafo pesato leggendo il CSV, e poi opera sul grafo, l'operazione di creazione del grafo è dispendiosa, ma poi sarà rapido scrivere sul file di output,
 per questo se le parole son poche verrà effettuato il processo con la prima versione, se invece son tante, verrà creato il grafo.*/
{

    srand(time(NULL));
    int Input_Graph_Pipe[2];
    int nextStep_Pipe[2];
    int endPipe[2];
    int endPipe2[2];

    pid_t pid;

    pipe(Input_Graph_Pipe); // Questa pipe verrà utilizzata per i dati
    pipe(nextStep_Pipe);    // Questa pipe verrà per coordinare figlio e padre
    pipe(endPipe);          // Questa pipe verrà utilizzata per decretare la fine della lettura
    pipe(endPipe2);

    if ((pid = fork()) == -1)
    {
        printf("errore nel fork");
        exit(1);
    }

    if (pid != 0)
    {
        // Padre
        readFileAndSendWords(input, Input_Graph_Pipe, endPipe, nextStep_Pipe, endPipe2);
    }
    else
    {
        pid_t pid2;

        close(Input_Graph_Pipe[1]);

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

        while (1)
        {

            read(endPipe2[0], endBuffer, sizeof(endBuffer));

            if (strcmp(endBuffer, "end\0") == 0)
            {

                break;
            }
            read(Input_Graph_Pipe[0], readbuffer, sizeof(readbuffer));

            readStringReciviedFromPipe(readbuffer);

            write(nextStep_Pipe[1], "nulla", 1); // Il padre notifica al figlio che può scrivere la prossima parola sulla PIPE
        }

        if ((pid2 = fork()) == -1)
        {
            printf("errore nel fork");
            exit(1);
        }
        if (pid2 == 0)
        {
            writeOnFile(output, atoi(numParole), start);
            printf("terminato il processo che scrive il file di testo casuale.\n\n");
        }
        else
        {
            printf("terminato il processo che crea la struttura.\n");
        }
        exit(0);
    }
}

void handle_sigusr1(int sig)
{
    printf("segnale ricevuto!\n");
    exit(0);
}

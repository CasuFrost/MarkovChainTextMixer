#include "../headers/compito2_multi.h"
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

void readStringReciviedFromPipe(char readbuffer[WORD_LENGHT]) /*Questa funzione, legge le stringhe del file CSV, ne estrapola i dati necessari
 per creare gli archi del grafo*/
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
}

void compito2_multi(char *input, char *output, char *numParole, char start[WORD_LENGHT]) /* Esistono due versioni del programma, la prima, opera direttamente sul file (efficente per creare poche
 parole), la seconda, crea un grafo pesato leggendo il CSV, e poi opera sul grafo, l'operazione di creazione del grafo è dispendiosa, ma poi sarà rapido scrivere sul file di output,
 per questo se le parole son poche verrà effettuato il processo con la prima versione, se invece son tante, verrà creato il grafo.*/
{
    srand(time(NULL));
    int Input_Graph_Pipe[2];
    int Input_Graph_Pipe2[2];
    int nextStep_Pipe[2];
    int endPipe[2];
    int endPipe2[2];

    pid_t pid;

    pipe(Input_Graph_Pipe); // Questa pipe verrà utilizzata per i dati
    pipe(Input_Graph_Pipe2);
    pipe(nextStep_Pipe); // Questa pipe verrà per coordinare figlio e padre
    pipe(endPipe);       // Questa pipe verrà utilizzata per decretare la fine della lettura
    pipe(endPipe2);

    if ((pid = fork()) == -1)
    {
        printf("errore nel fork");
        exit(1);
    }

    if (pid != 0)
    {
        /*Questo è il processo che legge il file CSV ed invia ad un altro processo le righe lette, per la creazione della struttura*/
        readFileAndSendWords(input, Input_Graph_Pipe, endPipe, nextStep_Pipe, endPipe2, Input_Graph_Pipe2);
        int status;
        wait(&status);
        exit(0);
    }
    else
    {
        pid_t pid2;

        int outPipe[2];
        pipe(outPipe);
        fcntl(outPipe[0], F_SETFL, O_NONBLOCK);
        int endPipe3[2];
        pipe(endPipe3);
        fcntl(endPipe3[0], F_SETFL, O_NONBLOCK);
        char endBuf[10];
        char outputBuf[WORD_LENGHT];

        if ((pid2 = fork()) == -1)
        {
            printf("errore nel fork");
            exit(1);
        }

        if (pid2 == 0)
        {
            /*Questo processo attende le parole da scrivere sul file di testo*/
            FILE *fp;
            fp = fopen(output, "w+"); /*Apro il file di output*/

            if (fp == NULL)
            {
                printf("Errore nell'apertura dei file.\n");
                exit(1);
            }

            int fine = 0;
            while (1)
            {
                int n = read(outPipe[0], outputBuf, WORD_LENGHT);
                if (read(endPipe3[0], endBuf, 10) != -1 || fine == 1)
                {
                    fine = 1;

                    if (n == -1)
                    {

                        break;
                    }
                }
                if (n != -1)
                {
                    fprintf(fp, "%s", outputBuf);
                    if (strcmp(outputBuf, "\n") != 0)
                    {
                        fprintf(fp, " ");
                    }
                }
            }
            fclose(fp);
            printf("terminato il processo che scrive il file di testo casuale.\n");
            exit(0);
        }
        else
        {
            /*Questo processo riceve le righe del CSV e crea la struttura*/
            close(Input_Graph_Pipe[1]);

            char readbuffer[WORD_LENGHT * 3];
            char endBuffer[80];
            int finito_lettura = 0;
            fcntl(endPipe[0], F_SETFL, O_NONBLOCK);
            fcntl(endPipe2[0], F_SETFL, O_NONBLOCK);
            fcntl(Input_Graph_Pipe[0], F_SETFL, O_NONBLOCK);
            fcntl(Input_Graph_Pipe2[0], F_SETFL, O_NONBLOCK);
            while (1)
            {
                int e = read(endPipe[0], endBuffer, sizeof(endBuffer));

                int n = read(Input_Graph_Pipe[0], readbuffer, WORD_LENGHT);
                if (e != -1 || finito_lettura)
                {
                    finito_lettura = 1;
                    if (n == -1)
                    {
                        break;
                    }
                }
                if (n != -1)
                {
                    // printf("%s\n", readbuffer);
                    createNode(readbuffer);
                }
                // write(nextStep_Pipe[1], "nulla", 1); // Il padre notifica al figlio che può scrivere la prossima parola sulla PIPE
            }

            finito_lettura = 0;
            while (1)
            {

                int e = read(endPipe2[0], endBuffer, sizeof(endBuffer));

                int n = read(Input_Graph_Pipe2[0], readbuffer, 3 * WORD_LENGHT);

                if (e != -1 || finito_lettura == 1)
                {

                    finito_lettura = 1;
                    if (n == -1)
                    {
                        break;
                    }
                }
                if (n != -1)
                {
                    // printf("letto : %s\n", readbuffer);
                    readStringReciviedFromPipe(readbuffer);
                }

                // write(nextStep_Pipe[1], "nulla", 1); // Il padre notifica al figlio che può scrivere la prossima parola sulla PIPE
            }
            // printGraph();
            writeOnFile_multi(atoi(numParole), start, outPipe[1], endPipe3[1]);
            printf("terminato il processo che crea la struttura.\n");
            int status;
            wait(&status);
            exit(0);
        }
    }

    exit(0);
}

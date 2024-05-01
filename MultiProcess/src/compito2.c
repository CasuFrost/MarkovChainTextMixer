#include "../headers/compito2.h"
#include "../headers/graph.h"

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
        // Figlio
        readFileAndSendWords(input, Input_Graph_Pipe, endPipe, nextStep_Pipe, endPipe2);
        // exit(0);
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
            // printf("FIGLIO : mi preparo a ricevere, endBuffer : %s\n", endBuffer);
            if (strcmp(endBuffer, "end") == 0)
            {
                break;
            }
            read(Input_Graph_Pipe[0], readbuffer, sizeof(readbuffer));
            // printf("FIGLIO : ho letto ciò che padre ha detto\n");
            createNode(readbuffer);
            write(nextStep_Pipe[1], "nulla", 1); // Il padre notifica al figlio che può scrivere la prossima parola sulla PIPE
            // printf("FIGLIO : pronto al prossimo step\n");
        }
        // printGraph();
        //  printf("ho ricevuto le parole del grafo\n");
        // return;
        // A questo punto del codice, il figlio ha ottenuto le parole distinte e creato i nodi del grafo (non gli archi)
        // printf("figlio INIZIA ASCOLTO, in endBuffer c'è %s\n", endBuffer);
        while (1)
        {
            // printf("figlio si appresta a leggere endPipe\n");
            read(endPipe2[0], endBuffer, sizeof(endBuffer));
            // printf("figlio ha letto endPipe e ora ascolta \n");
            if (strcmp(endBuffer, "end\0") == 0)
            {
                // printf("figlio ha finito\n");
                break;
            }
            read(Input_Graph_Pipe[0], readbuffer, sizeof(readbuffer));
            // printf("figlio legge la seguente riga di dimensione %d:\n", strlen(readbuffer));

            readStringReciviedFromPipe(readbuffer);
            // printf("figlio da conferma\n");
            write(nextStep_Pipe[1], "nulla", 1); // Il padre notifica al figlio che può scrivere la prossima parola sulla PIPE
        }

        pid_t pid2;
        if ((pid2 = fork()) == -1)
        {
            printf("errore nel fork");
            exit(1);
        }

        if (pid2 != 0)
        {
            writeOnFile(output, atoi(numParole), start);
        }
        // else
        //{
        // exit(0);
        //}

        // printGraph();
        exit(0);
    }
}

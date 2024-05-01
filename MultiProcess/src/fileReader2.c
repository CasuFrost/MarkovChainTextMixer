#include "../headers/compito2.h"

void readFileAndSendWords(char *input, int Input_Graph_Pipe[2], int endPipe[2], int nextStep_Pipe[2], int endPipe2[2])
{
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
    write(endPipe[1], "end\0", 4);
    // printf("\nPADRE : ho letto tutte le parole del grafo\n");
    // return;
    //  A questo punto del codice, il padre ha letto tutte le parole distinte nel file CSV, e le ha inviate al figlio
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

                // printf("padre comunica\n");
                write(endPipe2[1], "aaa\0", 5);
                // printf("%d %s %s\n", wordCounter, tmp, freq);
                //    write(endPipe[1], "aaa", 3);
                // printf("Invio la stringa : %s\n", buffer);
                write(Input_Graph_Pipe[1], buffer, (strlen(buffer) + 1));
                // printf("scritta\n");
                read(nextStep_Pipe[0], readbuffer, sizeof(readbuffer)); // Il figlio attende l'ordine del padre per la prossima parola
                // printf("padre riceve conferma\n--------------------------\n");
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
    // printf("padre ha finito\n");
    write(endPipe2[1], "end\0", 5);
    write(Input_Graph_Pipe[1], "end", 3);
}

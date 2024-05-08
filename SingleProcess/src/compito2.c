// Headers personali
#include "../headers/graph.h"
#include "../headers/compito2.h"

typedef struct wordAndFreq /* Questa struttura servirà a contenere le parole successive ad una parola corrente con le relative frequenze*/
{
    char word[WORD_LENGHT];
    float freq;
    float minRange;
    float maxRange;
} wordAndFreq;

void findSuccessive(char line[MAX_LINE_LENGHT], char successive[WORD_LENGHT]) /* Prende in input una linea del file CSV, e seleziona un successivo fra le parole che
 seguono la parola corrente*/
{

    wordAndFreq *nextWords = malloc(0); /* Questo array conterrà tutte le parole candidate ad essere le successive, con le relative frequenze */
    int nextWordsSize = 0;              /* Il numero di parole successive a quella corrente */

    char tmp[WORD_LENGHT];
    int j = 0; /*L'index relativo del buffer che conterrà la parola letta*/
    int firstWord = 0;

    for (int i = 0; i < strlen(line); i++)
    {
        if ((int)line[i] == 10 || (int)line[i] == 32 || (int)line[i] == 9) // Viene letto uno spazio o un accapo
        {
            continue;
        }

        if (line[i] == ',') // Leggo una virgola, ciò che c'è nel buffer sarà una parola
        {
            if (firstWord == 0) /* La prima parola letta è quella corrente e va ignorata*/
            {
                j = 0;
                firstWord++;
                continue;
            }

            else
            {
                tmp[j] = '\0'; /* Termino la stringa 'tmp' */
                j = 0;

                /* A questo punto ho letto una parola candidata ad essere successiva */

                i++;
                char freq[WORD_LENGHT];

                while ((line[i] != ',' && i < strlen(line))) /* Leggo il valore della frequenza */
                {
                    freq[j] = line[i];
                    j++;
                    i++;
                }

                freq[j] = '\0';

                float frequenza = atof(freq); /* La stringa che contiene la frequenza viene letta come float all'interno di una variabil */

                if (frequenza == 1.) /* Se la frequenza di una parola è 1, non sono necessari altri controlli, sarà la successiva al 100% */
                {
                    strcpy(successive, tmp);
                    return;
                }

                /*Inserisco la parola letta con relativa frequenza all'interno dell'array*/
                nextWordsSize++;

                nextWords = realloc(nextWords, nextWordsSize * (sizeof(wordAndFreq)));

                wordAndFreq new;

                strcpy(new.word, tmp);
                new.freq = frequenza;

                nextWords[nextWordsSize - 1] = new;

                // printf("parola: %s freq : %f\n", tmp, frequenza);

                j = 0;
                continue;
            }
        }

        tmp[j] = line[i];
        j++;
    }

    /* A questo punto del file ho dentro l'array 'nextWords' tutte le parole candidate ad essere
    la successiva*/

    /* Ad ogni parola sarà associato un intervallo di float, la parola 0-esima avrà intervallo da 0 alla sua frequenza,
    la parola i-esima avrà intervallo dall'estremo destro dell'intervallo della parola
    i-1 esima a lo stesso valore + la sua frequenza

    ESEMPIO

    parola : cane    frequenza : 0.25
    parola : ladro    frequenza : 0.5
    parola : sai    frequenza : 0.25

    range di cane sarà  : [0,0.25]
    range di ladro sarà : [0.25,0.75]
    range di sai sarà   : [0.75,1]


    */

    float tot = 0;

    for (int i = 0; i < nextWordsSize; i++) /* Assegno i range*/
    {
        nextWords[i].minRange = tot;
        nextWords[i].maxRange = tot + nextWords[i].freq;
        if (nextWords[i].maxRange >= 0.99)
        {
            nextWords[i].maxRange = 1;
        }
        tot += nextWords[i].freq;
    }

    nextWords[nextWordsSize - 1].maxRange = 1.;

    float random = (float)rand() / (float)RAND_MAX; /* Simulo una variabile aleatoria continua uniforme in [0,1] */

    /* Se la parola i ha range [a,b], la probabilità che i sia scelta come successiva è uguale all'integrale definito fra a ed b
    della funzione di densità di 'random' */

    int i = 0;
    for (int i = 0; i < nextWordsSize; i++) /* Controllo a quale parola è associato l'intervallo in cui rientra il numero generato casualmente*/
    {

        if (random >= nextWords[i].minRange && random <= nextWords[i].maxRange)
        {
            // printf("--%s--\n", nextWords[i].word);
            strcpy(successive, nextWords[i].word);
            free(nextWords);
            return;
        }
    }
    printf("fuori dal range con %f\n, il limite destro è %f", random, nextWords[nextWordsSize - 1].maxRange);
    // printf("\n\nla linea è: \n%s\n la parola selezionata è %s\n\n", line, nextWords[0].word);
    strcpy(successive, nextWords[0].word);
    free(nextWords);
    return;
}

void searchAndWriteWord(FILE *fp, int remainingWord, char word[32], int letteraMaiuscola, FILE *outputFile, int wordCount, char line[MAX_LINE_LENGHT])
/* Questa funzione si occupa di leggere il file CSV, selezionando la parola corrente, per poi selezionarne una successiva, in base alla frequenza. La funzione
legge riga per riga il file finché non arriva alla parola corrente, si occupa di scriverla sul file (se seguita da un punto, con la lettera maiuscola) per poi
selezionarne una successiva ripetendo ricorsivamente il procedimento, finché non scriverà tutte le parole */
{
    if (remainingWord == 0) /* Il numero di parole scritte è stato raggiunto, la funzione può terminare la ricorsione */
    {
        return;
    }

    // char line[MAX_LINE_LENGHT]; /* in questo buffer, inserirò la linea corrente letta dal file */

    while (fgets(line, MAX_LINE_LENGHT, fp)) // Leggo il file riga per riga
    {

        char tmp[WORD_LENGHT]; // Controllo la prima parola in questione

        for (int i = 0; i < strlen(line); i++) // Seleziono la prima parola della linea
        {
            if (line[i] == ',')
            {
                tmp[i] = '\0';
                break;
            }
            tmp[i] = line[i];
        }

        if (strcmp(tmp, word) == 0) /* Se la linea letta è relativa alla parola in questione, si avvia il procedimento di selezione della
        successiva, altrimenti leggo la prossima linea*/
        {

            // Scrivo la parola sul file

            if (letteraMaiuscola) /* Questa variabile determina se la parola è o non è all'inizio di una frase, in caso affermativo, la sua
            lettera iniziale deve essere maiuscola */
            {

                if ((int)word[0] < 123 && (int)word[0] > 96) /* Mi accerto che sia una parola con lettere e non un punto*/
                {
                    char copy[32];
                    strcpy(copy, word);
                    char c = copy[0];
                    int a = (int)c;
                    a -= 32;
                    copy[0] = (char)a;

                    fprintf(outputFile, "%s ", copy);
                }
            }
            else
            {
                fprintf(outputFile, "%s ", word);
            }

            if (wordCount % 25 == 0 && wordCount != 0) /* Quando sul file vengono scritte 15 parole, si va a capo*/
            {
                fprintf(outputFile, "\n");
            }

            char successive[WORD_LENGHT];     /* Definisco la variabile in cui andrà il successivo*/
            findSuccessive(line, successive); /* Seleziono la parola successiva randomicamente */

            /* La funzione inserirà dentro la variabile 'successive' la parola successiva */

            fseek(fp, 0, SEEK_SET); /* Torno all'inizio del file */

            int capitalize = 0;
            if (strcmp(word, ".") == 0 || strcmp(word, "!") == 0 || strcmp(word, "?") == 0)
            {
                /* Se la parola attuale è un punto, la prossima avrà la iniziale maiuscola */
                capitalize = 1;
            }

            /* Chiamo la ricorsione sulla parola successiva, facendo attenzione a diminuire di 1 il numero di parole
            rimanenti da scrivere! */
            searchAndWriteWord(fp, remainingWord - 1, successive, capitalize, outputFile, wordCount + 1, line);

            return;
        }
    }
    printf("La parola inserita non è presente nel testo!\n");
    exit(1);
}

int compito2(char *input, char *output, char *numParole, char start[WORD_LENGHT]) /* Esistono due versioni del programma, la prima, opera direttamente sul file (efficente per creare poche
 parole), la seconda, crea un grafo pesato leggendo il CSV, e poi opera sul grafo, l'operazione di creazione del grafo è dispendiosa, ma poi sarà rapido scrivere sul file di output,
 per questo se le parole son poche verrà effettuato il processo con la prima versione, se invece son tante, verrà creato il grafo.*/
{

    clock_t startTime, end;
    startTime = clock();
    double cpu_time_used;
    srand(time(NULL));

    if (atoi(numParole) < 1)
    {
        printf("Inserire un numero di parole valido\n");
        exit(1);
    }

    if (1) // Compito nuovo
    {

        /*Generazione di un punto di partenza casuale*/
        if (strcmp(start, ".") == 0)
        {

            int r = rand() % 3;

            switch (r)
            {
            case 0:
                strcpy(start, ".");
                break;
            case 1:
                strcpy(start, "!");
                break;
            case 2:
                strcpy(start, "?");
                break;
            }
        }

        createGraphFromFile(input);                  /* Leggo il file di input e preparo la struttura del grafo */
        writeOnFile(output, atoi(numParole), start); /* Con una passeggiata sul grafo, scrivo sul file di output */

        end = clock();
        cpu_time_used = ((double)(end - startTime)) / CLOCKS_PER_SEC;

        printf("\nProgramma andato a buon fine, sono state scritte %s parole in %.4f secondi!\n\n", numParole, cpu_time_used);

        exit(0);
    }

    /* Versione 1 del programma (senza grafo) */
    FILE *fp;
    fp = fopen(input, "r"); /*Apro il file di input*/

    FILE *outFile;
    outFile = fopen(output, "w+"); /*Apro il file di output*/

    if (outFile == NULL || fp == NULL)
    {
        printf("Errore nell'apertura dei file.");
        exit(1);
    }

    char line[MAX_LINE_LENGHT]; /* in questo buffer, inserirò la linea corrente letta dal file */

    searchAndWriteWord(fp, atoi(numParole) + 1, start, 1, outFile, 0, line); /*Avvio il procedimento sulla prima parola*/

    fclose(fp);
    fclose(outFile);

    end = clock();
    cpu_time_used = ((double)(end - startTime)) / CLOCKS_PER_SEC;
    printf("Programma andato a buon fine in %.3f secondi!\n", cpu_time_used);
}
#include "../headers/stringOpertion.h" /* contiene funzioni riguardo il controllo delle stringhe*/
#include "../headers/compito1.h"
#include "../headers/wordArray.h"

int **array;

void reSizeArray(int n)
{
    array = realloc(array, sizeof(int *) * n);
    for (int i = 0; i < n - 1; i++)
    {
        array[i] = realloc(array[i], sizeof(int) * (n));
    }
    array[n - 1] = calloc(n, sizeof(int));
}

char **iterateFile(char *fileName, int *numberOfWords);

void FirstInsert(char ***array_parole, int *counter, char word[WORD_LENGHT])
{
    // printf("\n%s", word);
    int k = *counter;
    minuscolaStringa(word);
    if (!checkIfWordInArray(*array_parole, k, word))
    {

        k++;
        *counter = k;

        *array_parole = realloc(*array_parole, k * sizeof(char *));
        (*array_parole)[k - 1] = malloc(WORD_LENGHT);
        strcpy((*array_parole)[k - 1], word);
    }
    else
    {
    }
}

void insert(char ***array_parole, int *counter, char word[WORD_LENGHT], char previous[WORD_LENGHT]) /* Questa funzione prende in input un array di parole (con la sua
 dimensione) ed una parola, ed aggiunge la parola nell'array (esclusivamente se non vi è già presente), questo array simula quindi un SET*/
{
    // printf("%s -> %s\n", previous, word);
    int k = *counter;
    minuscolaStringa(word);

    if (!checkIfWordInArray(*array_parole, k, word))
    {

        k++;
        *counter = k;

        *array_parole = realloc(*array_parole, k * sizeof(char *));
        (*array_parole)[k - 1] = malloc(WORD_LENGHT);
        strcpy((*array_parole)[k - 1], word);

        int currentId = k - 1;
        int previousId = getWordArrayId(*array_parole, k, previous);
        reSizeArray(k);

        array[previousId][k - 1] = 1;
    }
    else
    {
        int currentId = getWordArrayId(*array_parole, k, word);
        int previousId = getWordArrayId(*array_parole, k, previous);
        array[previousId][currentId]++;
    }

    strcpy(previous, word);
}

void c1(char *input, char *output)
{
    int wordsNumber = 0;
    char **arrayParole = iterateFile(input, &wordsNumber);
    // printArray(wordsNumber);
    writeCSVfile(arrayParole, "testOut.csv", wordsNumber); // Scrivo il file
}

char **iterateFile(char *fileName, int *numberOfWords)
{
    int wordsCounter = 0;
    int fileSize;

    char tmp[WORD_LENGHT];

    char *src = putFileInBuffer(fileName, &fileSize);

    int j = 0; /* Sarà l'index relativo della parola letta, se 0, vuol dire che si sta attendendo di leggere la prima lettera di una parola*/

    char **array_parole = malloc(0); // Questo array conterrà tutte le parole lette

    /*Come prima parola, aggiungo il punto*/
    char punto[WORD_LENGHT] = ".";
    char previous[WORD_LENGHT] = ".";

    FirstInsert(&array_parole, &wordsCounter, punto); // Aggiungo la parola all'array
    char c;

    array = malloc(sizeof(int *) * 1);
    array[0] = malloc(sizeof(int) * 1);

    for (int i = 0; i < fileSize; i++)
    {

        if (j > WORD_LENGHT - 1)
        {
            printf("Il testo contiene una parola più lunga di 30 caratteri, oppure contiene una parola, in cui \nè presente un carattere non ASCII standard, tale parola è stata tagliata!\n");
            j = 0;
            continue;
        }

        if (punteggiaturaDaScartare(src[i]))
        {
            // C'è della punteggiatura da scartare
            if (j != 0)
            {
                tmp[j] = '\0'; /* A questo punto 'tmp' contiene la parola letta, quindi aggiungo il simbolo di fine stringa*/

                insert(&array_parole, &wordsCounter, tmp, previous); // Aggiungo la parola all'array

                j = 0;
            }
            continue;
        }

        if ((int)src[i] == 39) // C'è un apostrofo
        {
            if (j != 0)
            {
                tmp[j] = src[i];
                tmp[j + 1] = '\0';

                insert(&array_parole, &wordsCounter, tmp, previous); // Aggiungo la parola all'array
                j = 0;
            }
            continue;
        }

        if (src[i] == '!' || src[i] == '.' || src[i] == '?') // Gestione dei punti
        {
            if (j == 0)
            {
                /*trovato punto da solo*/
                tmp[0] = src[i];
                tmp[1] = '\0'; /* A questo punto 'tmp' contiene la parola letta, quindi aggiungo il simbolo di fine stringa*/

                insert(&array_parole, &wordsCounter, tmp, previous); // Aggiungo la parola all'array
                continue;
            }
            else
            {
                // La parola attuale va salvata nel buffer
                tmp[j] = '\0'; /* A questo punto 'tmp' contiene la parola letta, quindi aggiungo il simbolo di fine stringa*/

                insert(&array_parole, &wordsCounter, tmp, previous); // Aggiungo la parola all'array

                j = 0;

                // Ora salvo il punto
                tmp[0] = src[i];
                tmp[1] = '\0';
                insert(&array_parole, &wordsCounter, tmp, previous); // Aggiungo la parola all'array
                continue;
            }
        }
        if (((int)src[i] >= 0 && (int)src[i] <= 32)) // Viene letto uno spazio o un accapo
        {
            if (j != 0) /*è stato letto uno spazio e c'è una parola nel buffer, va salvata la parola e svuotato il buffer*/
            {
                // parola finita
                tmp[j] = '\0'; /* A questo punto 'tmp' contiene la parola letta, quindi aggiungo il simbolo di fine stringa*/

                insert(&array_parole, &wordsCounter, tmp, previous); // Aggiungo la parola all'array

                j = 0;
            }
        }
        else /* La parola continua, metto il carattere nel buffer 'tmp'*/
        {
            tmp[j] = src[i];
            j++;
        }
    }

    // fclose(fp);
    //  FINE DEL CICLO FOR

    if (j != 0) /*Se l'ultima lettera è un carattere, la parola nel buffer non è stata ancora aggiunta all'array*/
    {
        tmp[j] = '\0'; /* A questo punto 'tmp' contiene la parola letta, quindi aggiungo il simbolo di fine stringa*/

        insert(&array_parole, &wordsCounter, tmp, previous); // Aggiungo la parola all'array
    }

    if (strcmp(tmp, ".") != 0 && wordsCounter >= 2)
    {
        /*Se l'ultima parola non è il punto, bisogna collegare la prima e l'ultima parola per evitare situazioni
        in cui il programma non sa quale altra parola usare*/
        array[wordsCounter - 1][1]++;
    }
    *numberOfWords = wordsCounter;

    return array_parole;
}

void printArray(int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%d  ", array[i][j]);
        }
        printf("\n");
    }
}

void writeCSVfile(char **array_parole, char *fileName, int n) /*Questa funzione legge la matrice e stampa sul file*/
{
    FILE *fp;

    fp = fopen(fileName, "w+"); // Creo il puntatore al file

    for (int i = 0; i < n; i++) /*Scorro ogni riga della matrice*/
    {
        // Faccio la somma della riga attuale
        int rawSum = 0;             /* Questa variabile salverà per ogni riga la somma delle occorrenze delle parole a seguito di quella controllata correntemente*/
        for (int j = 0; j < n; j++) /*Scorro ogni riga della matrice*/
        {
            rawSum += array[i][j];
        }

        fprintf(fp, "%s", array_parole[i]); // Scrivo la parola corrente

        for (int j = 0; j < n; j++) /*Scorro le parole che la seguono*/
        {
            if (array[i][j] != 0) /*Se le occorrenze sono diverse da zero, devo scrivere la frequenza*/
            {
                float f = (float)array[i][j] / (float)rawSum;
                fprintf(fp, ",%s,%f", array_parole[j], f);
            }
        }
        if (i != n - 1)
        {

            fprintf(fp, "\n"); // Vado a capo nel file CSV
        }
    }

    fclose(fp);
}
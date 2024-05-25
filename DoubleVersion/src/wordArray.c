/* Questo file si occupa di gestire un array di parole di lunghezza massima 62 byte.
Il termine array, è in realtà improprio, in quanto tale struttura sarà
un insieme.*/
#include "../headers/wordArray.h"
#include "../headers/hashTable.h"
#include "../headers/ioOperation.h"

struct WordElement
{
    char word[WORD_LENGHT];
    struct WordElement *next;
    int id;
};

struct WordElement *buckets[HASHSIZE] = {NULL}; /* Struttura che conterrà l'hashTable */

/*Verrà utilizzata una hash table per controllare in tempo costante se una parola è già stata considerata o no*/

int collisionCounter = 0; /*Calcolo delle collisioni nell'hashtable durante l'esecuzione */

int *matrix;
int n;

int checkIfWordInHashMap(char word[WORD_LENGHT]) /* controlla se nell'hashTable vi è già la parola data in input*/
{

    int hashVal = hash(word);
    if (buckets[hashVal] == NULL)
    {
        return 0;
    }
    else
    {
        if (strcmp(buckets[hashVal]->word, word) == 0)
        {
            return 1;
        }
        struct WordElement *tmp = buckets[hashVal];
        while (tmp->next != NULL)
        {
            tmp = tmp->next;
            if (strcmp(tmp->word, word) == 0)
            {
                return 1;
            }
        }
    }
    return 0;
}

int getWordIdFromHashMap(char word[WORD_LENGHT]) /* restituisce l'ID relativo all' 'array_parole', contenuto nel record dentro l'hashTable */
{

    int hashVal = hash(word);
    if (buckets[hashVal] == NULL)
    {
        printf("errore su %s indice : %d\n", word, hashVal);
        exit(1);
    }
    else
    {
        if (strcmp(buckets[hashVal]->word, word) == 0)
        {
            return buckets[hashVal]->id;
        }
        struct WordElement *tmp = buckets[hashVal];
        while (tmp->next != NULL)
        {
            tmp = tmp->next;
            if (strcmp(tmp->word, word) == 0)
            {
                return tmp->id;
            }
        }
    }
}

void freeHashMap()
{
    for (int i = 0; i < HASHSIZE; i++)
    {
        if (buckets[i] != NULL)
        {
            free(buckets[i]);
        }
    }
}

void freeMatrix()
{
    free(matrix);
}

void printHashMap()
{
    int wordsInHashMap = 0;
    for (int i = 0; i < HASHSIZE; i++)
    {
        int wordInBuckets = 0;
        if (buckets[i] != NULL)
        {
            printf("%d : %s", i, buckets[i]->word);
            wordsInHashMap++;
            wordInBuckets++;
            if (buckets[i]->next != NULL)
            {
                struct WordElement *tmp = buckets[i];
                while (tmp->next != NULL)
                {
                    tmp = tmp->next;
                    wordsInHashMap++;
                    wordInBuckets++;
                    printf("--> %s", tmp->word);
                }
            }
            printf("\n");
        }
    }
    printf("parole distinte hasmap : %d\n", wordsInHashMap);
}

void addWord(char ***array_parole, int *counter, char word[WORD_LENGHT]) /* Questa funzione prende in input un array di parole (con la sua
dimensione) ed una parola, ed aggiunge la parola nell'array (esclusivamente se non vi è già presente), questo array simula quindi un SET, facendo uso dell'HASHTABLE per
eseguire il controllo in tempo costante*/
{
    // printf("%s\n", word);
    int k = *counter;
    lowerCase(word);
    if (!checkIfWordInHashMap(word)) /*Controlla se la parola non è già nell'array*/
    {

        k++;
        *counter = k;

        /*Aggiungo la parola dentro l'hashMap */
        int hashVal = hash(word);
        if (buckets[hashVal] == NULL)
        {
            buckets[hashVal] = malloc(sizeof(struct WordElement));
            buckets[hashVal]->id = k - 1;
            buckets[hashVal]->next = NULL;
            strcpy(buckets[hashVal]->word, word);
        }
        else /* Vi è una collisioni, devo scorrere il bucket per poi aggiungere la parola in fondo ad esso*/
        {
            collisionCounter++;
            struct WordElement *tmp = buckets[hashVal];
            while (tmp->next != NULL)
            {
                tmp = tmp->next;
            }
            tmp->next = malloc(sizeof(struct WordElement));
            (tmp->next)->id = k - 1;
            (tmp->next)->next = NULL;
            strcpy((tmp->next)->word, word);
        }

        // Aggiungi all'array delle parole
        *array_parole = realloc(*array_parole, k * sizeof(char *));
        (*array_parole)[k - 1] = malloc(WORD_LENGHT);
        strcpy((*array_parole)[k - 1], word);
    }
}

void printMatrix(int wordsCounter, int f) /*per printare a schermo, o su un file, la matrice creata*/
{
    FILE *fp1 = fopen("matrix1.txt", "w+");
    FILE *fp2 = fopen("matrix2.txt", "w+");
    for (int i = 0; i < wordsCounter; i++)
    {
        for (int j = 0; j < wordsCounter; j++)
        {
            if (f)
            {
                fprintf(fp1, "| %d |", matrix[i * wordsCounter + j]);
            }
            else
            {
                printf("%d  ", matrix[i * wordsCounter + j]);
            }
        }
        if (f)
        {
            fprintf(fp1, "\n");
        }
        else
        {
            printf("\n");
        }
    }
}

void initMatrix(int words) /* Inizializza la matrice che verrà utilizzata per scrivere il file CSV */
{
    n = words;

    matrix = (int *)malloc(n * n * sizeof(int));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matrix[i * words + j] = 0;
        }
    }
}

int punteggiaturaDaScartare(char c)
{
    if (((int)c >= 34 && (int)c <= 38) || ((int)c >= 40 && (int)c <= 45) ||
        ((int)c >= 58 && (int)c <= 62) || ((int)c >= 91 && (int)c <= 96) ||
        ((int)c >= 123 && (int)c <= 126) || c == '/')
    {
        return 1;
    }
    return 0;
}

void fillMatrixWithWord(char *fileName, char **array_parole, int wordsCounter)
{
    /* NOTA IMPORTANTE : Questa funzione risulta nella sua struttura identica alla funzione 'getWordFromFile' vista nel file compito1.c
    si occupa di leggere nuovamente tutto il file, questa volta però, piuttosto che salvare le parole, riempie i campi della matrice
    già creata, la cui dimensione viene calcolata nella già citata 'getWordFromFile'.

    DOMANDA : Perché non creare dinamicamente e riempire la matrice in 'getWordFromFile', evitando di leggere l'intero file input per 2 volte?

    RISPOSTA : Ogni volta che viene modificata la matrice per aumentare la dimensione di ogni riga, va eseguita una re-alloc su ogni singola riga,
    il costo computazionale diventa troppo grande e risulta inefficente, è stato testato che, è molto più efficente leggere per due volte di fila il file,
    calcolando il numero di parole distinte per poi creare la matrice con la dimensione finale fin dall'inizio.
    */

    int fileSize;
    char *src = putFileInBuffer(fileName, &fileSize);
    int j = 0;
    char currentWord[WORD_LENGHT];
    char previousWord[WORD_LENGHT] = ".\0";

    int currentId;
    int previousId;

    for (int i = 0; i < fileSize; i++)
    {
        if (j > WORD_LENGHT - 1)
        {
            j = 0;
            continue;
        }
        if (punteggiaturaDaScartare(src[i]))
        {
            if (j != 0)
            {

                currentWord[j] = '\0';
                lowerCase(currentWord);
                currentId = getWordIdFromHashMap(currentWord);
                previousId = getWordIdFromHashMap(previousWord);

                matrix[previousId * n + currentId]++;
                strcpy(previousWord, currentWord);
                j = 0;
            }
            continue;
        }
        if ((int)src[i] == 39)
        {
            if (j != 0)
            {
                currentWord[j] = src[i];
                currentWord[j + 1] = '\0';
                lowerCase(currentWord);

                currentId = getWordIdFromHashMap(currentWord);
                previousId = getWordIdFromHashMap(previousWord);

                matrix[previousId * n + currentId]++;
                strcpy(previousWord, currentWord);

                j = 0;
            }
            continue;
        }
        if (src[i] == '!' || src[i] == '.' || src[i] == '?') // Gestione dei punti
        {
            if (j == 0)
            {
                currentWord[0] = src[i];
                currentWord[1] = '\0';

                currentId = getWordIdFromHashMap(currentWord);
                previousId = getWordIdFromHashMap(previousWord);

                matrix[previousId * n + currentId]++;
                strcpy(previousWord, currentWord);

                continue;
            }
            else
            {
                currentWord[j] = '\0';
                lowerCase(currentWord);

                currentId = getWordIdFromHashMap(currentWord);
                previousId = getWordIdFromHashMap(previousWord);

                matrix[previousId * n + currentId]++;
                strcpy(previousWord, currentWord);

                j = 0;

                currentWord[0] = src[i];
                currentWord[1] = '\0';

                currentId = getWordIdFromHashMap(currentWord);
                previousId = getWordIdFromHashMap(previousWord);

                matrix[previousId * n + currentId]++;
                strcpy(previousWord, currentWord);

                continue;
            }
        }

        if (((int)src[i] >= 0 && (int)src[i] <= 32))
        {
            if (j != 0) /*è stato letto uno spazio e c'è una parola nel buffer, va salvata la parola e svuotato il buffer*/
            {

                currentWord[j] = '\0'; /* A questo punto 'tmp' contiene la parola letta, quindi aggiungo il simbolo di fine stringa*/
                lowerCase(currentWord);

                currentId = getWordIdFromHashMap(currentWord);

                previousId = getWordIdFromHashMap(previousWord);

                matrix[previousId * n + currentId]++;

                strcpy(previousWord, currentWord);
                j = 0;
            }
        }
        else /* La parola continua, metto il carattere nel buffer 'tmp'*/
        {
            currentWord[j] = src[i];
            j++;
        }
    } // Fine del ciclo 'for'

    if (j != 0) /*Se l'ultima lettera è un carattere, la parola nel buffer non è stata ancora aggiunta all'array*/
    {
        currentWord[j] = '\0'; /* A questo punto 'tmp' contiene la parola letta, quindi aggiungo il simbolo di fine stringa*/
        lowerCase(currentWord);

        currentId = getWordIdFromHashMap(currentWord);
        previousId = getWordIdFromHashMap(previousWord);

        matrix[previousId * n + currentId]++;
    }

    if ((strcmp(currentWord, ".") != 0 || strcmp(currentWord, "!") != 0 || strcmp(currentWord, "?") != 0) && wordsCounter >= 2)
    {
        /*Se l'ultima parola non è il punto, bisogna collegare la prima e l'ultima parola per evitare situazioni
        in cui il programma non sa quale altra parola usare*/

        matrix[(wordsCounter - 1) * n + 1]++; // Prima del punto, c'è l'ultima parola
    }
}

void printFrequence(char **array_parole, char *fileName) /*Questa funzione legge la matrice e stampa sul file*/
{
    FILE *fp;

    fp = fopen(fileName, "w+"); // Creo il puntatore al file

    if (fp == NULL)
    {
        printf("il percorso %s non è stato trovato\n", fileName);
        exit(1);
    }

    for (int i = 0; i < n; i++) /*Scorro ogni riga della matrice*/
    {
        // Faccio la somma della riga attuale
        int rawSum = 0;             /* Questa variabile salverà per ogni riga la somma delle occorrenze delle parole a seguito di quella controllata correntemente*/
        for (int j = 0; j < n; j++) /*Scorro ogni riga della matrice*/
        {
            rawSum += matrix[i * n + j];
        }

        fprintf(fp, "%s", array_parole[i]); // Scrivo la parola corrente

        for (int j = 0; j < n; j++) /*Scorro le parole che la seguono*/
        {
            if (matrix[i * n + j] != 0) /*Se le occorrenze sono diverse da zero, devo scrivere la frequenza*/
            {
                float f = (float)matrix[i * n + j] / (float)rawSum;
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

void printFrequence_multi(char **array_parole, char *fileName, int out, int end) /*Questa funzione legge la matrice ed invia al processo che si occupa del file CSV le righe da scrivere*/
{
    char accapo[WORD_LENGHT] = "\n";
    for (int i = 0; i < n; i++) /*Scorro ogni riga della matrice*/
    {
        // Faccio la somma della riga attuale
        int rawSum = 0;             /* Questa variabile salverà per ogni riga la somma delle occorrenze delle parole a seguito di quella controllata correntemente*/
        for (int j = 0; j < n; j++) /*Scorro ogni riga della matrice*/
        {
            rawSum += matrix[i * n + j];
        }

        write(out, array_parole[i], WORD_LENGHT);
        for (int j = 0; j < n; j++) /*Scorro le parole che la seguono*/
        {
            if (matrix[i * n + j] != 0) /*Se le occorrenze sono diverse da zero, devo scrivere la frequenza*/
            {
                float f = (float)matrix[i * n + j] / (float)rawSum;

                char tmp[100];
                sprintf(tmp, ",%s,%f", array_parole[j], f);
                write(out, tmp, WORD_LENGHT);
            }
        }
        if (i != n - 1)
        {
            write(out, accapo, WORD_LENGHT);
        }
    }
    char endBuf[10] = "finito";
    write(end, endBuf, 10);
}

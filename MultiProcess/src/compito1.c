#include "../headers/compito1.h"
#include "../headers/ioOperation.h"
#include "../headers/wordArray.h"
#include <sys/wait.h>
char **getWordFromFile(char *fileName, int *numberOfWords, int inputPipe, int endPipe) /* Questa funzione prende come input il nome di un file, e restituisce
  un array di stringhe contenete le parole lette nel file.  prende come input anche un intero, che verrà
  aggiornato e conterrà il numero di parole*/
{
    int wordsCounter = 0;
    int fileSize;
    char tmp[WORD_LENGHT];

    char *src = putFileInBuffer(fileName, &fileSize);

    int j = 0; /* Sarà l'index relativo della parola letta, se 0, vuol dire che si sta attendendo di leggere la prima lettera di una parola*/

    char **array_parole = malloc(0); // Questo array conterrà tutte le parole lette

    /*Come prima parola, aggiungo il punto*/
    char punto[WORD_LENGHT] = ".";

    // addWord(&array_parole, &wordsCounter, punto); // Aggiungo la parola all'array
    write(inputPipe, punto, WORD_LENGHT);
    char c;

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
                write(inputPipe, tmp, WORD_LENGHT);
                // addWord(&array_parole, &wordsCounter, tmp); // Aggiungo la parola all'array

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
                write(inputPipe, tmp, WORD_LENGHT);
                // addWord(&array_parole, &wordsCounter, tmp); // Aggiungo la parola all'array
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
                write(inputPipe, tmp, WORD_LENGHT);
                // addWord(&array_parole, &wordsCounter, tmp); // Aggiungo la parola all'array
                continue;
            }
            else
            {
                // La parola attuale va salvata nel buffer
                tmp[j] = '\0'; /* A questo punto 'tmp' contiene la parola letta, quindi aggiungo il simbolo di fine stringa*/
                write(inputPipe, tmp, WORD_LENGHT);
                // addWord(&array_parole, &wordsCounter, tmp); // Aggiungo la parola all'array

                j = 0;

                // Ora salvo il punto
                tmp[0] = src[i];
                tmp[1] = '\0';
                write(inputPipe, tmp, WORD_LENGHT);
                // addWord(&array_parole, &wordsCounter, tmp); // Aggiungo la parola all'array
                continue;
            }
        }
        if (((int)src[i] >= 0 && (int)src[i] <= 32)) // Viene letto uno spazio o un accapo
        {
            if (j != 0) /*è stato letto uno spazio e c'è una parola nel buffer, va salvata la parola e svuotato il buffer*/
            {
                // parola finita
                tmp[j] = '\0'; /* A questo punto 'tmp' contiene la parola letta, quindi aggiungo il simbolo di fine stringa*/
                               // printf("scrivo %s\n", tmp);
                write(inputPipe, tmp, WORD_LENGHT);

                // addWord(&array_parole, &wordsCounter, tmp); // Aggiungo la parola all'array

                j = 0;
            }
        }
        else /* La parola continua, metto il carattere nel buffer 'tmp'*/
        {
            tmp[j] = src[i];
            j++;
        }
    }

    //  FINE DEL CICLO FOR

    if (j != 0) /*Se l'ultima lettera è un carattere, la parola nel buffer non è stata ancora aggiunta all'array*/
    {
        tmp[j] = '\0'; /* A questo punto 'tmp' contiene la parola letta, quindi aggiungo il simbolo di fine stringa*/

        write(inputPipe, tmp, WORD_LENGHT);
        // addWord(&array_parole, &wordsCounter, tmp); // Aggiungo la parola all'array
    }
    char us[1] = "a";
    write(endPipe, us, 1);

    *numberOfWords = wordsCounter;
    return array_parole;
}

void compito1(char *input, char *output)
{
    pid_t pid;
    int inputPipe[2];
    int endPipe[2];
    pipe(inputPipe);
    pipe(endPipe);

    fcntl(endPipe[0], F_SETFL, O_NONBLOCK);
    fcntl(inputPipe[0], F_SETFL, O_NONBLOCK);

    char usless[1];
    char buf[WORD_LENGHT];

    pid = fork();
    if (pid == 0)
    {
        int wordsCounter = 0;
        char **array_parole = malloc(0); // Questo array conterrà tutte le parole lette

        while (1)
        {
            // close(inputPipe[1]);
            //  Child
            if (read(inputPipe[0], buf, WORD_LENGHT) == -1)
            {
                if (read(endPipe[0], usless, 1) != -1)
                {
                    break;
                }
                else
                {
                    continue;
                }
            }
            addWord(&array_parole, &wordsCounter, buf); // Aggiungo la parola all'array
        }
        // printf("fine ciclo di ascolto\n");
        initMatrix(wordsCounter); // Inizializzo la matrice con numero di righe e colonne identico al numero delle parole distinte lette nel file

        fillMatrixWithWord(input, array_parole, wordsCounter); // Riempio i campi della matrice

        pid_t pid2 = fork();

        if (pid2 != 0)
        {

            printFrequence(array_parole, output); // Scrivo il file

            //  Libero la memoria
            freeHashMap();
            freeMatrix();
            free(array_parole);
            printf("terminato processo che scrive il CSV\n");
            printf("\nparole distinte nel testo : %d\n", wordsCounter); /* Per scrivere a schermo statistiche relative all'HashTable */
            exit(0);
        }
        else
        {
            printf("terminato processo che crea la struttura\n");
            exit(0);
        }
    }
    else
    {
        int wordsCounter = 0;
        getWordFromFile(input, &wordsCounter, inputPipe[1], endPipe[1]);
        printf("terminato processo che legge il file di testo\n");
        int status;
        wait(&status);
        exit(0);
    }
}

#include "../headers/rootHeader.h"
#include "../headers/compito2.h"
#include "../headers/compito1.h"

int main(int argc, char *argv[])
{

    char *inputFile = NULL;
    char *outputFile = NULL;
    int codCompito = 0;
    int numParole = 0;
    int c;
    char start[WORD_LENGHT] = ".";
    while ((c = getopt(argc, argv, "12n:i:o:s:")) != -1)
    {
        switch (c)
        {
        case 's':
            strcpy(start, optarg);
            break;
        case '1':
            if (codCompito == 2)
            {
                printf("Non è possibile svolgere entrambi i compiti.\n");
                exit(1);
            }
            codCompito = 1;
            break;
        case '2':
            if (codCompito == 1)
            {
                printf("Non è possibile svolgere entrambi i compiti.\n");
                exit(1);
            }
            codCompito = 2;
            break;
        case 'n':
            numParole = atoi(optarg);
            break;
        case 'o':
            outputFile = malloc(strlen(optarg) + 1);
            strcpy(outputFile, optarg);
            break;
        case 'i':

            inputFile = malloc(strlen(optarg) + 1);
            strcpy(inputFile, optarg);

            break;
        default:
            break;
        }
    }
    if (codCompito == 0)
    {
        printf("è necessario specificare se si vuole eseguire il compito 1 o il compito 2.\n");
        exit(1);
    }
    if (numParole <= 0 && codCompito == 2)
    {
        printf("inserire un numero di parole valido!\n");
        exit(1);
    }
    if (inputFile == NULL || outputFile == NULL)
    {
        printf("è necessario specificare un file in input ed un file in output\n");
        exit(1);
    }
    if (codCompito == 1)
    {
        compito1(inputFile, outputFile);
    }
    else
    {
        char punto[WORD_LENGHT] = ".";
        char str[50];
        sprintf(str, "%d", numParole);
        compito2(inputFile, outputFile, str, start);
    }
    exit(0);
}

int randomStart(char *input) /* Questa funzione seleziona un punto casuale dalla quale partire fra '?',  '.'  e '!' */
{
}


#include "../headers/rootHeader.h"
#include "../headers/compito2.h"
#include "../headers/compito1.h"

int main(int argc, char *argv[])
{
    if (0)
    {
        char *inputFile = NULL;
        char *outputFile = NULL;
        int codCompito = 0;
        int numParole = 0;
        int c;
        while ((c = getopt(argc, argv, "12n:i:o:")) != -1)
        {
            switch (c)
            {
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
                outputFile = malloc(strlen(optarg));
                strcpy(outputFile, optarg);
                break;
            case 'i':
                inputFile = malloc(strlen(optarg));
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
            // compito2(inputFile, outputFile, numParole, punto);
        }
        exit(0);
    }

    if (argc < 4) // è necessario passare almeno un file in input ed uno in output
    {
        printf("è necessario fornire un file input, un file output,\nbisogna anche specificare se si vuole eseguire il compito 1 o 2.\n");
        exit(1);
    }

    if (strcmp(argv[3], "1") == 0)
    {
        compito1(argv[1], argv[2]);
    }

    else if (strcmp(argv[3], "2") == 0)
    {
        if (argc < 5)
        {
            printf("è necessario fornire un numero di parole da generare!\n");
            exit(1);
        }

        char punto[WORD_LENGHT] = ".";
        if (argc < 6)
        {
            compito2(argv[2], argv[1], argv[4], punto);
        }
        else
        {
            compito2(argv[2], argv[1], argv[4], argv[5]);
        }
    }
    else
    {
        printf("Bisogna indicare se eseguire il compito 1 oppure il compito 2.\nIl codice %s non è valido per nessun compito.", argv[3]);
        exit(1);
    }
}

int randomStart(char *input) /* Questa funzione seleziona un punto casuale dalla quale partire fra '?',  '.'  e '!' */
{
}


#include "../headers/rootHeader.h"
#include "../headers/compito2.h"
#include "../headers/compito1.h"
void printMatrice(int **array, int n);
void testMatrice2d(int n)
{

    int **array;

    array = malloc(sizeof(int *) * n);

    for (int i = 0; i < n; i++)
    {
        array[i] = malloc(sizeof(int) * n);
    }

    array[0][1] = 4;
    array[1][1] = 3;
    array[0][0] = 9;
    array[1][0] = 2;
    array[4][5] = 99;

    printMatrice(array, n);
    printf("\n");

    array = realloc(array, sizeof(int *) * (n + 1));
    for (int i = 0; i < n; i++)
    {
        array[i] = realloc(array[i], sizeof(int) * (n + 1));
    }
    array[n] = calloc((n + 1), sizeof(int));

    printMatrice(array, n + 1);
    exit(0);
}

void printMatrice(int **array, int n)
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

int main(int argc, char *argv[])
{
    // testMatrice2d(6);
    //  testGraph();
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

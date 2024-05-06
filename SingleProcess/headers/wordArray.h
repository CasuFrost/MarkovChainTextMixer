/* Questo file si occupa di gestire un array di parole di lunghezza massima 30 byte.
Il termine array, è in realtà improprio, in quanto tale struttura sarà
un insieme.*/
#include "ioOperation.h"
#include "stringOpertion.h"

void freeMatrix();

int checkIfWordInHashMap(char word[WORD_LENGHT]);

int getWordIdFromHashMap(char word[WORD_LENGHT]);

void addWord(char ***array_parole, int *counter, char word[WORD_LENGHT]);

void printMatrix(int wordsCounter, int f);

void initMatrix(int words);

/* matrix[i,j]=x implica che
array_parole[j] si trova dopo array_parole[i] con una frequenza x
*/

int punteggiaturaDaScartare(char c);

void fillMatrixWithWord(char *fileName, char **array_parole, int wordsCounter);

void printFrequence(char **array_parole, char *fileName);
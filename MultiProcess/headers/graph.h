#include "rootHeader.h"

struct Node;

struct Edge;

struct Node *getGraph();

void addNearId(int nodeFrom, int nodeTo, float w);

void addNear(struct Node *nodeFrom, struct Node *nodeTo, float w);

void createEdge(struct Node *node1, struct Node *node2, float w);

void printGraph();

void createNode(char word[WORD_LENGHT]);

void createEdgeFromWord(int id1, char word2[WORD_LENGHT], float w);

void createGraphFromFile(char *fileName);

int selectNearId(int id);

void writeOnFile(char *fileName, int words, char start[WORD_LENGHT]);

void freeGraphStructures();
#ifndef __SERIABILIDADE__
#define __SERIABILIDADE__
#include "escalona.h"

typedef struct node {
    int index;
    struct node *neighbours;
    int visited;
} node;

int DFS(node *G, node *v, int n_nodes);    // retorna 0 se há ciclo, 1 caso contrário
int newNode(node *G, int tam, int identificador);
void criaAresta(node *G, int Ti, int Tj, int n_nodes);
char *seriabilidade(transacao *transacoes, int tam);

#endif
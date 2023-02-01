#ifndef __SERIABILIDADE__
#define __SERIABILIDADE__
#include "escalona.h"

int DFS(node *G, node *v, int n_nodes);    // retorna 0 se há ciclo, 1 caso contrário
char *seriabilidade(transacao *transacoes, int tam);

#endif
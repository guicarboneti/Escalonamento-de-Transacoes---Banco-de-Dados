#include <stdlib.h>
#include <stdio.h>
#include "seriabilidade.h"
#include "escalona.h"

int DFS(node *G, node *v, int n_nodes) {    // retorna 0 se há ciclo, 1 caso contrário
    int i;
    v->visited = 1;
    for (i=0; i<n_nodes-1; i++)
        if (v->neighbours[i].index == 0)
            v->visited = 0;
        else if (v->neighbours[i].visited)
            return 0;
        else if (!DFS(G, &v->neighbours[i], n_nodes))
            return 0;

    return 1;
}

void criaAresta(node *list, int Ti, int Tj, int n_nodes) {
    int i, j;
    node nodej;
    for (i=0;i<n_nodes;i++)
        if (list[i].index == Tj)
            nodej = list[i];
    for (i=0; i<n_nodes; i++)
        if (list[i].index == Ti)
            for (j=0; j<n_nodes; j++)
                if (list[i].neighbours[j].index == 0) {
                    list[i].neighbours[j] = nodej;
                    break;
                }
}

int newNode(node *list, int tam, int identificador) {
    int i;
    for(i=0; i<tam; i++)
        if (list[i].index == identificador)
            return 0;
    return 1;
}

char *seriabilidade(transacao *transacoes, int tam) {
    int i, j, m, n_nodes=0;
    node *list_nodes = (node*) malloc(tam*sizeof(node));
    for(i=0; i<tam; i++) {
        list_nodes[i].index = 0;
        list_nodes[i].visited = 0;
        list_nodes[i].neighbours = (node*) malloc(tam*sizeof(node));
    }

     // cria um nó para cada T do escalonamento
    for (i=0; i<tam; i++)
        if (newNode(list_nodes, tam, transacoes[i].identificador))
            list_nodes[n_nodes++].index = transacoes[i].identificador;

    // cria aresta Ti -> Tj para cada r(x) em Tj depois de w(x) em Ti
    for (i=0; i<n_nodes; i++) {     // percorre os nós T
        int Ti = list_nodes[i].index;
        for (j=0; j<tam-1; j++)     // percorre a lista de transacoes
            if (transacoes[j].identificador==Ti && transacoes[j].operacao=='W')
                for (m=j+1; m<tam; m++)
                    if (transacoes[m].identificador != Ti && transacoes[m].operacao=='R' && transacoes[j].atributo == transacoes[m].atributo)
                        criaAresta(list_nodes, Ti, transacoes[m].identificador, n_nodes);
    }

    // cria aresta Ti -> Tj para cada w(x) em Tj depois de r(x) em Ti
    for (i=0; i<n_nodes; i++) {     // percorre os nós T
        int Ti = list_nodes[i].index;
        for (j=0; j<tam-1; j++)     // percorre a lista de transacoes
            if (transacoes[j].identificador==Ti && transacoes[j].operacao=='R')
                for (m=j+1; m<tam; m++)
                    if (transacoes[m].identificador != Ti && transacoes[m].operacao=='W' && transacoes[j].atributo == transacoes[m].atributo)
                        criaAresta(list_nodes, Ti, transacoes[m].identificador, n_nodes);
    }

    // cria aresta Ti -> Tj para cada w(x) em Tj depois de w(x) em Ti
    for (i=0; i<n_nodes; i++) {     // percorre os nós T
        int Ti = list_nodes[i].index;
        for (j=0; j<tam-1; j++)     // percorre a lista de transacoes
            if (transacoes[j].identificador==Ti && transacoes[j].operacao=='W')
                for (m=j+1; m<tam; m++)
                    if (transacoes[m].identificador != Ti && transacoes[m].operacao=='W' && transacoes[j].atributo == transacoes[m].atributo)
                        criaAresta(list_nodes, Ti, transacoes[m].identificador, n_nodes);
    }

    for (i=0; i<n_nodes; i++)
        if (!DFS(list_nodes, &list_nodes[i], n_nodes))    // busca em profundidade para detectar ciclo 
            return "NS";   // não é serial
    return "SS";   // é serial
}
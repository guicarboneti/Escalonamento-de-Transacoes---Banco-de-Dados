#include <stdio.h>
#include <stdlib.h>
#define TAM_BUFFER 20

typedef struct transacao {
    int tempo_chegada;
    int identificador;
    char operacao;
    char atributo;

} transacao;

typedef struct node {
    int index;
    int *points_to;
} node;

void readList(transacao *list, int *tam) {
    int i;
    char line[TAM_BUFFER];
    for (i=0; i<5;i++) {
        fgets(line, TAM_BUFFER, stdin);
        sscanf(line, "%d %d %c %c", &list[i].tempo_chegada, &list[i].identificador, &list[i].operacao, &list[i].atributo);
        (*tam)++;
    }
}

void printList(transacao *list, int tam) {
    int i;
    for (i=0; i<tam; i++){
        printf("%d", list[i].tempo_chegada);
        printf(" - %d", list[i].identificador);
        printf(" - %c", list[i].operacao);
        printf(" - %c\n", list[i].atributo);
    }
}

int newNode(node *list, int tam, int identificador) {
    int i;
    for(i=0; i<tam; i++) {
        if (list[i].index == identificador)
            return 0;
    }
    return 1;
}

int detectCycle() {
    return 1;
}

void criaAresta(node *list, int Ti, int Tj, int tam) {
    int i, j;
    for (i=0; i<tam; i++) {
        if (list[i].index == Ti) {
            for (j=0; j<tam; j++) {
                if (list[i].points_to[j] == 0)
                    list[i].points_to[j] = Tj;
            }
        }
    }
}

int seriabilidade(transacao *transacoes, int tam) {
    int i, j, m;
    node *list_nodes = (node*) malloc(tam*sizeof(node));
    for(i=0; i<tam; i++)
        list_nodes[i].points_to = (int*) calloc(tam,sizeof(int));

     // cria um nó para cada T do escalonamento
    for (i=0; i<tam; i++) {
        if (newNode(list_nodes, tam, transacoes[i].identificador))
            list_nodes[i].index = transacoes[i].identificador;
    }

    // cria aresta Ti -> Tj para cada r(x) em Tj depois de w(x) em Ti
    for (i=0; i<tam; i++) {     // percorre os nós T
        int Ti = list_nodes[i].index;
        if (Ti == 0)
            break;
        
        for (j=0; j<tam-1; j++) {     // percorre a lista de transacoes
            if (transacoes[j].identificador==Ti && transacoes[j].operacao=='R') {
                for (m=j+1; m<tam; m++) {
                    if (transacoes[m].identificador != Ti && transacoes[m].operacao=='W' && transacoes[j].atributo == transacoes[m].atributo)
                        criaAresta(list_nodes, Ti, transacoes[m].identificador, tam);
                }
            }
        }
    }

    for (i=0;i<tam;i++) {
        if (list_nodes[i].index==0)
            break;
        printf("T%d:\n", list_nodes[i].index);
        for (j=0;j<tam;j++) {
            printf("%d",  list_nodes[i].points_to[j]);
        }
        printf("\n");
    }

    if (detectCycle())    // se há ciclo no grafo
        return 1;   // é serial
    else
        return 0;   // não é serial
}

int main() {
    int i, tam_list=0;
    transacao *lista_transacoes = malloc (TAM_BUFFER * sizeof(transacao)) ;
    readList(lista_transacoes, &tam_list);
    if (seriabilidade(lista_transacoes, tam_list))
        printf("É serial\n");
    else
        printf("Não é serial\n");

    printList(lista_transacoes, tam_list);
    return 0;
}
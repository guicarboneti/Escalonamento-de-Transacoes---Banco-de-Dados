#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAM_BUFFER 20

typedef struct transacao {
    int tempo_chegada;
    int identificador;
    char operacao;
    char atributo;

} transacao;

typedef struct node {
    int index;
    struct node *neighbours;
} node;

void readList(transacao *list, int *tam) {
    int i;
    char line[TAM_BUFFER];
    for (i=0; ;i++) {
        if (!fgets(line, TAM_BUFFER, stdin))
            break;
        sscanf(line, "%d %d %c %c", &list[i].tempo_chegada, &list[i].identificador, &list[i].operacao, &list[i].atributo);
        (*tam)++;
    }
}

char *printList(transacao *list, int tam) {
    int i;
    char *str, c;
    str = malloc(tam*sizeof(char));
    for (i=0; i<tam; i++){
        c = list[i].identificador+'0';
        strcat(str, &c);
        if (i<tam-1)
            strcat(str, ",");
    }
    return(str);
}

int newNode(node *list, int tam, int identificador) {
    int i;
    for(i=0; i<tam; i++) {
        if (list[i].index == identificador)
            return 0;
    }
    return 1;
}

// DFS
int detectCycle(node node) {
    for(node)  // percorre os vizinhos
    return 1;
}

void criaAresta(node *list, int Ti, int Tj, int n_nodes) {
    int i, j;
    node nodej;
    for (i=0;i<n_nodes;i++){
        if (list[i].index == Tj)
            nodej = list[i];
    }
    for (i=0; i<n_nodes; i++) {
        if (list[i].index == Ti) {
            for (j=0; j<n_nodes; j++) {
                if (list[i].neighbours[j].index == 0) {
                    list[i].neighbours[j] = nodej;
                    break;
                }
            }
        }
    }
}

char *seriabilidade(transacao *transacoes, int tam) {
    int i, j, m, n_nodes=0;
    node *list_nodes = (node*) malloc(tam*sizeof(node));
    for(i=0; i<tam; i++)
        list_nodes[i].neighbours = (node*) malloc(tam*sizeof(node));

     // cria um nó para cada T do escalonamento
    for (i=0, j=0; i<tam; i++) {
        if (newNode(list_nodes, tam, transacoes[i].identificador)) {
            n_nodes++;
            list_nodes[j].index = transacoes[i].identificador;
            j++;
        }
    }

    // cria aresta Ti -> Tj para cada r(x) em Tj depois de w(x) em Ti
    for (i=0; i<n_nodes; i++) {     // percorre os nós T
        int Ti = list_nodes[i].index;
        if (Ti == 0)
            break;
        for (j=0; j<tam-1; j++) {     // percorre a lista de transacoes
            if (transacoes[j].identificador==Ti && transacoes[j].operacao=='W') {
                for (m=j+1; m<tam; m++) {
                    if (transacoes[m].identificador != Ti && transacoes[m].operacao=='R' && transacoes[j].atributo == transacoes[m].atributo){
                        criaAresta(list_nodes, Ti, transacoes[m].identificador, n_nodes);
                    }
                }
            }
        }
    }

    // cria aresta Ti -> Tj para cada w(x) em Tj depois de r(x) em Ti
    for (i=0; i<n_nodes; i++) {     // percorre os nós T
        int Ti = list_nodes[i].index;
        if (Ti == 0)
            break;
        for (j=0; j<tam-1; j++) {     // percorre a lista de transacoes
            if (transacoes[j].identificador==Ti && transacoes[j].operacao=='R') {
                for (m=j+1; m<tam; m++) {
                    if (transacoes[m].identificador != Ti && transacoes[m].operacao=='W' && transacoes[j].atributo == transacoes[m].atributo){
                        criaAresta(list_nodes, Ti, transacoes[m].identificador, n_nodes);
                    }
                }
            }
        }
    }

    // cria aresta Ti -> Tj para cada w(x) em Tj depois de w(x) em Ti
    for (i=0; i<n_nodes; i++) {     // percorre os nós T
        int Ti = list_nodes[i].index;
        if (Ti == 0)
            break;
        for (j=0; j<tam-1; j++) {     // percorre a lista de transacoes
            if (transacoes[j].identificador==Ti && transacoes[j].operacao=='W') {
                for (m=j+1; m<tam; m++) {
                    if (transacoes[m].identificador != Ti && transacoes[m].operacao=='W' && transacoes[j].atributo == transacoes[m].atributo){
                        criaAresta(list_nodes, Ti, transacoes[m].identificador, n_nodes);
                    }
                }
            }
        }
    }

    for (i=0;i<n_nodes;i++) {
        printf("T%d:\n", list_nodes[i].index);
        for (j=0;j<n_nodes-1;j++) {
            printf("%d",  list_nodes[i].neighbours[j].index);
        }
        printf("\n");
    }

    if (detectCycle(list_nodes[0]))    // se há ciclo no grafo
        return "SS";   // é serial
    else
        return "NS";   // não é serial
}

char *visao(transacao *transacoes, int tam) {

    if (1)
        return "SV";   // é equivalente
    else
        return "NV";   // não é equivalente
}

int main() {
    int i, tam_list=0, n_escalonamentos=1;
    transacao *lista_transacoes = malloc (TAM_BUFFER * sizeof(transacao)) ;
    readList(lista_transacoes, &tam_list);
    // separaEscalonamentos()  // funcao para separar os escalonamentos em listas -----> a fazer
    for (i=0; i<n_escalonamentos; i++)
        printf("%d, %s, %s, %s\n", i+1, printList(lista_transacoes, tam_list), seriabilidade(lista_transacoes, tam_list), visao(lista_transacoes, tam_list));
        
    return 0;
}
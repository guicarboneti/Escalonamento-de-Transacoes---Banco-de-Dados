#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAM_BUFFER 20

typedef struct escalonamento {
    int begin;
    int end;
} escalonamento;

typedef struct transacao {
    int tempo_chegada;
    int identificador;
    char operacao;
    char atributo;

} transacao;

typedef struct node {
    int index;
    struct node *neighbours;
    int visited;
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
    int i, j, list_ids[tam], printed, idx_id=0;
    char *str, c;
    str = malloc(tam*sizeof(char));
    for (i=0; i<tam; i++) {
        printed = 0;
        for (j=0;j<idx_id;j++) {
            if (list[i].identificador == list_ids[j]) {
                printed = 1;
                break;
            }
        }
        if (!printed)
            list_ids[idx_id++] = list[i].identificador;
    }
    for (i=0; i<idx_id; i++){
        c = list_ids[i]+'0';
        strcat(str, &c);
        if (i<idx_id-1)
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

int DFS(node *G, node *v, int n_nodes) {
    int i;
    v->visited = 1;
    for (i=0; i<n_nodes-1; i++) {
        if (v->neighbours[i].index == 0)
            return 1;
        else if (v->neighbours[i].visited)
            return 0;
        else if (!DFS(G, &v->neighbours[i], n_nodes))
            return 0;
    }
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
    for(i=0; i<tam; i++) {
        list_nodes[i].index = 0;
        list_nodes[i].visited = 0;
        list_nodes[i].neighbours = (node*) malloc(tam*sizeof(node));
    }

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

    if (DFS(list_nodes, &list_nodes[0], n_nodes))    // busca em profundidade para detectar ciclo 
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

void separaEscalonamentos(transacao *lista, escalonamento *esc, int *n_esc, int tam) {
    int i, j, m, idx_ids=0, passed;
    int *list_ids = calloc(tam, sizeof(int));
    for (i=0; i<tam; i++) {
        passed = 0;
        for (j=0;j<idx_ids;j++) {
            if (lista[i].identificador == list_ids[j]) {
                passed = 1;
                break;
            }
        }
        if (!passed)
            list_ids[idx_ids++] = lista[i].identificador;
        if (lista[i].atributo == '-') {
            for (j=0; j<idx_ids; j++) {
                if (lista[i].identificador == list_ids[j]) {
                    if (list_ids[j+1] == 0)
                        list_ids[j] = 0;
                    else {
                        for (m=j; m<idx_ids-1; m++) 
                            list_ids[m] = list_ids[m+1];
                    }
                    idx_ids--;
                    break;
                }
            }
        }
        if (list_ids[0] == 0) {
            esc[(*n_esc)].begin = (esc[(*n_esc)].end == 0) ? 0 : esc[(*n_esc)-1].end+1;
            esc[(*n_esc)++].end = i;
        }
    }
}

int main() {
    int i, ini, tam_tra, tam_list=0, n_escalonamentos=0;
    escalonamento *escalonamentos = malloc (TAM_BUFFER * sizeof(escalonamento)) ;
    transacao *lista_transacoes = malloc (TAM_BUFFER * sizeof(transacao)) ;
    readList(lista_transacoes, &tam_list);
    separaEscalonamentos(lista_transacoes, escalonamentos, &n_escalonamentos, tam_list);  // funcao para separar os escalonamentos em listas -----> a fazer
    for (i=0; i<n_escalonamentos; i++) {
        ini = escalonamentos[i].begin;
        tam_tra = escalonamentos[i].end - escalonamentos[i].begin + 1;
        printf("%d %s %s %s\n", i+1, printList(&lista_transacoes[ini], tam_tra), seriabilidade(&lista_transacoes[ini], tam_tra), visao(&lista_transacoes[ini], tam_tra));
    }
        
    free(lista_transacoes);
    return 0;
}
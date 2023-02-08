#ifndef __ESCALONA__
#define __ESCALONA__

typedef struct escalonamento {
    int begin;
    int end;
} escalonamento;

typedef struct transacao {
    int tempo_chegada;
    int identificador;
    int qtd_operacoes;
    char operacao;
    char atributo;

} transacao;

typedef struct node {
    int index;
    struct node *neighbours;
    int visited;
} node;

void readList(transacao *list, int *tam);
char *printList(transacao *list, int tam);
void separaEscalonamentos(transacao *lista, escalonamento *esc, int *n_esc, int tam);

#endif
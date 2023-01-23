#include <stdio.h>
#include <stdlib.h>

typedef struct transacao {
    int tempo_chegada;
    int identificador;
    char operacao;
    char atributo;

} transacao;

void readList(transacao **lista, int *tam) {
    int i;
    for (i=0; ;i++) {
        if (scanf("%d", &lista[i]->tempo_chegada) == EOF)
            break;
        scanf("%d", &lista[i]->identificador);
        scanf(" %c", &lista[i]->operacao);
        scanf(" %c", &lista[i]->atributo);
        tam++;
    }
}

int main() {
    int i, tam_list=0;
    transacao **lista_transacoes = malloc (10 * sizeof(transacao*)) ;
    readList(lista_transacoes, &tam_list);

    // for (i=0; ;i++) {
    //     if (scanf("%d", &lista_transacoes[i].tempo_chegada) == EOF)
    //         break;
    //     scanf("%d", &lista_transacoes[i].identificador);
    //     scanf(" %c", &lista_transacoes[i].operacao);
    //     scanf(" %c", &lista_transacoes[i].atributo);
    //     tam_list++;
    // }


    for (i=0; i<tam_list; i++){
        printf("%d", lista_transacoes[i]->tempo_chegada);
        printf(" - %d", lista_transacoes[i]->identificador);
        printf(" - %c", lista_transacoes[i]->operacao);
        printf(" - %c\n", lista_transacoes[i]->atributo);
    }

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "escalona.h"
#include "seriabilidade.h"
#include "visao.h"
#define TAM_BUFFER 200

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
    int i, j, *list_ids, printed, idx_id=0, min, aux;
    char *str, *c;
    str = malloc(tam*sizeof(char));
    c = malloc(3*sizeof(char));
    list_ids = calloc(tam, sizeof(char));
    // select numbers
    for (i=0; i<tam; i++) {
        printed = 0;
        for (j=0;j<idx_id;j++)
            if (list[i].identificador == list_ids[j]) {
                printed = 1;
                break;
            }
        if (!printed)
            list_ids[idx_id++] = list[i].identificador;
    }
    // sort array
    for (i=0; i<idx_id-1; i++) {
        min = i;
        for (j=i+1; j<idx_id; j++)
            if (list_ids[j] < list_ids[min]) 
                min = j;
        if (i != min) {
            aux = list_ids[i];
            list_ids[i] = list_ids[min];
            list_ids[min] = aux;
        }
    }
    // convert to string
    for (i=0; i<idx_id; i++){
        sprintf(c, "%d", list_ids[i]);
        strcat(str, c);
        if (i<idx_id-1)
            strcat(str, ",");
    }
    return(str);
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
                    if (idx_ids == 1)
                        list_ids[j] = 0;
                    for (m=j; m<idx_ids-1; m++) 
                        list_ids[m] = list_ids[m+1];
                    idx_ids--;
                    break;
                }
            }
        }
        if (list_ids[0] == 0) {
            esc[(*n_esc)].begin = (esc[(*n_esc)-1].end == 0) ? 0 : esc[(*n_esc)-1].end+1;
            esc[(*n_esc)++].end = i;
        }
    }
}

int main() {
    int i, ini, tam_tra, tam_list=0, n_escalonamentos=0;
    escalonamento *escalonamentos = malloc (TAM_BUFFER * sizeof(escalonamento)) ;
    transacao *lista_transacoes = malloc (TAM_BUFFER * sizeof(transacao)) ;
    readList(lista_transacoes, &tam_list);
    separaEscalonamentos(lista_transacoes, escalonamentos, &n_escalonamentos, tam_list);
    for (i=0; i<n_escalonamentos; i++) {
        ini = escalonamentos[i].begin;
        tam_tra = escalonamentos[i].end - escalonamentos[i].begin + 1;
        // printf("%d %s %s %s\n", i+1, printList(&lista_transacoes[ini], tam_tra), seriabilidade(&lista_transacoes[ini], tam_tra), visao(&lista_transacoes[ini], tam_tra));
        printf("%d %s %s\n", i+1, printList(&lista_transacoes[ini], tam_tra), seriabilidade(&lista_transacoes[ini], tam_tra));
    }
        
    free(lista_transacoes);
    return 0;
}
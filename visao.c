#include <stdlib.h>
#include <stdio.h>
#include "visao.h"
#include "escalona.h"

#define READ 'R'
#define WRITE 'W'
#define MAX_TRANSACTIONS 150

char *visao(transacao *transacoes, int num_operacoes)
{
    int i, j, qtd_transacoes = 0;
    int flag, identificador;
    int *ids_unicos = calloc(MAX_TRANSACTIONS, sizeof(int));
   
    // separa as transações por número de id
    transacao *transacoes_id = malloc(MAX_TRANSACTIONS * sizeof(transacao));
    for (i = 0; i < num_operacoes; i++) {
        flag = 0;
        for (j = 0; j < qtd_transacoes; j++) {
            if (transacoes[i].identificador == ids_unicos[j]) {
                flag = 1;
                break;
            }
        }
        if (!flag) {
            ids_unicos[qtd_transacoes] = transacoes[i].identificador;

            transacoes_id[qtd_transacoes].identificador = transacoes[i].identificador;
            transacoes_id[qtd_transacoes].atributo = transacoes[i].atributo;
            transacoes_id[qtd_transacoes].operacao = transacoes[i].operacao;
            transacoes_id[qtd_transacoes].tempo_chegada = transacoes[i].tempo_chegada;

            qtd_transacoes++;
        }
    }
    // cria duas matrizes para armazenar os atributos escritos e lidos para cada transação no escalonamento
    int writes[qtd_transacoes][num_operacoes];
    int reads[qtd_transacoes][num_operacoes];

    // cria dois vetores para armazenar número de leituras e escritas por vetor
    int *cont_escrita = calloc(MAX_TRANSACTIONS, sizeof(int));
    int *cont_leitura = calloc(MAX_TRANSACTIONS, sizeof(int));

    // inicializa matrizes
    for (int i = 0; i < qtd_transacoes; i++)
    {
        for (int j = 0; j < num_operacoes; j++)
        {
            writes[i][j] = 0;
            reads[i][j] = 0;
        }
    }

    // contabiliza operações e armazena a operação de cada transação
    for (int i = 0; i < num_operacoes; i++)
    {
        identificador = transacoes[i].identificador;
        
        if (transacoes[i].operacao == READ)
        {
            reads[identificador][cont_leitura[identificador]] = transacoes[i].atributo;
            cont_leitura[identificador]++;
        }
        else if (transacoes[i].operacao == WRITE)
        {
            writes[identificador][cont_escrita[identificador]] = transacoes[i].atributo;
            cont_escrita[identificador]++;
        }
    }

    // verifica se para cada leitura de atributo de Ti já foi escrito por Tj
    for (int i = 0; i < qtd_transacoes; i++)
    {
        for (int j = 0; j < cont_leitura[i]; j++)
        {
            int atributo = reads[i][j];
            int escrito = 0;
            for (int k = 0; k < qtd_transacoes; k++)
            {
                // garante que não seja considerada as escritas da própria transação Ti
                if (k == i)
                    continue;
                for (int l = 0; l < cont_escrita[k]; l++)
                {
                    if (writes[k][l] == atributo)
                    {
                        escrito = 1;
                        break;
                    }
                }
                if (escrito)
                    break;
            }
            if (!escrito)
                return "NV";
        }
    }
    // valida se a escrita de um atributo for a última operação da transação
    for (int i = 0; i < qtd_transacoes; i++)
    {
        for (int j = 0; j < cont_escrita[i]; j++)
        {
            int atributo = writes[i][j];
            for (int k = 0; k < qtd_transacoes; k++)
            {
                // garante que não seja considerada as operações de escrita da transação i
                if (k == i)
                    continue;
                for (int l = 0; l < cont_escrita[k]; l++)
                {
                    if (writes[k][l] == atributo)
                        return "NV";
                }
            }
        }
    } 

    return "SV";
}
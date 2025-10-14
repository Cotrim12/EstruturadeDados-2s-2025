#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tarefa.h"

Lista * CriarLista(int C) {

Lista * L = malloc(sizeof(Lista));

if (L == NULL){
    printf("ERRO: nao existe memoria para a estrutura da lista");
    return NULL;
}

L->Tamanho = 0;
L-> Capacidade = C;
L->Dados = (Tarefa *)malloc(C * sizeof(Tarefa));

if (L->Dados == NULL) {
    printf("ERRO: nao existe memoria para o vetor de tarefas");
    free(L);
    return NULL;
}

return L;

}
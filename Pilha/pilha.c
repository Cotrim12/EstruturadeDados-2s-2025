#include <stdio.h>
#include <stdlib.h>

#include "pilha.h"


Pilha * criarPilha(int Chave){

    Pilha * P = (Pilha *) malloc (sizeof(Pilha));

    if (P == NULL) {
        printf("ERRO: nao foi possivel alocar memoria para a pilha");
        return NULL;
    }

    P-> Tamanho = 0;
    P-> Topo = NULL;

    return P;
}

Pilha * criarPilha(){

}
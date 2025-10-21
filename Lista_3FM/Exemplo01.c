#include <stdio.h>
#include <stdlib.h>

// Definindo a estrutura de um ITEM de uma lista ENCADEADA

typedef struct Item {
    int Chave;
    struct Item * Proximo;

    } Item;

    // Bloco principal

    int main(){

        Item * I = malloc(sizeof(Item));

        I->Chave = 23;
        I->Proximo = NULL;

        // Alguns testes (printf's)

        printf("Endereco (hexadecimal): %X\n", I);
        printf("%d\n", I->Chave);
        printf("%X\n", I->Proximo);
    }


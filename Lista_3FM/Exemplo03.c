#include <stdio.h>
#include <stdlib.h>

// Definindo a estrutura de um ITEM de uma lista ENCADEADA

typedef struct Item {
    int Chave;
    struct Item * Proximo;

    } Item;

    // Bloco principal

    int main(){

        Item * A = (Item *) malloc (sizeof(Item));
        Item * B = (Item *) malloc (sizeof(Item));
        Item * C = (Item *) malloc (sizeof(Item));
        Item * D = (Item *) malloc (sizeof(Item));

        A->Chave = 17;
        B->Chave = 29;
        C->Chave = 41;
        D->Chave = 97;

        A->Proximo = NULL;
        B->Proximo = NULL;
        C->Proximo = NULL;
        D->Proximo = NULL;

        // Alguns testes printfs

        printf("%X \t %d\n", A, A->Chave);
        printf("%X \t %d\n", B, B->Chave);
        printf("%X \t %d\n", C, C->Chave);
        printf("%X \t %d\n", D, D->Chave);

        // Construindo o encadeamento (LISTA) A,B,C,D

        A->Proximo = B;
        B->Proximo = C;
        C->Proximo = D;

        // Percorrer a lista

        Item * X = (Item *) malloc (sizeof(Item));

  




    }
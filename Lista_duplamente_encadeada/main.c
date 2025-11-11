#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "musica.h"

int main(){

    Item * D = criarItem("Aquarela", "Toquinho", 1983);
    Item * E = criarItem("Romaria", "Renato Teixeira", 1978);
    Item * F = criarItem("Eu nasci há dez mil anos atrás", "Raul Seixas", 1976);
    Item * G = criarItem("Calice", "Gilberto Gil e Chico Buarque", 1978);
    Item * H = criarItem("Tempo Perdido", "Renato Russo", 1986);


     // Construindo o encandeamento D,E,F,G,H

     D->Anterior = NULL;
     D->Posterior = F;

     E->Anterior = D;
     E->Posterior = F;

     F->Anterior = E;
     F->Posterior = G;

     G->Anterior = F;
     G->Posterior = H;

     H->Anterior = G;
     H->Posterior = NULL;

     

     // Criando uma lista

     Lista * PlayList = (Lista *) malloc (sizeof(Lista));
     if (PlayList == NULL){
        exit(1);
     }

     PlayList -> Tamanho = 5;
     PlayList -> Inicio = D;
     PlayList -> Fim = H;

     exibirInicio(PlayList);

}
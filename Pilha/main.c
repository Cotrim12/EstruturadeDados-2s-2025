#include <stdio.h>
#include <stdlib.h>

#include "pilha.h"

int main(){

    Item * I1 = criarItem(30);
    Item * I2 = criarItem(26);
    Item * I3 = criarItem(12);
    Item * I4 = criarItem(9);
    Item * I5 = criarItem(5);
    Item * I6 = criarItem(10);
    Item * I7 = criarItem(27);

    Pilha * MinhaPilha = criarPilha();

    printf("%X", MinhaPilha);
}
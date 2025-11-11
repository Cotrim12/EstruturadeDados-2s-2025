#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cidades.h"

int main() {
    int Soma = 0;
    int NumTeste = 0;
    FILE *Resposta = fopen("Resultado2.txt", "w");

    Estrada *T1 = getEstrada("Teste02.txt");
    double D1 = calcularMenorVizinhanca("Teste02.txt");
    char *C1 = cidadeMenorVizinhanca("Teste02.txt");

    if (T1->T == 20) Soma++;
    NumTeste++;
    if (T1->N == 4) Soma++;
    NumTeste++;

    // Esses valores dependem do que o professor espera nesse teste:
    fprintf(Resposta, "Tamanho da estrada: %d\n", T1->T);
    fprintf(Resposta, "Número de cidades: %d\n", T1->N);
    fprintf(Resposta, "Menor vizinhança: %.2f\n", D1);
    fprintf(Resposta, "Cidade com menor vizinhança: %s\n", C1);
    fprintf(Resposta, "\nVocê acertou %d de %d testes.\n", Soma, NumTeste);

    fclose(Resposta);
    free(C1);
}

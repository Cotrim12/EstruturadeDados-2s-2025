#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cidades.h"

#define MAX_NOME 256

// Funções auxiliares simples (sem ctype.h)
static void trim_trailing(char *s) {
    int len = (int)strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r' || s[len - 1] == ' ' || s[len - 1] == '\t')) {
        s[len - 1] = '\0';
        len--;
    }
}

static void ltrim(char *s) {
    int i = 0, j = 0;
    while (s[i] == ' ' || s[i] == '\t') i++;
    if (i > 0) {
        while (s[i]) s[j++] = s[i++];
        s[j] = '\0';
    }
}

// Libera memória da estrada e das cidades
static void freeEstrada(Estrada *e) {
    if (!e) return;
    Cidade *p = e->Inicio;
    while (p) {
        Cidade *next = p->Proximo;
        free(p);
        p = next;
    }
    free(e);
}

// -----------------------------------------------------
// 1. Lê o arquivo e cria a estrutura da estrada
// -----------------------------------------------------
Estrada *getEstrada(const char *nomeArquivo) {
    if (nomeArquivo == NULL) return NULL;

    FILE *fp = fopen(nomeArquivo, "r");
    if (!fp) return NULL;

    char linha[1024];
    long T, N;

    // Lê T (comprimento da estrada)
    do {
        if (!fgets(linha, sizeof(linha), fp)) { fclose(fp); return NULL; }
        trim_trailing(linha);
    } while (linha[0] == '\0');
    if (sscanf(linha, "%ld", &T) != 1) { fclose(fp); return NULL; }

    // Lê N (número de cidades)
    do {
        if (!fgets(linha, sizeof(linha), fp)) { fclose(fp); return NULL; }
        trim_trailing(linha);
    } while (linha[0] == '\0');
    if (sscanf(linha, "%ld", &N) != 1) { fclose(fp); return NULL; }

    // Valida restrições
    if (T < 3 || T > 1000000L || N < 2 || N > 10000L) {
        fclose(fp);
        return NULL;
    }

    int *posicoes = (int *)malloc(sizeof(int) * (size_t)N);
    if (!posicoes) { fclose(fp); return NULL; }

    Estrada *estrada = (Estrada *)malloc(sizeof(Estrada));
    if (!estrada) { free(posicoes); fclose(fp); return NULL; }
    estrada->N = (int)N;
    estrada->T = (int)T;
    estrada->Inicio = NULL;

    Cidade *ultima = NULL;
    int lidas = 0;

    // Lê cada cidade
    for (int i = 0; i < (int)N; i++) {
        if (!fgets(linha, sizeof(linha), fp)) {
            freeEstrada(estrada);
            free(posicoes);
            fclose(fp);
            return NULL;
        }
        trim_trailing(linha);
        if (linha[0] == '\0') { i--; continue; }

        int xi;
        char nome[MAX_NOME];
        int ok = sscanf(linha, "%d %[^\n]", &xi, nome);
        if (ok < 2) {
            freeEstrada(estrada);
            free(posicoes);
            fclose(fp);
            return NULL;
        }

        trim_trailing(nome);
        ltrim(nome);

        // Valida posição
        if (!(xi > 0 && xi < estrada->T)) {
            freeEstrada(estrada);
            free(posicoes);
            fclose(fp);
            return NULL;
        }

        // Verifica duplicata
        for (int j = 0; j < lidas; j++) {
            if (posicoes[j] == xi) {
                freeEstrada(estrada);
                free(posicoes);
                fclose(fp);
                return NULL;
            }
        }
        posicoes[lidas++] = xi;

        // Cria cidade
        Cidade *c = (Cidade *)malloc(sizeof(Cidade));
        if (!c) {
            freeEstrada(estrada);
            free(posicoes);
            fclose(fp);
            return NULL;
        }

        c->Posicao = xi;
        strncpy(c->Nome, nome, MAX_NOME - 1);
        c->Nome[MAX_NOME - 1] = '\0';
        c->Proximo = NULL;

        if (estrada->Inicio == NULL)
            estrada->Inicio = c;
        else
            ultima->Proximo = c;

        ultima = c;
    }

    free(posicoes);
    fclose(fp);
    return estrada;
}

// -----------------------------------------------------
// 2. Calcula o menor comprimento de vizinhança
// -----------------------------------------------------
double calcularMenorVizinhanca(const char *nomeArquivo) {
    Estrada *e = getEstrada(nomeArquivo);
    if (!e) return -1.0;

    int n = e->N;
    int T = e->T;

    Cidade **arr = (Cidade **)malloc(sizeof(Cidade *) * (size_t)n);
    if (!arr) { freeEstrada(e); return -1.0; }

    Cidade *p = e->Inicio;
    for (int i = 0; i < n; i++) {
        arr[i] = p;
        p = p->Proximo;
    }

    // Ordena por posição (insertion sort simples)
    for (int i = 1; i < n; i++) {
        Cidade *temp = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j]->Posicao > temp->Posicao) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = temp;
    }

    double menor = -1.0;
    for (int i = 0; i < n; i++) {
        double esquerda, direita;
        if (i == 0)
            esquerda = 0.0;
        else
            esquerda = (arr[i - 1]->Posicao + arr[i]->Posicao) / 2.0;

        if (i == n - 1)
            direita = (double)T;
        else
            direita = (arr[i]->Posicao + arr[i + 1]->Posicao) / 2.0;

        double tamanho = direita - esquerda;
        if (menor < 0 || tamanho < menor)
            menor = tamanho;
    }

    free(arr);
    freeEstrada(e);
    return menor;
}

// -----------------------------------------------------
// 3. Retorna o nome da cidade com menor vizinhança
// -----------------------------------------------------
char *cidadeMenorVizinhanca(const char *nomeArquivo) {
    Estrada *e = getEstrada(nomeArquivo);
    if (!e) return NULL;

    int n = e->N;
    int T = e->T;

    Cidade **arr = (Cidade **)malloc(sizeof(Cidade *) * (size_t)n);
    if (!arr) { freeEstrada(e); return NULL; }

    Cidade *p = e->Inicio;
    for (int i = 0; i < n; i++) {
        arr[i] = p;
        p = p->Proximo;
    }

    // Ordena por posição
    for (int i = 1; i < n; i++) {
        Cidade *temp = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j]->Posicao > temp->Posicao) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = temp;
    }

    double menor = -1.0;
    int indice = 0;

    for (int i = 0; i < n; i++) {
        double esquerda, direita;
        if (i == 0)
            esquerda = 0.0;
        else
            esquerda = (arr[i - 1]->Posicao + arr[i]->Posicao) / 2.0;

        if (i == n - 1)
            direita = (double)T;
        else
            direita = (arr[i]->Posicao + arr[i + 1]->Posicao) / 2.0;

        double tamanho = direita - esquerda;
        if (menor < 0 || tamanho < menor) {
            menor = tamanho;
            indice = i;
        }
    }

    char *resposta = (char *)malloc(strlen(arr[indice]->Nome) + 1);
    if (!resposta) {
        free(arr);
        freeEstrada(e);
        return NULL;
    }

    strcpy(resposta, arr[indice]->Nome);
    free(arr);
    freeEstrada(e);
    return resposta;
}

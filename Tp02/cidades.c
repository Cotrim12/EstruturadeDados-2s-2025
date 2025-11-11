#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cidades.h"

#define MAX_NOME 256

// Funções auxiliares (internas)
static void trim_trailing(char *s) {
    int len = (int)strlen(s);
    while (len > 0 && (s[len-1] == '\n' || s[len-1] == '\r' || isspace((unsigned char)s[len-1]))) {
        s[len-1] = '\0';
        --len;
    }
}
static void ltrim(char *s) {
    int i = 0, j = 0;
    while (s[i] && isspace((unsigned char)s[i])) i++;
    if (i > 0) {
        while (s[i]) s[j++] = s[i++];
        s[j] = '\0';
    }
}

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

Estrada *getEstrada(const char *nomeArquivo) {
    if (nomeArquivo == NULL) return NULL;

    FILE *fp = fopen(nomeArquivo, "r");
    if (!fp) return NULL;

    char line[1024];
    long T;
    long N;

    // Ler T
    do {
        if (!fgets(line, sizeof(line), fp)) { fclose(fp); return NULL; }
        trim_trailing(line);
    } while (line[0] == '\0'); // pular linhas em branco
    if (sscanf(line, "%ld", &T) != 1) { fclose(fp); return NULL; }

    // Ler N
    do {
        if (!fgets(line, sizeof(line), fp)) { fclose(fp); return NULL; }
        trim_trailing(line);
    } while (line[0] == '\0');
    if (sscanf(line, "%ld", &N) != 1) { fclose(fp); return NULL; }

    // Validar restrições iniciais
    if (T < 3 || T > 1000000L || N < 2 || N > 10000L) {
        fclose(fp);
        return NULL;
    }

    // Vetor temporário para checar duplicatas
    int *positions = (int*) malloc(sizeof(int) * (size_t)N);
    if (!positions) { fclose(fp); return NULL; }
    int readCount = 0;

    Estrada *estrada = (Estrada*) malloc(sizeof(Estrada));
    if (!estrada) { free(positions); fclose(fp); return NULL; }
    estrada->N = (int)N;
    estrada->T = (int)T;
    estrada->Inicio = NULL;

    Cidade *last = NULL;
    // Ler N linhas com Xi e nome
    for (int i = 0; i < (int)N; ++i) {
        if (!fgets(line, sizeof(line), fp)) {
            // Arquivo com menos linhas que o declarado -> erro
            freeEstrada(estrada);
            free(positions);
            fclose(fp);
            return NULL;
        }
        trim_trailing(line);
        // Pular linhas em branco (se houver)
        if (line[0] == '\0') { i--; continue; }

        int xi;
        char nome[MAX_NOME];
        // ler inteiro e resto da linha como nome (com espaços)
        // usar "%d %[^\n]" para pegar o nome com espaços
        int matched = sscanf(line, "%d %[^\n]", &xi, nome);
        if (matched < 1) {
            freeEstrada(estrada);
            free(positions);
            fclose(fp);
            return NULL;
        }
        if (matched == 1) {
            // não havia nome (apenas posicao) -> erro
            freeEstrada(estrada);
            free(positions);
            fclose(fp);
            return NULL;
        }
        trim_trailing(nome);
        ltrim(nome);

        // validar Xi
        if (!(xi > 0 && xi < estrada->T)) {
            freeEstrada(estrada);
            free(positions);
            fclose(fp);
            return NULL;
        }
        // checar duplicata
        for (int j = 0; j < readCount; ++j) {
            if (positions[j] == xi) {
                freeEstrada(estrada);
                free(positions);
                fclose(fp);
                return NULL;
            }
        }
        positions[readCount++] = xi;

        // criar nodo Cidade e anexar ao fim (ordem de leitura)
        Cidade *c = (Cidade*) malloc(sizeof(Cidade));
        if (!c) {
            freeEstrada(estrada);
            free(positions);
            fclose(fp);
            return NULL;
        }
        c->Posicao = xi;
        c->Proximo = NULL;
        // copiar nome com segurança
        strncpy(c->Nome, nome, MAX_NOME-1);
        c->Nome[MAX_NOME-1] = '\0';

        if (estrada->Inicio == NULL) {
            estrada->Inicio = c;
            last = c;
        } else {
            last->Proximo = c;
            last = c;
        }
    }

    free(positions);
    fclose(fp);
    return estrada;
}

double calcularMenorVizinhanca(const char *nomeArquivo) {
    Estrada *e = getEstrada(nomeArquivo);
    if (!e) return -1.0; // sinal de erro (a especificação só exige getEstrada retornar NULL)
    int n = e->N;
    int T = e->T;
    if (n <= 0) { freeEstrada(e); return -1.0; }

    // Colocar ponteiros em array para ordenar por posição
    Cidade **arr = (Cidade**) malloc(sizeof(Cidade*) * (size_t)n);
    if (!arr) { freeEstrada(e); return -1.0; }
    Cidade *p = e->Inicio;
    for (int i = 0; i < n; ++i) {
        arr[i] = p;
        if (p) p = p->Proximo;
    }

    // ordenar por Posicao
    int cmp(const void *a, const void *b) {
        Cidade *A = *(Cidade**)a;
        Cidade *B = *(Cidade**)b;
        return (A->Posicao - B->Posicao);
    }
    qsort(arr, (size_t)n, sizeof(Cidade*), cmp);

    // calcular vizinhanças
    double menor = -1.0;
    for (int i = 0; i < n; ++i) {
        double left, right;
        if (i == 0) left = 0.0;
        else left = (arr[i-1]->Posicao + arr[i]->Posicao) / 2.0;
        if (i == n-1) right = (double) T;
        else right = (arr[i]->Posicao + arr[i+1]->Posicao) / 2.0;
        double len = right - left;
        if (menor < 0 || len < menor) menor = len;
    }

    free(arr);
    freeEstrada(e);
    return menor;
}

char *cidadeMenorVizinhanca(const char *nomeArquivo) {
    Estrada *e = getEstrada(nomeArquivo);
    if (!e) return NULL;
    int n = e->N;
    int T = e->T;
    if (n <= 0) { freeEstrada(e); return NULL; }

    // Colocar ponteiros em array para ordenar por posição
    Cidade **arr = (Cidade**) malloc(sizeof(Cidade*) * (size_t)n);
    if (!arr) { freeEstrada(e); return NULL; }
    Cidade *p = e->Inicio;
    for (int i = 0; i < n; ++i) {
        arr[i] = p;
        if (p) p = p->Proximo;
    }

    int cmp(const void *a, const void *b) {
        Cidade *A = *(Cidade**)a;
        Cidade *B = *(Cidade**)b;
        return (A->Posicao - B->Posicao);
    }
    qsort(arr, (size_t)n, sizeof(Cidade*), cmp);

    double menor = -1.0;
    int idxMenor = 0;
    for (int i = 0; i < n; ++i) {
        double left, right;
        if (i == 0) left = 0.0;
        else left = (arr[i-1]->Posicao + arr[i]->Posicao) / 2.0;
        if (i == n-1) right = (double) T;
        else right = (arr[i]->Posicao + arr[i+1]->Posicao) / 2.0;
        double len = right - left;
        if (menor < 0 || len < menor) {
            menor = len;
            idxMenor = i;
        }
    }

    // Alocar e retornar o nome (cópia)
    char *res = (char*) malloc((strlen(arr[idxMenor]->Nome) + 1) * sizeof(char));
    if (!res) {
        free(arr);
        freeEstrada(e);
        return NULL;
    }
    strcpy(res, arr[idxMenor]->Nome);

    free(arr);
    freeEstrada(e);
    return res;
}

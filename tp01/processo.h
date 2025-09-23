#ifndef PROCESSO_H
#define PROCESSO_H

#include <time.h>

#define MAX_STR 512

typedef struct {
    char id_processo[MAX_STR];
    char numero_sigilo[MAX_STR];
    char sigla_grau[MAX_STR];
    char procedimento[MAX_STR];
    char ramo_justica[MAX_STR];
    char sigla_tribunal[MAX_STR];
    char id_tribunal[MAX_STR];
    char recurso[MAX_STR];
    char id_ultimo_oj[MAX_STR];
    char dt_recebimento[MAX_STR];
    char id_ultima_classe[MAX_STR];
    char flag_violencia_domestica[MAX_STR];
    char flag_feminicidio[MAX_STR];
    char flag_ambiental[MAX_STR];
    char flag_quilombolas[MAX_STR];
    char flag_indigenas[MAX_STR];
    char flag_infancia[MAX_STR];
    char decisao[MAX_STR];
    char dt_resolvido[MAX_STR];
    int cnm1;
    int primeirasentm1;
    int baixm1;
    int decm1;
    int mpum1;
    int julgadom1;
    int desm1;
    int susm1;
} Processo;

typedef struct {
    Processo *arr;
    size_t n;
    size_t cap;
} ListaProcessos;

void init_lista(ListaProcessos *L);
void free_lista(ListaProcessos *L);
void append_processo(ListaProcessos *L, const Processo *p);

int carregar_csv(const char *path, ListaProcessos *L);

size_t contar_processos(const ListaProcessos *L);

const char* id_ultimo_oj_a_partir_de_id_processo(const ListaProcessos *L, const char *id_processo);

const char* id_processo_mais_antigo(const ListaProcessos *L);

size_t contar_flag(const ListaProcessos *L, const char *campo_name);

long dias_entre_recebimento_resolvido(const Processo *p);

double percentual_cumprimento_meta1(const ListaProcessos *L);

int gerar_csv_julgados_meta1(const ListaProcessos *L, const char *outpath);


time_t parse_data_para_time(const char *s); 

#endif
#include "processo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <errno.h>

#define INITIAL_CAP 1024
#define SEP ";"
#define MAX_LINE 8192


void init_lista(ListaProcessos *L){
    L->arr = malloc(sizeof(Processo) * INITIAL_CAP);
    L->n = 0;
    L->cap = (L->arr? INITIAL_CAP:0);
}
void free_lista(ListaProcessos *L){
    free(L->arr);
    L->arr = NULL;
    L->n = 0;
    L->cap = 0;
}
void append_processo(ListaProcessos *L, const Processo *p){
    if (L->n >= L->cap){
        size_t nc = L->cap * 2;
        Processo *tmp = realloc(L->arr, sizeof(Processo) * nc);
        if(!tmp){ perror("realloc"); return; }
        L->arr = tmp;
        L->cap = nc;
    }
    L->arr[L->n++] = *p;
}

static void trim(char *s){
    if(!s) return;
    char *b = s;
    while(isspace((unsigned char)*b)) b++;
    if (b != s) memmove(s, b, strlen(b)+1);
    size_t len = strlen(s);
    while(len>0 && isspace((unsigned char)s[len-1])) s[--len]=0;
}

time_t parse_data_para_time(const char *s){
    if(!s) return (time_t)-1;
    if(strlen(s)==0) return (time_t)-1;
    struct tm tm = {0};
    char buf[64];
    strncpy(buf, s, sizeof(buf)-1);
    buf[sizeof(buf)-1]=0;
    trim(buf);
    if(strlen(buf)==0) return (time_t)-1;
    int d=0,m=0,y=0;
    if (sscanf(buf, "%d/%d/%d", &d, &m, &y) == 3){
        tm.tm_mday = d; tm.tm_mon = m-1; tm.tm_year = y-1900;
    } else if (sscanf(buf, "%d-%d-%d", &y, &m, &d) == 3){
        tm.tm_mday = d; tm.tm_mon = m-1; tm.tm_year = y-1900;
    } else {
        return (time_t)-1;
    }
    tm.tm_hour = 12; 
    time_t t = mktime(&tm);
    return t;
}

int carregar_csv(const char *path, ListaProcessos *L){
    FILE *f = fopen(path, "r");
    if(!f){ perror("fopen"); return -1; }
    char line[MAX_LINE];
    if(!fgets(line, sizeof(line), f)){ fclose(f); return -1; }
    while(fgets(line, sizeof(line), f)){
        char *p = strchr(line, '\n'); if(p) *p = 0;
        Processo proc;
        memset(&proc, 0, sizeof(proc));
        for(int i=0;i<MAX_STR;i++) { /* ensure null-terminated */ }
        /* tokenizar por ; */
        char *copy = strdup(line);
        char *tk;
        int col=0;
        tk = strtok(copy, SEP);
        while(tk){
            trim(tk);
            switch(col){
                case 0: strncpy(proc.id_processo, tk, MAX_STR-1); break;
                case 1: strncpy(proc.numero_sigilo, tk, MAX_STR-1); break;
                case 2: strncpy(proc.sigla_grau, tk, MAX_STR-1); break;
                case 3: strncpy(proc.procedimento, tk, MAX_STR-1); break;
                case 4: strncpy(proc.ramo_justica, tk, MAX_STR-1); break;
                case 5: strncpy(proc.sigla_tribunal, tk, MAX_STR-1); break;
                case 6: strncpy(proc.id_tribunal, tk, MAX_STR-1); break;
                case 7: strncpy(proc.recurso, tk, MAX_STR-1); break;
                case 8: strncpy(proc.id_ultimo_oj, tk, MAX_STR-1); break;
                case 9: strncpy(proc.dt_recebimento, tk, MAX_STR-1); break;
                case 10: strncpy(proc.id_ultima_classe, tk, MAX_STR-1); break;
                case 11: strncpy(proc.flag_violencia_domestica, tk, MAX_STR-1); break;
                case 12: strncpy(proc.flag_feminicidio, tk, MAX_STR-1); break;
                case 13: strncpy(proc.flag_ambiental, tk, MAX_STR-1); break;
                case 14: strncpy(proc.flag_quilombolas, tk, MAX_STR-1); break;
                case 15: strncpy(proc.flag_indigenas, tk, MAX_STR-1); break;
                case 16: strncpy(proc.flag_infancia, tk, MAX_STR-1); break;
                case 17: strncpy(proc.decisao, tk, MAX_STR-1); break;
                case 18: strncpy(proc.dt_resolvido, tk, MAX_STR-1); break;
                case 19: proc.cnm1 = atoi(tk); break;
                case 20: proc.primeirasentm1 = atoi(tk); break;
                case 21: proc.baixm1 = atoi(tk); break;
                case 22: proc.decm1 = atoi(tk); break;
                case 23: proc.mpum1 = atoi(tk); break;
                case 24: proc.julgadom1 = atoi(tk); break;
                case 25: proc.desm1 = atoi(tk); break;
                case 26: proc.susm1 = atoi(tk); break;
                default: break;
            }
            col++;
            tk = strtok(NULL, SEP);
        }
        free(copy);
        append_processo(L, &proc);
    }
    fclose(f);
    return 0;
}

/* 1 */
size_t contar_processos(const ListaProcessos *L){
    return L->n;
}

/* 2 */
const char* id_ultimo_oj_a_partir_de_id_processo(const ListaProcessos *L, const char *id_processo){
    for(size_t i=0;i<L->n;i++){
        if(strcmp(L->arr[i].id_processo, id_processo)==0) return L->arr[i].id_ultimo_oj;
    }
    return NULL;
}

/* 3 */
const char* id_processo_mais_antigo(const ListaProcessos *L){
    time_t best = (time_t)LLONG_MAX;
    const char *best_id = NULL;
    for(size_t i=0;i<L->n;i++){
        time_t t = parse_data_para_time(L->arr[i].dt_recebimento);
        if (t == (time_t)-1) continue;
        if (best_id==NULL || t < best){
            best = t;
            best_id = L->arr[i].id_processo;
        }
    }
    return best_id;
}

/* 4-9 generic contador de flags pelo nome */
size_t contar_flag(const ListaProcessos *L, const char *campo_name){
    size_t cnt=0;
    for(size_t i=0;i<L->n;i++){
        const Processo *p = &L->arr[i];
        const char *val = NULL;
        if(strcmp(campo_name, "flag_violencia_domestica")==0) val = p->flag_violencia_domestica;
        else if(strcmp(campo_name, "flag_feminicidio")==0) val = p->flag_feminicidio;
        else if(strcmp(campo_name, "flag_ambiental")==0) val = p->flag_ambiental;
        else if(strcmp(campo_name, "flag_quilombolas")==0) val = p->flag_quilombolas;
        else if(strcmp(campo_name, "flag_indigenas")==0) val = p->flag_indigenas;
        else if(strcmp(campo_name, "flag_infancia")==0) val = p->flag_infancia;
        else return 0;
        if(val && (val[0]=='1' || tolower(val[0])=='t')) cnt++;
    }
    return cnt;
}

/* 10 */
long dias_entre_recebimento_resolvido(const Processo *p){
    time_t t1 = parse_data_para_time(p->dt_recebimento);
    time_t t2 = parse_data_para_time(p->dt_resolvido);
    if(t1 == (time_t)-1 || t2 == (time_t)-1) return LONG_MIN;
    double diff = difftime(t2, t1);
    long dias = (long)(diff / 86400.0 + 0.5); /* arredondado */
    return dias;
}

/* 11 */
double percentual_cumprimento_meta1(const ListaProcessos *L){
    long long cnm1 = 0;
    long long julgadom1 = 0;
    long long desm1 = 0;
    long long susm1 = 0;
    for(size_t i=0;i<L->n;i++){
        cnm1 += L->arr[i].cnm1;
        julgadom1 += L->arr[i].julgadom1;
        desm1 += L->arr[i].desm1;
        susm1 += L->arr[i].susm1;
    }
    long long den = cnm1 + desm1 - susm1;
    if(den == 0) return 0.0;
    double perc = (double)julgadom1 / (double)den * 100.0;
    return perc;
}

/* 12 */
int gerar_csv_julgados_meta1(const ListaProcessos *L, const char *outpath){
    FILE *f = fopen(outpath, "w");
    if(!f){ perror("fopen out"); return -1; }
    /* escrever cabeçalho (mesmo do arquivo original) */
    fprintf(f, "id_processo;numero_sigilio;sigla_grau;procedimento;ramo_justica;sigla_tribunal;id_tribunal;recurso;id_ultimo_oj;dt_recebimento;id_ultima_classe;flag_violencia_domestica;flag_feminicidio;flag_ambiental;flag_quilombolas;flag_indigenas;flag_infancia;decisao;dt_resolvido;cnm1;primeirasentm1;baixm1;decm1;mpum1;julgadom1;desm1;susm1\n");
    for(size_t i=0;i<L->n;i++){
        const Processo *p = &L->arr[i];
        if(p->julgadom1 > 0){
            /* imprimir linha com separador ; */
            fprintf(f,
                "%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%d;%d;%d;%d;%d;%d;%d;%d\n",
                p->id_processo,
                p->numero_sigilo,
                p->sigla_grau,
                p->procedimento,
                p->ramo_justica,
                p->sigla_tribunal,
                p->id_tribunal,
                p->recurso,
                p->id_ultimo_oj,
                p->dt_recebimento,
                p->id_ultima_classe,
                p->flag_violencia_domestica,
                p->flag_feminicidio,
                p->flag_ambiental,
                p->flag_quilombolas,
                p->flag_indigenas,
                p->flag_infancia,
                p->decisao,
                p->dt_resolvido,
                p->cnm1,
                p->primeirasentm1,
                p->baixm1,
                p->decm1,
                p->mpum1,
                p->julgadom1,
                p->desm1,
                p->susm1
            );
        }
    }
    fclose(f);
    return 0;
}
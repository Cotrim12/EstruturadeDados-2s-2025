#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processo.h"

int main(int argc, char **argv){
    const char *path;
    if(argc < 2){
        char buf[512];
        printf("Informe o caminho do CSV (ex: TJDFT_filtrado.csv): ");
        if(!fgets(buf, sizeof(buf), stdin)) return 1;
        buf[strcspn(buf, "\r\n")] = 0;
        path = strdup(buf);
    } else path = argv[1];

    ListaProcessos L;
    init_lista(&L);
    if(carregar_csv(path, &L) != 0){
        fprintf(stderr, "Erro ao carregar CSV '%s'\n", path);
        free_lista(&L);
        return 1;
    }

    printf("1) Numero de processos na base: %zu\n", contar_processos(&L));

    if(L.n > 0){
        const char *ex_id = L.arr[0].id_processo;
        const char *id_oj = id_ultimo_oj_a_partir_de_id_processo(&L, ex_id);
        printf("2) id_ultimo_oj do processo '%s' = %s\n", ex_id, id_oj ? id_oj : "(nao encontrado)");
    } else {
        printf("2) sem processos para demonstrar lookup\n");
    }

    const char *antigo = id_processo_mais_antigo(&L);
    printf("3) id_processo com dt_recebimento mais antigo: %s\n", antigo ? antigo : "(nao encontrado)");

    printf("4) Processos com violencia domestica: %zu\n", contar_flag(&L, "flag_violencia_domestica"));
    printf("5) Processos com feminicidio: %zu\n", contar_flag(&L, "flag_feminicidio"));
    printf("6) Processos com ambiental: %zu\n", contar_flag(&L, "flag_ambiental"));
    printf("7) Processos com quilombolas: %zu\n", contar_flag(&L, "flag_quilombolas"));
    printf("8) Processos com indigenas: %zu\n", contar_flag(&L, "flag_indigenas"));
    printf("9) Processos com infancia/juventude: %zu\n", contar_flag(&L, "flag_infancia"));

    printf("10) Dias entre dt_recebimento e dt_resolvido (amostra até 10 linhas):\n");
    int shown = 0;
    for(size_t i=0;i<L.n && shown < 10;i++){
        long dias = dias_entre_recebimento_resolvido(&L.arr[i]);
        if(dias != LONG_MIN){
            printf("   %s : %ld dias\n", L.arr[i].id_processo, dias);
            shown++;
        }
    }
    if(shown==0) printf("   Nenhum processo com dt_resolvido valido encontrado.\n");

    double perc = percentual_cumprimento_meta1(&L);
    printf("11) Percentual de cumprimento da Meta 1 (conjunto): %.2f%%\n", perc);

    const char *out = "julgados_meta1.csv";
    if(gerar_csv_julgados_meta1(&L, out) == 0){
        printf("12) Arquivo gerado: %s (processos com julgadom1 > 0)\n", out);
    } else {
        printf("12) Erro ao gerar arquivo %s\n", out);
    }

    free_lista(&L);
    return 0;
}
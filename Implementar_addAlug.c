#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Size_data 13
#define Size_sala 21
#define Size_horario 5
#define Size_nome 52
#define Size_cpf 19
#define Size_numero 21
#define Size_prof_responsavel 52
#define Size_monitor_sn 2
#define Size_evento 1001

struct aluguel {
    char data[Size_data];
    char sala[Size_sala];
    char horario[Size_horario];
    char horarioFim[Size_horario];
    char nome[Size_nome];
    char cpf[Size_cpf];
    char celular[Size_numero];
    char prof_responsavel[Size_prof_responsavel];
    char monitor_sn[Size_monitor_sn];
    char evento[Size_evento];
};

void limparAluguel(struct aluguel *aluguelaserlimpo) {
    memset(aluguelaserlimpo, 0, sizeof(struct aluguel));  // Limpa toda a struct com zero
}

int verifica_disponibilidade(const char *data, const char *sala, char hora_inicial[], char hora_final[]) {
    char nome_arquivo[Size_data + 5];
    FILE *arquivo;
    struct aluguel aluguel_existente; // corrigido por john, não tenho ctz se era essa a intenção, verficar dps com matheus

    // Montar o nome do arquivo
    snprintf(nome_arquivo, sizeof(nome_arquivo), "%s.csv", data);

    // Abrir o arquivo
    arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo %s.\n", nome_arquivo);
        return 0;
    }

    // Ler o arquivo linha a linha usando fscanf
    while (fscanf(arquivo,
                  "%8[^;];%20[^;];%4[^;];%4[^;];%51[^;];%19[^;];%21[^;];%52[^;];%1[^;];%1000[^;];\n",
                  aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario,
                  aluguel_existente.horarioFim, aluguel_existente.nome, aluguel_existente.cpf,
                  aluguel_existente.celular, aluguel_existente.prof_responsavel,
                  aluguel_existente.monitor_sn, aluguel_existente.evento) != EOF) {

        // Converter os horários existentes para inteiros
        int hora_existente_ini = atoi(aluguel_existente.horario);
        int hora_existente_fim = atoi(aluguel_existente.horarioFim);

        // Comparar a sala e os horários
        if (strcmp(sala, aluguel_existente.sala) == 0) {
            if (!verifica_horarios(hora_inicial, hora_final, hora_existente_ini, hora_existente_fim)) {
                fclose(arquivo);
                limparAluguel(&aluguel_existente);
                printf("Conflito encontrado, insira hora inicial e final validas.");
                return 0; // Conflito encontrado
            }
        }
    }
    limparAluguel(&aluguel_existente);
    printf("Sem conflitos.");
    fclose(arquivo);
    return 1; // Sem conflito
}

void addAlug(const char *data, const char *sala, const char *horario, const char *horarioFim,
            const char *nome, const char *cpf, const char *celular, const char *prof_responsavel,
            const char *monitor_sn, const char *evento) {
    
    struct aluguel aluguel;
    char data_do_csv[Size_data + 5];
    sprintf(data_do_csv, "%s.csv", data);

    FILE *pont_csv = fopen(data_do_csv, "r+"); //ponteiro spawna no começo do arquivo

    //teste de conflito
    if(verifica_disponibilidade(data, sala, horario, horarioFim) == 0){
        fclose(pont_csv);
        limparAluguel(&aluguel);
        return;
    }

    if (!verificarCriarArquivo(data_do_csv)) {
        return;
    }

	strcpy(aluguel.data, data);
    strcpy(aluguel.sala, sala);
    strcpy(aluguel.horario, horario);
    strcpy(aluguel.horarioFim, horarioFim);
    strcpy(aluguel.nome, nome);
    strcpy(aluguel.cpf, cpf);
    strcpy(aluguel.celular, celular);
    strcpy(aluguel.prof_responsavel, prof_responsavel);
    strcpy(aluguel.monitor_sn, monitor_sn);
    strcpy(aluguel.evento, evento);

    fseek(pont_csv, 0, SEEK_END);

    //CHECK DE INFORMAÇOES

    //copiar a const pra mutavel pra conseguir usar no check

    //int check_de_erro = TrocarLouO (dataCheck, aluguel.sala, aluguel.horario); //aluguel.horarioFim // obsoleto
	int check_de_erro = IeFnoLouO (aluguel.data, aluguel.sala, aluguel.horario, aluguel.horarioFim);

    if (check_de_erro==0){ //faz o check de erro, se der tudo certo, retornando 0, printa no final e usa o L ou O dentro do proprio check de erro
        fprintf(pont_csv, "\n%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;",
            aluguel.data, aluguel.sala, aluguel.horario, aluguel.horarioFim, aluguel.nome,
            aluguel.cpf, aluguel.celular, aluguel.prof_responsavel, aluguel.monitor_sn,
            aluguel.evento);
        limparAluguel(&aluguel);
        printf("Aluguel adicionado! :)\n");
    }
    else{
        limparAluguel(&aluguel);
        printf("Tente novamente. Verifique se as informacoes estao validas.");
        return;
    }

    fclose(pont_csv);
 }

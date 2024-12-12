#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definindo os tamanhos para cada campo
#define Size_data 13
#define Size_sala 21
#define Size_horario 5
#define Size_nome 52
#define Size_cpf 19
#define Size_numero 21
#define Size_prof_responsavel 52
#define Size_monitor_sn 2
#define Size_evento 1001

//NAO PONHA A STRUCT DE ALUGUEL DEPOIS, ELA P R E C I S A FICAR AQUI.
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

int compDataHorSala(const char data1[], const char data2[], const char sala1[], const char sala2[], const char horario1[], const char horario2[]) {
    if (strcmp(data1, data2) == 0 && strcmp(sala1, sala2) == 0 && strcmp(horario1, horario2) == 0) {
        return 1;  // Retorna 1 se todas as comparações forem verdadeiras
    }
    return 0;  // Caso contrário, retorna 0
}

void removAlug(const char data[], const char sala[], const char horario[]) {
    char data_do_csv[Size_data + 5];
    sprintf(data_do_csv, "%s.csv", data);

    FILE *pont_csv = fopen(data_do_csv, "r");
    FILE *pont_temp = fopen("temp.csv", "w");

    struct aluguel aluguel_existente;
    int start_temp = 0;
    int tudocerto = 0;

    g_print("Tentando abrir o arquivo: _%s_\n", data_do_csv);

    if (pont_csv == NULL || pont_temp == NULL) {
        g_print("Erro ao procurar o arquivo, insira uma data valida ou tente novamente. :C\n");
        if (pont_csv) fclose(pont_csv);
        if (pont_temp) fclose(pont_temp);
        return;
    }

	g_print("\ndata:_%s_", data);

	g_print("\nsala:_%s_", sala);

	g_print("\nhorario:_%s_", horario);
    //Lendo arquivo existente
    while (fscanf(pont_csv, "%8[^;];%20[^;];%4[^;];%4[^;];%51[^;];%19[^;];%21[^;];%52[^;];%1[^;];%1000[^;];\n",
                  aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario, aluguel_existente.horarioFim,
                  aluguel_existente.nome, aluguel_existente.cpf, aluguel_existente.celular,
                  aluguel_existente.prof_responsavel, aluguel_existente.monitor_sn,
                  aluguel_existente.evento) == 10) {

		g_print("\naluguel_existente.data:_%s_", aluguel_existente.data);
		g_print("\ndata:_%s_", data);
		g_print("\naluguel_existente.sala:_%s_", aluguel_existente.sala);
		g_print("\nsala:_%s_", sala);
		g_print("\naluguel_existente.horario:_%s_", aluguel_existente.horario);
		g_print("\nhorario:_%s_\n", horario);

        //Verifica se o registro atual corresponde ao que deve ser removido
        if (compDataHorSala(aluguel_existente.data, data, aluguel_existente.sala, sala, aluguel_existente.horario, horario) == 1) {
            printf("Registro encontrado! :D\nData: %s, Sala: %s, Horario: %s\n",
                   aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario);
            //verificacao de validez pra planilha

            char dataCheck[Size_data], salaCheck[Size_sala], horarioCheck[Size_horario];
            strcpy(dataCheck, data);
            strcpy(salaCheck, sala);
            strcpy(horarioCheck, horario);
            //copiar a const pra mutavel pra conseguir usar no check

            //int check_de_erro = TrocarLouO (dataCheck, salaCheck, horarioCheck); //aluguel_existente.horarioFim // obsoleto
			int check_de_erro = IeFnoLouO (dataCheck, salaCheck, horarioCheck, aluguel_existente.horarioFim);
			//printf("\ncheck de erro IeF:_%i_", check_de_erro);

            if (check_de_erro == 0){ //faz o check de erro, se der tudo certo, retornando 0, printa no final e usa o L ou O dentro do proprio check de erro
                printf("Aluguel removido! :)\n");
                tudocerto = 1;
                continue; //nao escreve no temporario
            }
            else{ // so sera acionado se existir no registro mas nao na planilha, descartavel, mas deixo caso precisemos no futuro de algo semelhante
                printf("Tente novamente. Verifique se as informacoes estao validas.");
                limparAluguel(&aluguel_existente);
                fclose(pont_csv);
                remove("temp.csv");
                return;
            }

        }
            if (start_temp == 0){
            fprintf(pont_temp, "1;2;3;4;5;6;7;8;9;10;");
            start_temp = 1;
            }
        //Copia os outros registros existentes para o arquivo temporário
        fprintf(pont_temp, "\n%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;",
                aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario, aluguel_existente.horarioFim,
                aluguel_existente.nome, aluguel_existente.cpf, aluguel_existente.celular,
                aluguel_existente.prof_responsavel, aluguel_existente.monitor_sn,
                aluguel_existente.evento);
    }

    //Fecha os arquivos se estiverem abertos
    if (pont_csv != NULL) {
        fclose(pont_csv);
    }
    if (pont_temp != NULL) {
        fclose(pont_temp);
    }

    if (tudocerto == 1) {
        remove(data_do_csv);
        rename("temp.csv", data_do_csv);
        limparAluguel(&aluguel_existente);
        printf("Registro removido com sucesso! :)\n");
    } else {
        remove("temp.csv");
        limparAluguel(&aluguel_existente);
        printf("Registro nao encontrado. :C\n");
    }
}

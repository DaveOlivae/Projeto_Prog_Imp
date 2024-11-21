#include <stdio.h>
#include <string.h>

// Definindo os tamanhos para cada campo
#define Size_data 11
#define Size_sala 5
#define Size_horario 5
#define Size_nome 50
#define Size_cpf 13
#define Size_numero 20
#define Size_prof_responsavel 50
#define Size_monitor_sn 2
#define Size_evento 1000

// Struct para um aluguel
struct aluguel {
    char data[Size_data];
    char sala[Size_sala];
    char horario[Size_horario];
    char nome[Size_nome];
    char cpf[Size_cpf];
    char celular[Size_numero];
    char prof_responsavel[Size_prof_responsavel];
    char monitor_sn[Size_monitor_sn];
    char evento[Size_evento];
    int modificado;
};

// Função para comparar dados para testar conflitos
int compDataHorSala(const char *data1, const char *data2, const char *sala1, const char *sala2, const char *horario1, const char *horario2) {
    return (strcmp(data1, data2) == 0 && strcmp(sala1, sala2) == 0 && strcmp(horario1, horario2) == 0);
}

// Função para adicionar registro
void addAlug(const char *nome_do_csv) {
    struct aluguel aluguel, aluguel_existente;
    FILE *pont_csv = fopen(nome_do_csv, "a+");

    if (pont_csv == NULL) {
        printf("Arquivo de registro não encontrado. :C\n");
        return;
    }

    printf("Insira a data (DD-MM-AAAA): ");
    fgets(aluguel.data, Size_data, stdin);
    aluguel.data[strcspn(aluguel.data, "\n")] = '\0';

    printf("Insira a sala (B02, I15, ...): ");
    fgets(aluguel.sala, Size_sala, stdin);
    aluguel.sala[strcspn(aluguel.sala, "\n")] = '\0';

    printf("Insira o horário (formato 710 para 7:10): ");
    fgets(aluguel.horario, Size_horario, stdin);
    aluguel.horario[strcspn(aluguel.horario, "\n")] = '\0';

    printf("Nome do solicitante: ");
    fgets(aluguel.nome, Size_nome, stdin);
    aluguel.nome[strcspn(aluguel.nome, "\n")] = '\0';

    printf("Digite o CPF do solicitante: ");
    fgets(aluguel.cpf, Size_cpf, stdin);
    aluguel.cpf[strcspn(aluguel.cpf, "\n")] = '\0';

    printf("Celular para contato: ");
    fgets(aluguel.celular, Size_numero, stdin);
    aluguel.celular[strcspn(aluguel.celular, "\n")] = '\0';

    printf("Professor responsável: ");
    fgets(aluguel.prof_responsavel, Size_prof_responsavel, stdin);
    aluguel.prof_responsavel[strcspn(aluguel.prof_responsavel, "\n")] = '\0';

    printf("Monitor? (S/N): ");
    fgets(aluguel.monitor_sn, Size_monitor_sn, stdin);
    aluguel.monitor_sn[strcspn(aluguel.monitor_sn, "\n")] = '\0';

    printf("Insira a ocorrência: ");
    fgets(aluguel.evento, Size_evento, stdin);
    aluguel.evento[strcspn(aluguel.evento, "\n")] = '\0';

    aluguel.modificado = 0;

    // Teste de conflito de agendamentos
    rewind(pont_csv);
    while (fscanf(pont_csv, "%10[^;];%4[^;];%4[^\n]\n",
                  aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario) == 3) {
        if (compDataHorSala(aluguel.data, aluguel_existente.data, aluguel.sala, aluguel_existente.sala, aluguel.horario, aluguel_existente.horario)) {
            printf("Erro: Esse horário já foi agendado. :C\n");
            fclose(pont_csv);
            return;
        }
    }

    // Adiciona o novo registro
    fprintf(pont_csv, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%d\n",
            aluguel.data, aluguel.sala, aluguel.horario, aluguel.nome,
            aluguel.cpf, aluguel.celular, aluguel.prof_responsavel, aluguel.monitor_sn,
            aluguel.evento, aluguel.modificado);

    printf("Aluguel adicionado! :)\n");
    fclose(pont_csv);
}

// Função para atualizar um registro existente
void attAlugExistente(const char *nome_do_csv, const char *data, const char *sala, const char *horario) {
    FILE *pont_csv = fopen(nome_do_csv, "r");
    FILE *pont_temp = fopen("temp.csv", "w");
    struct aluguel aluguel_existente, aluguel_novo;
    int encontrou = 0;

    if (pont_csv == NULL || pont_temp == NULL) {
        printf("Arquivo de registro não encontrado. :C\n");
        if (pont_csv) fclose(pont_csv);
        if (pont_temp) fclose(pont_temp);
        return;
    }

    while (fscanf(pont_csv, "%10[^;];%4[^;];%4[^;];%49[^;];%12[^;];%19[^;];%49[^;];%1[^;];%999[^;];%d\n",
                  aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario,
                  aluguel_existente.nome, aluguel_existente.cpf, aluguel_existente.celular,
                  aluguel_existente.prof_responsavel, aluguel_existente.monitor_sn,
                  aluguel_existente.evento, &aluguel_existente.modificado) == 10) {

        if (compDataHorSala(aluguel_existente.data, data, aluguel_existente.sala, sala, aluguel_existente.horario, horario)) {
            printf("Registro encontrado! Insira as novas informações ou pressione Enter para manter os valores atuais:\n");

            printf("Nome do solicitante [%s]: ", aluguel_existente.nome);
            fgets(aluguel_novo.nome, Size_nome, stdin);
            aluguel_novo.nome[strcspn(aluguel_novo.nome, "\n")] = '\0';
            if (strlen(aluguel_novo.nome) == 0) strcpy(aluguel_novo.nome, aluguel_existente.nome);

            printf("CPF [%s]: ", aluguel_existente.cpf);
            fgets(aluguel_novo.cpf, Size_cpf, stdin);
            aluguel_novo.cpf[strcspn(aluguel_novo.cpf, "\n")] = '\0';
            if (strlen(aluguel_novo.cpf) == 0) strcpy(aluguel_novo.cpf, aluguel_existente.cpf);

            printf("Celular [%s]: ", aluguel_existente.celular);
            fgets(aluguel_novo.celular, Size_numero, stdin);
            aluguel_novo.celular[strcspn(aluguel_novo.celular, "\n")] = '\0';
            if (strlen(aluguel_novo.celular) == 0) strcpy(aluguel_novo.celular, aluguel_existente.celular);

            printf("Professor responsável [%s]: ", aluguel_existente.prof_responsavel);
            fgets(aluguel_novo.prof_responsavel, Size_prof_responsavel, stdin);
            aluguel_novo.prof_responsavel[strcspn(aluguel_novo.prof_responsavel, "\n")] = '\0';
            if (strlen(aluguel_novo.prof_responsavel) == 0) strcpy(aluguel_novo.prof_responsavel, aluguel_existente.prof_responsavel);

            printf("Monitor (S/N) [%s]: ", aluguel_existente.monitor_sn);
            fgets(aluguel_novo.monitor_sn, Size_monitor_sn, stdin);
            aluguel_novo.monitor_sn[strcspn(aluguel_novo.monitor_sn, "\n")] = '\0';
            if (strlen(aluguel_novo.monitor_sn) == 0) strcpy(aluguel_novo.monitor_sn, aluguel_existente.monitor_sn);

            printf("Ocorrência [%s]: ", aluguel_existente.evento);
            fgets(aluguel_novo.evento, Size_evento, stdin);
            aluguel_novo.evento[strcspn(aluguel_novo.evento, "\n")] = '\0';
            if (strlen(aluguel_novo.evento) == 0) strcpy(aluguel_novo.evento, aluguel_existente.evento);

            aluguel_novo.modificado = 1;
            encontrou = 1;

            fprintf(pont_temp, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%d\n",
                    aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario,
                    aluguel_novo.nome, aluguel_novo.cpf, aluguel_novo.celular,
                    aluguel_novo.prof_responsavel, aluguel_novo.monitor_sn,
                    aluguel_novo.evento, aluguel_novo.modificado);
        } else {
            fprintf(pont_temp, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%d\n",
                    aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario,
                    aluguel_existente.nome, aluguel_existente.cpf, aluguel_existente.celular,
                    aluguel_existente.prof_responsavel, aluguel_existente.monitor_sn,
                    aluguel_existente.evento, aluguel_existente.modificado);
        }
    }

    fclose(pont_csv);
    fclose(pont_temp);

    if (encontrou) {
        remove(nome_do_csv);
        rename("temp.csv", nome_do_csv);
        printf("Registro atualizado!\n");
    } else {
        printf("Registro não encontrado.\n");
        remove("temp.csv");
    }
}

// Função principal
int main() {
    char nome_do_csv[] = "registro.csv";

    addAlug(nome_do_csv);
    attAlugExistente(nome_do_csv, "20-11-2024", "A15", "730");

    return 0;
}

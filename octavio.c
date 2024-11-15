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
struct Alug {
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

// Função para comparação de dados para testar conflitos
int compDataHorSala(const char *data1, const char *data2, const char *sala1, const char *sala2, const char *horario1, const char *horario2) {
    return (strcmp(data1, data2) == 0 && strcmp(sala1, sala2) == 0 && strcmp(horario1, horario2) == 0);
}

// Função para adicionar registro
void addAlug(const char *nome_do_csv) {
    struct Alug aluguel, aluguel_existente;
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

// Função para atualizar registro existente
void attAlugExistente(const char *nome_do_csv, const char *data, const char *sala, const char *horario) {
    FILE *pont_csv = fopen(nome_do_csv, "r");
    FILE *pont_temp = fopen("temp.csv", "w");
    struct Alug aluguel, aluguel_existente;
    int encontrou = 0;

    if (pont_csv == NULL || pont_temp == NULL) {
        printf("Arquivo de registro não encontrado. :C\n");
        if (pont_csv) fclose(pont_csv);
        if (pont_temp) fclose(pont_temp);
        return;
    }

    while (fscanf(pont_csv, "%10[^;];%4[^;];%4[^\n]\n", 
                  aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario) == 3) {
        if (compDataHorSala(aluguel_existente.data, data, aluguel_existente.sala, sala, aluguel_existente.horario, horario)) {
            printf("Registro encontrado! Insira as novas informações:\n");

            printf("Nome do solicitante: ");
            fgets(aluguel.nome, Size_nome, stdin);
            aluguel.nome[strcspn(aluguel.nome, "\n")] = '\0';

            printf("CPF: ");
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

            printf("Ocorrência: ");
            fgets(aluguel.evento, Size_evento, stdin);
            aluguel.evento[strcspn(aluguel.evento, "\n")] = '\0';

            aluguel.modificado = 1;
            encontrou = 1;
        }

        fprintf(pont_temp, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%d\n", 
                aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario, 
                aluguel.nome, aluguel.cpf, aluguel.celular, 
                aluguel.prof_responsavel, aluguel.monitor_sn, aluguel.evento, aluguel.modificado);
    }

    fclose(pont_csv);
    fclose(pont_temp);

    if (encontrou == 1) {
        remove(nome_do_csv);
        rename("temp.csv", nome_do_csv);
        printf("Registro atualizado! :)\n");
    } else {
        printf("Registro não encontrado. :C\n");
        remove("temp.csv");
    }
}

// Função para remover um registro
void removAlug(const char *nome_do_csv, const char *data, const char *sala, const char *horario) {
    FILE *pont_csv = fopen(nome_do_csv, "r");
    FILE *pont_temp = fopen("temp.csv", "w");
    struct Alug aluguel_existente;
    int encontrou = 0;

    if (pont_csv == NULL || pont_temp == NULL) {
        printf("Arquivo de registro não encontrado. :C\n");
        if (pont_csv) fclose(pont_csv);
        if (pont_temp) fclose(pont_temp);
        return;
    }

    while (fscanf(pont_csv, "%10[^;];%4[^;];%4[^\n]\n", 
                  aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario) == 3) {
        
        if (compDataHorSala(aluguel_existente.data, data, aluguel_existente.sala, sala, aluguel_existente.horario, horario)) {
            encontrou = 1;
            continue;
        }

        fprintf(pont_temp, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%d\n", 
                aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario, 
                aluguel_existente.nome, aluguel_existente.cpf, aluguel_existente.celular, 
                aluguel_existente.prof_responsavel, aluguel_existente.monitor_sn, 
                aluguel_existente.evento, aluguel_existente.modificado);
    }

    fclose(pont_csv);
    fclose(pont_temp);

    if (encontrou == 1) {
        remove(nome_do_csv);
        rename("temp.csv", nome_do_csv);
        printf("Registro removido! :)\n");
    } else {
        printf("Registro não encontrado. :C\n");
        remove("temp.csv");
    }
}

// Função principal do programa
int registrador() {
    int opcao;
    char data[Size_data], sala[Size_sala], horario[Size_horario];

    do {
        printf("Selecione uma opção:\n");
        printf("1 - ADICIONAR um registro\n");
        printf("2 - ATUALIZAR um registro\n");
        printf("3 - REMOVER um registro\n");
        printf("0 - SAIR\n");
        printf("Opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer após scanf

        if (opcao == 2 || opcao == 3) {
            printf("Data (DD-MM-AAAA): ");
            fgets(data, Size_data, stdin);
            data[strcspn(data, "\n")] = '\0';

            printf("Sala (B02, I15, ...): ");
            fgets(sala, Size_sala, stdin);
            sala[strcspn(sala, "\n")] = '\0';

            printf("Horário (formato 710 para 7:10): ");
            fgets(horario, Size_horario, stdin);
            horario[strcspn(horario, "\n")] = '\0';
        }

        switch (opcao) {
            case 1:
                addAlug("alugueis.csv");
                break;
            case 2:
                attAlugExistente("alugueis.csv", data, sala, horario);
                break;
            case 3:
                removAlug("alugueis.csv", data, sala, horario);
                break;
            case 0:
                printf("Programa encerrado.\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 0);

    return 0;
}

int main() {
    registrador();
    return 0;
}

#include <stdio.h>

// Struct para um aluguel operante
struct Alug {
    char data[11];
    char sala[4];
    char evento[50];
    char nome[50];
    char cpf[15];
    char numero[20];
    int horario;
    int modificado;
//
	char data[11];
	char sala[5];
	char bloco_hora[4];
	char monitor_s/n[1];
	char CPF[13];
	char nome[50];
	char celular[20]; 
	char prof_responsavel[50];
	char evento [1000];
};

// Funcao de comparacao para testar conflitos
int compDataHorSala(const char *data1, const char *data2, const char *sala1, const char *sala2, int horario1, int horario2) {
    return (strcmp(data1, data2) == 0 && strcmp(sala1, sala2) == 0 && horario1 == horario2);
}

// Funcao para adicionar registro
void addAlug(const char *nome_do_csv) {
    struct Alug aluguel, aluguel_existente;
    FILE *pont_csv = fopen(nome_do_csv, "a+");

    if (pont_csv == NULL) {
        printf("Arquivo de registro não encontrado. :C\n");
        return;
    }

    printf("Insira a data (DD-MM-AAAA):\n");
    scanf("%10s", aluguel.data);
    
    printf("Insira a sala (B02, I15, ...):\n");
    scanf("%3s", aluguel.sala);
    
    printf("Insira a ocorrencia:\n");
    scanf(" %49[^\n]", aluguel.evento);
    
    printf("Nome do solicitante:\n");
    scanf(" %49[^\n]", aluguel.nome);
    
    printf("Digite o CPF do solicitante:\n");
    scanf("%14s", aluguel.cpf);
    
    printf("Celular para contato:\n");
    scanf("%19s", aluguel.numero);
    
    printf("Insira o horario (formato 710 para 7:10):\n");
    scanf("%d", &aluguel.horario);
    
    aluguel.modificado = 0;

    //Teste de conflito de agendamentos
    rewind(pont_csv); //Ponteiro de leitura retorna ao comeco
    while (fscanf(pont_csv, "%10[^;];%3[^;];%49[^;];%49[^;];%14[^;];%19[^;];%d;%d\n", 
                  aluguel_existente.data, aluguel_existente.sala, aluguel_existente.evento, aluguel_existente.nome, 
                  aluguel_existente.cpf, aluguel_existente.numero, &aluguel_existente.horario, &aluguel_existente.modificado) == 8) {
        if (compDataHorSala(aluguel.data, aluguel_existente.data, aluguel.sala, aluguel_existente.sala, aluguel.horario, aluguel_existente.horario)) {
            printf("Erro: Esse horario ja foi agendado. :C\n");
            fclose(pont_csv);
            return;
        }
    }

    //Se deu certo, adiciona o novo registro
    fprintf(pont_csv, "%s;%s;%s;%s;%s;%s;%d;%d\n", 
            aluguel.data, aluguel.sala, aluguel.evento, aluguel.nome, 
            aluguel.cpf, aluguel.numero, aluguel.horario, aluguel.modificado);
    
    printf("Aluguel adicionado ! C:\n");

    fclose(pont_csv);
}

//Funcao para atualizar um registro
void attAlug(const char *nome_do_csv, const char *sala, const char *data, int horario) {
    FILE *pont_csv = fopen(nome_do_csv, "r");
    FILE *pont_temp = fopen("temp.csv", "w");
    struct Alug aluguel;
    int encontrou = 0;

    if (pont_csv == NULL) {
        printf("Arquivo de registro não encontrado. :C\n");
        return;
    }

    //Escaneia linha a linha procurando o registro desejado
    while (fscanf(pont_csv, "%10[^;];%3[^;];%49[^;];%49[^;];%14[^;];%19[^;];%d;%d\n", 
                  aluguel.data, aluguel.sala, aluguel.evento, aluguel.nome, 
                  aluguel.cpf, aluguel.numero, &aluguel.horario, &aluguel.modificado) == 8) {
        //Caso encontre um igual, marca a modificacao e substitui esses dados pelos novos ao inves de copia-los do antigo
        if (compDataHorSala(aluguel.data, data, aluguel.sala, sala, aluguel.horario, horario)) {
            printf("Registro encontrado ! Insira as novas informações:\n");
            printf("Ocorrencia:\n");
            scanf(" %49[^\n]", aluguel.evento);
            printf("Nome do solicitante:\n");
            scanf(" %49[^\n]", aluguel.nome);
            printf("CPF:\n");
            scanf("%14s", aluguel.cpf);
            printf("Celular para contato:\n");
            scanf("%19s", aluguel.numero);
            aluguel.modificado = 1;
            encontrou = 1;
        }
        
        //Transcreve os dados para o arquivo temporario
        fprintf(pont_temp, "%s;%s;%s;%s;%s;%s;%d;%d\n", 
                aluguel.data, aluguel.sala, aluguel.evento, aluguel.nome, 
                aluguel.cpf, aluguel.numero, aluguel.horario, aluguel.modificado);
    }
//limpeza
    fclose(pont_csv);
    fclose(pont_temp);

    if (encontrou == 1) {
        remove(nome_do_csv);
        rename("temp.csv", nome_do_csv);
        printf("Registro atualizado! C:\n");
    } else {
        printf("Registro não encontrado. :C\n");
        remove("temp.csv");
    }
}

//Funcao para remover um registro
void removAlug(const char *nome_do_csv, const char *sala, const char *data, int horario) {
    FILE *pont_csv = fopen(nome_do_csv, "r");
    FILE *pont_temp = fopen("temp.csv", "w");
    struct Alug aluguel;
    int encontrou = 0;

    if (pont_csv == NULL) {
        printf("Arquivo de registro não encontrado. :C\n");
        return;
    }

    while (fscanf(pont_csv, "%10[^;];%3[^;];%49[^;];%49[^;];%14[^;];%19[^;];%d;%d\n", 
                  aluguel.data, aluguel.sala, aluguel.evento, aluguel.nome, 
                  aluguel.cpf, aluguel.numero, &aluguel.horario, &aluguel.modificado) == 8) {
        
        if (compDataHorSala(aluguel.data, data, aluguel.sala, sala, aluguel.horario, horario)) {
            encontrou = 1;
            continue; //Nao transcreve o registro escolhido e sinaliza que o achou
        }
        
        //Transcreve os dados para o arquivo temporario
        fprintf(pont_temp, "%s;%s;%s;%s;%s;%s;%d;%d\n", 
                aluguel.data, aluguel.sala, aluguel.evento, aluguel.nome, 
                aluguel.cpf, aluguel.numero, aluguel.horario, aluguel.modificado);
    }
//limpeza
    fclose(pont_csv);
    fclose(pont_temp);

    if (encontrou == 1) {
        remove(nome_do_csv);
        rename("temp.csv", nome_do_csv);
        printf("Registro removido ! C:\n");
    } else {
        printf("Registro não encontrado. :C\n");
        remove("temp.csv");
    }
}

//Feito por Matheus, Usa as funções de adicionar, atualizar ou remover um registro
int registrador() {
    int opcao;
    char sala[4], data[11];
    int horario;

    do {
        printf("Insira SOMENTE o numero:\n");
        printf("1 para ADICIONAR um registro;\n");
        printf("2 para ATUALIZAR um registro;\n");
        printf("3 para REMOVER um registro;\n");
        printf("0 para SAIR do programa.\n");
        printf("Opcao selecionada: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                addAlug("alugueis.csv");
                break;
            case 2:
                printf("Insira a sala (B02, I15, ...):\n");
                scanf("%3s", sala);
                printf("Digite a data (AAAA-MM-DD):\n");
                scanf("%10s", data);
                printf("Insira o horario (formato 710 para 7:10):\n");
                scanf("%d", &horario);
                attAlug("alugueis.csv", sala, data, horario);
                break;
            case 3:
                printf("Insira a sala (B02, I15, ...):\n");
                scanf("%3s", sala);
                printf("Digite a data (AAAA-MM-DD):\n");
                scanf("%10s", data);
                printf("Insira o horario (formato 710 para 7:10):\n");
                scanf("%d", &horario);
                removAlug("alugueis.csv", sala, data, horario);
                break;
            case 0:
                printf("Programa encerrado.\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    return 0;
}

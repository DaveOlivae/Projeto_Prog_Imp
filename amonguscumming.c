#include <stdio.h>

// Struct para um aluguel operante
#define Size_data 11
#define Size_sala 5
#define Size_horario 5
#define Size_nome 50
#define Size_cpf 13
#define Size_celular 20
#define Size_prof_responsavel 50
#define Size_monitor_sn 1
#define Size_evento 1000

struct Alug {
	char data[Size_data];
	char sala[Size_sala];
	char horario[Size_horario];
    char nome[Size_nome];
	char cpf[Size_cpf];
    char celular[Size_celular];
    char prof_responsavel[Size_prof_responsavel];
    char monitor_sn[Size_monitor_sn];
    char evento [Size_evento];
	int modificado;
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
//#region [ rgba(150, 20, 20, 0.5)]
    printf("Insira a data (DD-MM-AAAA):\n");
    scanf("%10s", aluguel.data);
    
    printf("Insira a sala (B02, I15, ...):\n");
    scanf("%3s", aluguel.sala);
    
    printf("Insira o horario (formato 710 para 7:10):\n");
    scanf("%d", &aluguel.horario);

    printf("Nome do solicitante:\n");
    scanf(" %49[^\n]", aluguel.nome);
    
    printf("Digite o CPF do solicitante:\n");
    scanf("%14s", aluguel.cpf);
    
    printf("Insira a ocorrencia:\n");
    scanf(" %49[^\n]", aluguel.evento);
    
    printf("Celular para contato:\n");
    scanf("%19s", aluguel.numero);
    
//#endregion

    aluguel.modificado = 0;
//#region [ rgba(0, 205, 30, 0.1)]
    //Teste de conflito de agendamentos
    rewind(pont_csv); //Ponteiro de leitura retorna ao comeco
    while (fscanf(pont_csv, "%"  #Size_data  "[^;];%" #Size_sala  "[^;];%" #Size_horario  "[^\n]\n",
                  aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario) == 3) {
        if (compDataHorSala(aluguel.data, aluguel_existente.data, aluguel.sala, aluguel_existente.sala, aluguel.horario, aluguel_existente.horario)) {
            printf("Erro: Esse horario ja foi agendado. :C\n");
            fclose(pont_csv);
            return;
        }
    }
//#endregion
    //Se deu certo, adiciona o novo registro
    fprintf(pont_csv, "%s;%s;%s;%s;%s;%s;%s;%d\n", 
            aluguel.data, aluguel.sala, aluguel.horario, aluguel.nome, 
            aluguel.cpf, aluguel.numero, aluguel.evento, aluguel.modificado);
    
    printf("Aluguel adicionado ! C:\n");

    fclose(pont_csv);
}

//Funcao para atualizar as informacoes associadas a um registro (nao altera data nem horario. Necessita-se criar outra funcao)
void attAlug(const char *nome_do_csv, const char *data, const char *sala, int horario) {
    FILE *pont_csv = fopen(nome_do_csv, "r");
    FILE *pont_temp = fopen("temp.csv", "w");
    struct Alug aluguel;
    int encontrou = 0;

    if (pont_csv == NULL) {
        printf("Arquivo de registro não encontrado. :C\n");
        return;
    }
//#region [ rgba(0, 205, 30, 0.1)]
    //Escaneia linha a linha procurando o registro desejado
    while (fscanf(pont_csv, "%"  #Size_data  "[^;];%" #Size_sala  "[^;];%" #Size_horario  "[^\n]\n",
                  aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario) == 3) {//#endregion
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
                aluguel.data, aluguel.sala, aluguel.horario, aluguel.nome, 
                aluguel.cpf, aluguel.numero, aluguel.modificado);
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
//#region [ rgba(0, 205, 30, 0.1)]
    while (fscanf(pont_csv, "%"  #Size_data  "[^;];%" #Size_sala  "[^;];%" #Size_horario  "[^\n]\n",
                  aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario) == 3) {
        
        if (compDataHorSala(aluguel.data, data, aluguel.sala, sala, aluguel.horario, horario)) {
            encontrou = 1;
            continue; //Nao transcreve o registro escolhido e sinaliza que o achou
        }
        //#endregion
        //Transcreve os dados para o arquivo temporario
        fprintf(pont_temp, "%s;%s;%s;%s;%s;%s;%s;%d\n", 
                aluguel.data, aluguel.sala, aluguel.horario, aluguel.nome, 
                aluguel.cpf, aluguel.numero, aluguel.evento, aluguel.modificado);
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
    char data[Size_data], sala[Size_sala], horario[Size_horario];

    do {
        printf("Insira SOMENTE o numero:\n");
        printf("1 para ADICIONAR um registro;\n");
        printf("2 para ATUALIZAR um registro;\n");
        printf("3 para REMOVER um registro;\n");
        printf("0 para SAIR do programa.\n");
        printf("Opcao selecionada: ");
        scanf("%d", &opcao);
//#region [ rgba(0, 0, 90, 0.5)] nao somente esse mas outras partes com scanf preciso implementar o sizeof
        switch (opcao) {
            case 1:
                addAlug("alugueis.csv");
                break;
            case 2:
                printf("Digite a data (AAAA-MM-DD):\n");
                scanf("%10s", data);

                printf("Insira a sala (B02, I15, ...):\n");
                scanf("%3s", sala);

                printf("Insira o horario (formato 710 para 7:10):\n");
                scanf("%d", &horario);

                attAlug("alugueis.csv", data, sala, horario);
                break;
            case 3:
                printf("Digite a data (AAAA-MM-DD):\n");
                scanf("%10s", data);

                printf("Insira a sala (B02, I15, ...):\n");
                scanf("%3s", sala);

                printf("Insira o horario (formato 710 para 7:10):\n");
                scanf("%d", &horario);

                removAlug("alugueis.csv", data, sala, horario);
                break; //#endregion
            case 0:
                printf("Programa encerrado.\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// Definindo os tamanhos para cada campo
#define Size_data 50
#define Size_sala 5
#define Size_horario 5
#define Size_nome 51
#define Size_cpf 13
#define Size_numero 20
#define Size_prof_responsavel 50
#define Size_monitor_sn 2
#define Size_evento 1000

#define fprintfTudo "%s,%s,%s,%s,%s,%s,%s,%s,%s,%d\n"

// Declarando funções
int compDataHorSala(const char *data1, const char *data2, const char *sala1, const char *sala2, const char *horario1, const char *horario2);
void addAlug(const char *data_do_csv, const char *data);
int verifCriaArquivo;
void attAlug(const char *data_do_csv, const char *data, const char *sala, const char *horario);
void removAlug(const char *data_do_csv, const char *data, const char *sala, const char *horario);
int registrador();
int TrocarLouO (char Data_T[], char Sala_T[], char Hora_T[]);
void AdicionarSala ();
void RemoverSala ();
void PlanilhaDefaultExistinator ();

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

//Comparacao de conflitos
int compDataHorSala(const char *data1, const char *data2, const char *sala1, const char *sala2, const char *horario1, const char *horario2) {
    if (strcmp(data1, data2) == 0 && strcmp(sala1, sala2) == 0 && strcmp(horario1, horario2) == 0) {
        return 1;  // Retorna 1 se todas as comparações forem verdadeiras
    }
    return 0;  // Caso contrário, retorna 0
}

//Verificacao para o addAlug, apenas caso o arquivo nao exista ainda
int verificarCriarArquivo(const char *data_do_csv) {
    FILE *pont_csv = fopen(data_do_csv, "r+");

    if (pont_csv == NULL) {
        pont_csv = fopen(data_do_csv, "a+");

        if (pont_csv == NULL) {
            printf("Erro ao tentar criar o arquivo.\n");
            return 0;
        }
        
        fclose(pont_csv);
    } else {
        fclose(pont_csv);
    }

    return 1; //criou um arquivo novo
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

//Adicao de registro
void addAlug(const char *data_do_csv, const char *data) {
    struct aluguel aluguel, aluguel_existente;

    if (!verificarCriarArquivo(data_do_csv)) {
        return;
    }

    FILE *pont_csv = fopen(data_do_csv, "r+"); //ponteiro spawna no começo do arquivo
	
	strcpy(aluguel.data, data);
	// Deixar como obrigatório preencher aqui, != de \n ou apenas \0 ou != de 8 characteres

    printf("Insira a sala (B02, I15, ...): ");
        fgets(aluguel.sala, Size_sala, stdin);
        aluguel.sala[strcspn(aluguel.sala, "\n")] = '\0';
        if (strlen(aluguel.sala) == Size_sala - 1 && aluguel.sala[Size_sala - 2] != '\n') {
        limparBuffer();
        }
// Deixar como obrigatório preencher aqui, != de \n ou apenas \0
	
    printf("Insira o horario (formato 710 para 7:10): ");
        fgets(aluguel.horario, Size_horario, stdin);
        aluguel.horario[strcspn(aluguel.horario, "\n")] = '\0';
        if (strlen(aluguel.horario) == Size_horario - 1 && aluguel.horario[Size_horario - 2] != '\n') {
        limparBuffer();
        }
// Deixar como obrigatório preencher aqui, != de \n ou apenas \0

    printf("Nome do solicitante: ");
        fgets(aluguel.nome, Size_nome, stdin);
        aluguel.nome[strcspn(aluguel.nome, "\n")] = '\0';
        if (strlen(aluguel.nome) == Size_nome - 1 && aluguel.nome[Size_nome - 2] != '\n') {
        limparBuffer();
        }

    printf("Digite o CPF do solicitante: ");
        fgets(aluguel.cpf, Size_cpf, stdin);
        aluguel.cpf[strcspn(aluguel.cpf, "\n")] = '\0';
        if (strlen(aluguel.cpf) == Size_cpf - 1 && aluguel.cpf[Size_cpf - 2] != '\n') {
        limparBuffer();
        }

    printf("Celular para contato: ");
        fgets(aluguel.celular, Size_numero, stdin);
        aluguel.celular[strcspn(aluguel.celular, "\n")] = '\0';
        if (strlen(aluguel.celular) == Size_numero - 1 && aluguel.celular[Size_numero - 2] != '\n') {
        limparBuffer();
        }

    printf("Professor responsavel: ");
        fgets(aluguel.prof_responsavel, Size_prof_responsavel, stdin);
        aluguel.prof_responsavel[strcspn(aluguel.prof_responsavel, "\n")] = '\0';
        if (strlen(aluguel.prof_responsavel) == Size_prof_responsavel - 1 && aluguel.prof_responsavel[Size_prof_responsavel - 2] != '\n') {
        limparBuffer();
        }

    printf("Monitor? (S/N): ");
        fgets(aluguel.monitor_sn, Size_monitor_sn, stdin);
        aluguel.monitor_sn[strcspn(aluguel.monitor_sn, "\n")] = '\0';
        if (strlen(aluguel.monitor_sn) == Size_monitor_sn - 1 && aluguel.monitor_sn[Size_monitor_sn - 2] != '\n') {
        limparBuffer();
        }

    printf("Insira a ocorrencia: ");
        fgets(aluguel.evento, Size_evento, stdin);
        aluguel.evento[strcspn(aluguel.evento, "\n")] = '\0';
        if (strlen(aluguel.evento) == Size_evento - 1 && aluguel.evento[Size_evento - 2] != '\n') {
        limparBuffer();
        }


    aluguel.modificado = 0;

    //teste de conflito
    rewind(pont_csv);
    while (fscanf(pont_csv, "%10[^;];%4[^;];%4[^;];",
              aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario) == 3) {
        if (compDataHorSala(aluguel.data, aluguel_existente.data, aluguel.sala, aluguel_existente.sala, aluguel.horario, aluguel_existente.horario)) {
            printf("Erro: Esse horario ja foi agendado. :C\n");
            fclose(pont_csv);
            return;
        }
    }

    fseek(pont_csv, 0, SEEK_END);

    //CHECK DE INFORMAÇOES

    char dataCheck[Size_data];
    strcpy(dataCheck, data);
    //copiar a const pra mutavel pra conseguir usar no check

    int check_de_erro = TrocarLouO (dataCheck, aluguel.sala, aluguel.horario); 

    if (check_de_erro==0){ //faz o check de erro, se der tudo certo, retornando 0, printa no final e usa o L ou O dentro do proprio check de erro
        fprintf(pont_csv, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%d\n", 
            aluguel.data, aluguel.sala, aluguel.horario, aluguel.nome, 
            aluguel.cpf, aluguel.celular, aluguel.prof_responsavel, aluguel.monitor_sn, 
            aluguel.evento, aluguel.modificado);
        printf("Aluguel adicionado! :)\n");
    }
    else{
        printf("Tente novamente. Verifique se as informacoes estao validas.");
        return;
    }

    fclose(pont_csv);
}

//Atualizacao de registro
void attAlug(const char *data_do_csv, const char *data, const char *sala, const char *horario) {
    FILE *pont_csv = fopen(data_do_csv, "r");
    FILE *pont_temp = fopen("temp.csv", "w");

    struct aluguel aluguel, aluguel_existente, aluguel_novo, aluguel_conflito;
    int tudocerto = 0;

    printf("Tentando abrir o arquivo: %s\n", data_do_csv);

    if (pont_csv == NULL || pont_temp == NULL) {
        printf("Erro ao procurar o arquivo, insira uma data valida ou tente novamente. :C\n");
        if (pont_csv) fclose(pont_csv);
        if (pont_temp) fclose(pont_temp);
        return;
    }

    while (fscanf(pont_csv, "%10[^;];%4[^;];%4[^;];%49[^;];%12[^;];%19[^;];%49[^;];%1[^;];%999[^;];%d\n",
                  aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario,
                  aluguel_existente.nome, aluguel_existente.cpf, aluguel_existente.celular,
                  aluguel_existente.prof_responsavel, aluguel_existente.monitor_sn,
                  aluguel_existente.evento, &aluguel_existente.modificado) == 10) {

        long posicao_p_csv = ftell(pont_csv);

    if (compDataHorSala(aluguel_existente.data, data, aluguel_existente.sala, sala, aluguel_existente.horario, horario)) {
                        aluguel_conflito = aluguel_existente;
        printf(":D Registro encontrado! Insira as novas informações ou pressione ENTER para manter os valores atuais:\n");

            printf("Data [%s]: ", aluguel_conflito.data);
            fgets(aluguel_novo.data, Size_data, stdin);
            aluguel_novo.data[strcspn(aluguel_novo.data, "\n")] = '\0';
            if (strlen(aluguel_novo.data) == 0) {
                strcpy(aluguel_novo.data, aluguel_conflito.data);
            }
            limparBuffer();

            printf("Sala [%s]: ", aluguel_conflito.sala);
            fgets(aluguel_novo.sala, Size_sala, stdin);
            aluguel_novo.sala[strcspn(aluguel_novo.sala, "\n")] = '\0';
            if (strlen(aluguel_novo.sala) == 0) {
                strcpy(aluguel_novo.sala, aluguel_conflito.sala);
            }
            limparBuffer();

            printf("Horario [%s]: ", aluguel_conflito.horario);
            fgets(aluguel_novo.horario, Size_horario, stdin);
            aluguel_novo.horario[strcspn(aluguel_novo.horario, "\n")] = '\0';
            if (strlen(aluguel_novo.horario) == 0) {
                strcpy(aluguel_novo.horario, aluguel_conflito.horario);
            }
            limparBuffer();

            printf("Nome do solicitante [%s]: ", aluguel_conflito.nome);
            fgets(aluguel_novo.nome, Size_nome, stdin);
            aluguel_novo.nome[strcspn(aluguel_novo.nome, "\n")] = '\0';
            if (strlen(aluguel_novo.nome) == 0) {
                strcpy(aluguel_novo.nome, aluguel_conflito.nome);
            }

            limparBuffer();

            printf("CPF [%s]: ", aluguel_conflito.cpf);
            fgets(aluguel_novo.cpf, Size_cpf, stdin);
            aluguel_novo.cpf[strcspn(aluguel_novo.cpf, "\n")] = '\0';
            if (strlen(aluguel_novo.cpf) == 0) {
                strcpy(aluguel_novo.cpf, aluguel_conflito.cpf);
            }
            limparBuffer();

            printf("Celular para contato [%s]: ", aluguel_conflito.celular);
            fgets(aluguel_novo.celular, Size_numero, stdin);
            aluguel_novo.celular[strcspn(aluguel_novo.celular, "\n")] = '\0';
            if (strlen(aluguel_novo.celular) == 0) {
                strcpy(aluguel_novo.celular, aluguel_conflito.celular);
            }
            limparBuffer();

            printf("Professor responsável [%s]: ", aluguel_conflito.prof_responsavel);
            fgets(aluguel_novo.prof_responsavel, Size_prof_responsavel, stdin);
            aluguel_novo.prof_responsavel[strcspn(aluguel_novo.prof_responsavel, "\n")] = '\0';
            if (strlen(aluguel_novo.prof_responsavel) == 0) {
                strcpy(aluguel_novo.prof_responsavel, aluguel_conflito.prof_responsavel);
            }
            limparBuffer();

            printf("Monitor? (S/N) [%s]: ", aluguel_conflito.monitor_sn);
            fgets(aluguel_novo.monitor_sn, Size_monitor_sn, stdin);
            aluguel_novo.monitor_sn[strcspn(aluguel_novo.monitor_sn, "\n")] = '\0';
            if (strlen(aluguel_novo.monitor_sn) == 0) {
                strcpy(aluguel_novo.monitor_sn, aluguel_conflito.monitor_sn);
            }
            limparBuffer();

            printf("Ocorrência [%s]: ", aluguel_conflito.evento);
            fgets(aluguel_novo.evento, Size_evento, stdin);
            aluguel_novo.evento[strcspn(aluguel_novo.evento, "\n")] = '\0';
            if (strlen(aluguel_novo.evento) == 0) {
                strcpy(aluguel_novo.evento, aluguel_conflito.evento);
            }
            limparBuffer();

            aluguel_novo.modificado = 1;

            // Verificação de conflitos com novos dados
            rewind(pont_csv); // Volta para o início do arquivo para verificar o arquivo inteiro
            while (fscanf(pont_csv, "%10[^;];%4[^;];%4[^;];%*[^\n]\n", 
                          aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario) == 3) {
    
                // Verifica se aluguel_existente é igual a aluguel_conflito
                if (compDataHorSala(aluguel_existente.data, aluguel_conflito.data, 
                                    aluguel_existente.sala, aluguel_conflito.sala, 
                                    aluguel_existente.horario, aluguel_conflito.horario)) {
                    //Ignorando linha correspondente a aluguel_conflito.\n")
                    continue;
                }

                //Verifica possivel conflito com aluguel_novo
                if (compDataHorSala(aluguel_novo.data, aluguel_existente.data, 
                        aluguel_novo.sala, aluguel_existente.sala, 
                        aluguel_novo.horario, aluguel_existente.horario)) {
                    printf("O horário e data correspondentes para essa sala já foram reservados. :C\n");
                    fclose(pont_csv);
                    fclose(pont_temp);
                    remove("temp.csv");
                    return;
                }
            }


            // Se não houver conflito, escreve os novos dados no arquivo temporário
            fprintf(pont_temp, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%d\n", 
            aluguel_novo.data, aluguel_novo.sala, aluguel_novo.horario, aluguel_novo.nome, 
            aluguel_novo.cpf, aluguel_novo.celular, aluguel_novo.prof_responsavel, aluguel_novo.monitor_sn, 
            aluguel_novo.evento, aluguel_novo.modificado);

            //check de erro na planilha com os novos dados
            int check_de_erro = TrocarLouO (aluguel_novo.data, aluguel_novo.sala, aluguel_novo.horario); 

            if (check_de_erro==0){ //faz o check de erro, se der tudo certo, retornando 0, printa no final e usa o L ou O dentro do proprio check de erro
                printf("Informacoes validadas...");
                tudocerto = 1;
            }
            else{
                printf("Tente novamente. Verifique se as informacoes estao validas.");
                return;
            }

        } else {
            // Se não for o registro para alterar, copia o registro atual para o arquivo temporário
            fprintf(pont_temp, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%d\n", 
            aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario, aluguel_existente.nome, 
            aluguel_existente.cpf, aluguel_existente.celular, aluguel_existente.prof_responsavel, aluguel_existente.monitor_sn, 
            aluguel_existente.evento, aluguel_existente.modificado);
        }
    }

    if (pont_csv != NULL) {
    fclose(pont_csv);
    }

    if (pont_temp != NULL) {
    fclose(pont_temp);
    }

    if (tudocerto) {
        remove(data_do_csv);
        rename("temp.csv", data_do_csv);
        printf("Registro atualizado! C:\n");
    } else {
        printf("Registro não encontrado. :C\n");
        remove("temp.csv");
    }
}

//Remover registro
void removAlug(const char *data_do_csv, const char *data, const char *sala, const char *horario) {
    FILE *pont_csv = fopen(data_do_csv, "r");
    FILE *pont_temp = fopen("temp.csv", "w");

    struct aluguel aluguel_existente;
    int tudocerto = 0;
    
    printf("Tentando abrir o arquivo: %s\n", data_do_csv);

    if (pont_csv == NULL || pont_temp == NULL) {
        printf("Erro ao procurar o arquivo, insira uma data valida ou tente novamente. :C\n");
        if (pont_csv) fclose(pont_csv);
        if (pont_temp) fclose(pont_temp);
        return;
    }

    //Lendo arquivo existente
    while (fscanf(pont_csv, "%10[^;];%4[^;];%4[^;];%49[^;];%12[^;];%19[^;];%49[^;];%1[^;];%999[^;];%d\n",
                  aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario,
                  aluguel_existente.nome, aluguel_existente.cpf, aluguel_existente.celular,
                  aluguel_existente.prof_responsavel, aluguel_existente.monitor_sn,
                  aluguel_existente.evento, &aluguel_existente.modificado) == 10) {

        //Verifica se o registro atual corresponde ao que deve ser removido
        if (compDataHorSala(aluguel_existente.data, data, aluguel_existente.sala, sala, aluguel_existente.horario, horario)) {
            printf("Registro encontrado! :D\nData: %s, Sala: %s, Horário: %s\n",
                   aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario);
            continue; //Nao escreve no temporario

            //verificacao de validez pra planilha do cagador de cama

            char dataCheck[Size_data], salaCheck[Size_sala], horarioCheck[Size_horario];
            strcpy(dataCheck, data);
            strcpy(salaCheck, sala);
            strcpy(horarioCheck, horario);
            //copiar a const pra mutavel pra conseguir usar no check

            int check_de_erro = TrocarLouO (dataCheck, salaCheck, horarioCheck); 

            if (check_de_erro==0){ //faz o check de erro, se der tudo certo, retornando 0, printa no final e usa o L ou O dentro do proprio check de erro
                printf("Aluguel removido! :)\n");
                tudocerto = 1;
            }
            else{
                printf("Tente novamente. Verifique se as informacoes estao validas.");
                return;
            }

        }

        //Copia os outros registros existentes para o arquivo temporário
        fprintf(pont_temp, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%d\n",
                aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario,
                aluguel_existente.nome, aluguel_existente.cpf, aluguel_existente.celular,
                aluguel_existente.prof_responsavel, aluguel_existente.monitor_sn,
                aluguel_existente.evento, aluguel_existente.modificado);
    }

    //Fecha os arquivos se estiverem abertos
    if (pont_csv != NULL) {
        fclose(pont_csv);
    }
    if (pont_temp != NULL) {
        fclose(pont_temp);
    }

    if (tudocerto) {
        remove(data_do_csv);
        rename("temp.csv", data_do_csv);
        printf("Registro removido com sucesso! :)\n");
    } else {
        remove("temp.csv");
        printf("Registro não encontrado. :C\n");
    }
}

int registrador() {
    int opcao;
    char data[Size_data], sala[Size_sala], horario[Size_horario], data_do_csv[Size_data + 5];

    do {
        printf("Selecione uma opcao:\n");
        printf("1 - ADICIONAR um REGISTRO;\n");
        printf("2 - ATUALIZAR um REGISTRO;\n");
        printf("3 - REMOVER um REGISTRO;\n");
		printf("4 - ADICIONAR uma SALA aos horarios;\n");
		printf("5 - REMOVER uma SALA dos horarios;\n");
        printf("0 - SAIR.\n");
        printf("Opcao escolhida: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:

                printf("Data (DD-MM-AAAA): ");
                fgets(data, Size_data, stdin);
                data[strcspn(data, "\n")] = '\0';
                if (strlen(data) == Size_data - 1 && data[Size_data - 2] != '\n') {
                    limparBuffer();
                }

                sprintf(data_do_csv, "%s.csv", data);

                addAlug(data_do_csv, data);
                break;
            case 2:

                printf("Data (DD-MM-AAAA): ");
                fgets(data, Size_data, stdin);
                data[strcspn(data, "\n")] = '\0';
                if (strlen(data) == Size_data - 1 && data[Size_data - 2] != '\n') {
                    limparBuffer();
                }

                sprintf(data_do_csv, "%s.csv", data);

                printf("Sala (B02, I15, ...): ");
                fgets(sala, Size_sala, stdin);
                sala[strcspn(sala, "\n")] = '\0';
                if (strlen(sala) == Size_sala - 1 && sala[Size_sala - 2] != '\n') {
                    limparBuffer();
                }

                printf("Horario (formato 710 para 7:10): ");
                fgets(horario, Size_horario, stdin);
                horario[strcspn(horario, "\n")] = '\0';
                if (strlen(horario) == Size_horario - 1 && horario[Size_horario - 2] != '\n') {
                    limparBuffer();
                }

                    attAlug(data_do_csv, data, sala, horario);
                    break;
            case 3:

                printf("Data (DD-MM-AAAA): ");
                fgets(data, Size_data, stdin);
                data[strcspn(data, "\n")] = '\0';

                if (strlen(data) == Size_data - 1 && data[Size_data - 2] != '\n') {
                    limparBuffer();
                }

                sprintf(data_do_csv, "%s.csv", data);

                printf("Sala (B02, I15, ...): ");
                fgets(sala, Size_sala, stdin);

                if (strlen(sala) == Size_sala - 1 && sala[Size_sala - 2] != '\n') {
                    while (getchar() != '\n');
                }

                printf("Horario (formato 710 para 7:10): ");
                fgets(horario, Size_horario, stdin);
                horario[strcspn(horario, "\n")] = '\0';

                if (strlen(horario) == Size_horario - 1 && horario[Size_horario - 2] != '\n') {
                    while (getchar() != '\n');
                }

                    removAlug(data_do_csv, data, sala, horario);
                    break;
			case 4:
				AdicionarSala ();
				break;
			case 5:
				RemoverSala ();
				break;
            case 0:
                printf("Programa encerrado.\n");
                Sleep(1000);
                break;
            default:
                printf("Opçao invalida.\n");
        }
    } while (opcao != 0);

}

//######################################################################################################################################
//######################################################################################################################################
//######################################################################################################################################
int TrocarLouO (char Data_T[], char Sala_T[], char Hora_T[])
{
    int pode_registrar = 0; // Se der um erro não deixa registrar
	FILE *Ponteiro_Arquivo; // Aponta para um arquivo
	int Size_ID_planilha = 13;
	char ID_do_arquivo[Size_ID_planilha]; // Array que recebe o nome do Arquivo, formato exemplo: 112024.csv

	strcpy(ID_do_arquivo, Data_T); //input do usuario para ID_do_arquivo

    //printf("_%s_\n",ID_do_arquivo); //DEBUG

	char comeco_nome[] = "planilha";
	int lencomeco = strlen(comeco_nome);
	int lenID = strlen(ID_do_arquivo);

	// Tamanho do resultado suficiente para caber os dois
	char resultado[lencomeco + lenID + 1]; //

    // Copia a primeira array para a array resultado
    for (int i = 0; i < lencomeco; i++)
    {
        resultado[i] = comeco_nome[i];
    }

    // Copia a segunda array para a array resultado, depois da primeira
    for (int i = 0; i < lenID; i++)
    {
        resultado[lencomeco + i] = ID_do_arquivo[i];
    }

    // Adiciona \0 no final das arrays somadas
    resultado[lencomeco + lenID] = '\0';
    strcat(resultado, ".csv");
	//printf("_%s_\n", resultado); //DEBUG

	Ponteiro_Arquivo = fopen(resultado, "r+"); //############## ABRE O ARQUIVO ##############
	if (Ponteiro_Arquivo == NULL)
	{
        Ponteiro_Arquivo = fopen(resultado, "w");
		FILE *Ler_default = fopen("planilhadefault.csv", "r");

		if (Ler_default == NULL)
		{
            PlanilhaDefaultExistinator();
		}

		char buffer[1024];  // Buffer temporario
    		size_t bytes_lidos;
		while ((bytes_lidos = fread(buffer, 1, sizeof(buffer), Ler_default)) > 0)
		{
            fwrite(buffer, 1, bytes_lidos, Ponteiro_Arquivo);
        }

		fclose(Ler_default); //fechar os arquivos
		fclose(Ponteiro_Arquivo); //fechar por garantia, só para ter certeza que vai funcionar

		Ponteiro_Arquivo = fopen(resultado, "r+");
		rewind(Ponteiro_Arquivo);
		if (Ponteiro_Arquivo == NULL)
		{
			perror("Erro ao abrir o arquivo, verifique o local do arquivo");
	       		return 0;
		}
	}

////// Parte de modifica horários ###########################################################

    char nova_sala[Size_nome]; // SALA ##############################
	strcpy(nova_sala, Sala_T);
	for (int i = 0; i < strlen(nova_sala); i++)
	{
        if (nova_sala[i] == '\n')
		{
            nova_sala[i] = '\0';
		}
	}

	// HORARIO ##############################
    // Converte a string para um int
	int hora_b = (Hora_T[0] - '0') * 10 + (Hora_T[1] - '0'); //converte char pra int - horas
	int minuto_b = (Hora_T[2] - '0') * 10 + (Hora_T[3] - '0'); //converte char pra int - minutos
	int bloco_horario = (hora_b * 60) + minuto_b;
	// Calcular os intervalos de 50 minutos
    bloco_horario = ((bloco_horario - 430) / 50) * 2; // Começa a contar a partir do 0, *2 por conta do ';'
	//printf("_1BLOH_%i_\n", bloco_horario);

    //Verificar se a sala já existe ###################################################
    rewind(Ponteiro_Arquivo); //Ponteiro de leitura retorna ao comeco do arquivo

	char sala_presente[Size_nome];
	int ja_registrado = 0;
	int posicao_horario = 1;

    while (fscanf(Ponteiro_Arquivo, "%20[^;]", sala_presente) == 1) //#####################################################
	{
		size_t len = strcspn(sala_presente, "\n");
		if (sala_presente[len] == '\n')
		{
		// Move os caracteres uma posição para a esquerda para remover o \n
		memmove(sala_presente + len, sala_presente + len + 1, strlen(sala_presente) - len);
		posicao_horario++; // contar a passagem de linha
		}

		fscanf(Ponteiro_Arquivo, "%*c");
		//printf("_%sfim\n", sala_presente);

        if (strcmp(nova_sala, sala_presente) == 0)
		{
            printf("Sala encontrada\n");
			ja_registrado = 1;
            break;
        }
    }//####################################################################################################################

	if (ja_registrado == 0) //Verificar se a sala existe
	{
		printf("Foi selecionada uma sala nao registrada\n");
		pode_registrar = 1;
		return pode_registrar;
	}

    long int Posicao_leitura = ftell(Ponteiro_Arquivo);

	fseek(Ponteiro_Arquivo, Posicao_leitura + bloco_horario, SEEK_SET);

	char char_atual = fgetc(Ponteiro_Arquivo);

    if (char_atual == EOF)
	{
        printf("Erro: Chegou ao EOF (final do arquivo) ou a uma posicao invalida.\n");
        fclose(Ponteiro_Arquivo);
        pode_registrar = 1;
        fclose(Ponteiro_Arquivo);
		return pode_registrar; // Retorna se a posicao for invalida
    }

    // Checar de o charactere é 'L' ou 'O' e então mudar ###############################################
    if (char_atual == 'L')
	{
        fseek(Ponteiro_Arquivo, Posicao_leitura + bloco_horario, SEEK_SET);  // Seek a mesma posição
        fputc('O', Ponteiro_Arquivo);  // 'L' vira 'O'
        printf("Char na linha %d, Posicao %d, 'L' para 'O'.\n", posicao_horario, bloco_horario);
    }
    else if (char_atual == 'O')
	{
        fseek(Ponteiro_Arquivo, Posicao_leitura + bloco_horario, SEEK_SET);  // Seek a mesma posição
        fputc('L', Ponteiro_Arquivo);  // 'O' vira 'L'
        printf("Char na linha %d, Posicao %d, 'O' para 'L'.\n", posicao_horario, bloco_horario);
		printf("Char atual: _%c_ \n", char_atual);
    }
	else if (char_atual == ';') //DEBUG ou se algo de muito errado tiver acontecido
	{
        printf("Erro: Caractere na posicao %d eh um ';' \n", posicao_horario);
        fclose(Ponteiro_Arquivo);
        pode_registrar = 1;
		return pode_registrar;
    }
    else //DEBUG ou se algo de muito errado tiver acontecido parte 2
	{
        printf("Erro: Caractere na posicao %d nao eh uma 'L' ou 'O' ou ';'.\n", posicao_horario);
		printf("Char atual: _%c_ \n", char_atual); //DEBUG
		fclose(Ponteiro_Arquivo);
		pode_registrar = 1;
		return pode_registrar;
    }

	fflush(Ponteiro_Arquivo);
	fclose(Ponteiro_Arquivo); //############## FECHA O ARQUIVO ##############

	return 0;
}
//######################################################################################################################################
//######################################################################################################################################
//######################################################################################################################################

void AdicionarSala ()
{
	FILE *Ponteiro_Arquivo; // Aponta para um arquivo
	char ID_do_arquivo[20]; // Array que recebe o nome do Arquivo, formato exemplo: 112024

	strcpy(ID_do_arquivo, "planilhadefault.csv"); //Nome para ID_do_arquivo nesse caso é fixo por ser um único arquivo
	ID_do_arquivo[19] = '\0';

	Ponteiro_Arquivo = fopen(ID_do_arquivo, "a+"); //############## ABRE O ARQUIVO ##############
	if (Ponteiro_Arquivo == NULL)
	{
	        perror("Erro ao abrir o arquivo");
	        return;
	}
////// Parte de adicionar salas

	char nova_sala[Size_nome]; // SALA ##############################
	printf("Digite a sala: ");
	fgets(nova_sala, Size_nome, stdin);
	for (int i = 0; i < strlen(nova_sala); i++)
	{
        	if (nova_sala[i] == '\n')
		{
            		nova_sala[i] = '\0';
		}
	}

	//printf("_%s_\n", nova_sala); //DEBUG
    	//Verificar se a sala já existe
    	rewind(Ponteiro_Arquivo); //Ponteiro de leitura retorna ao comeco do arquivo

	char sala_presente[Size_nome];

	int ja_registrado = 0;
	while (fscanf(Ponteiro_Arquivo, "%20[^;]", sala_presente) == 1)
	{
		size_t len = strcspn(sala_presente, "\n");
		if (sala_presente[len] == '\n')
		{
			// Move os caracteres uma posição para a esquerda para remover o \n
			memmove(sala_presente + len, sala_presente + len + 1, strlen(sala_presente) - len);
		}

		fscanf(Ponteiro_Arquivo, "%*c");
		//printf("_%sfim\n", sala_presente);

        	if (strcmp(nova_sala, sala_presente) == 0)
		{
	            	printf("Essa sala ja foi registrada\n");
	            	fclose(Ponteiro_Arquivo);
			ja_registrado = 1;
            		break;
        	}
	}
	//printf("_%s_\n", nova_sala);
	char L[36];
	for (int i = 0; i < 37; i++)
	{
        	L[i] = (i % 2 == 0) ? ';' : 'L';
        }
	int size_linhadasala = strlen(nova_sala) + strlen(L);

	char linhadasala[size_linhadasala];
	strcpy(linhadasala, nova_sala);
	strcat(linhadasala, L);

	if (ja_registrado == 0)
	{
		fprintf(Ponteiro_Arquivo, "\n%s", linhadasala);
		fflush(Ponteiro_Arquivo);
		printf("A sala foi registrada\n");
		fclose(Ponteiro_Arquivo); //############## FECHA O ARQUIVO ##############
	}
	//printf("_%s_\n", linhadasala); //debug
	return;
}
//######################################################################################################################################
//######################################################################################################################################
//######################################################################################################################################
void RemoverSala ()
{
	FILE *Ponteiro_Arquivo; // Aponta para um arquivo
	char ID_do_arquivo[20]; // Array que recebe o nome do Arquivo, formato exemplo: 112024

	strcpy(ID_do_arquivo, "planilhadefault.csv"); //Nome para ID_do_arquivo nesse caso é fixo por ser um único arquivo
	ID_do_arquivo[20] = '\0';

	Ponteiro_Arquivo = fopen(ID_do_arquivo, "r+"); //############## ABRE O ARQUIVO ##############

	FILE *Pont_ArquivoTemp = fopen("ArquivoTemporario.csv", "w+");
	if (Pont_ArquivoTemp == NULL)
    {
        perror("Error opening file");
        return;
    }
////// Parte de encontrar e remover a sala

	char sala_para_remover[Size_nome]; // SALA ##############################
    printf("Digite a sala a ser removida: ");
	fgets(sala_para_remover, Size_nome, stdin);
	for (int i = 0; i < strlen(sala_para_remover); i++)
	{
        if (sala_para_remover[i] == '\n')
		{
            sala_para_remover[i] = '\0';
		}
	}
	//printf("_%s_\n", sala_para_remover); //DEBUG

    //Verificar se a sala já existe
    rewind(Ponteiro_Arquivo); //Ponteiro de leitura retorna ao comeco do arquivo

	char sala_presente[Size_nome];

	int ja_registrado = 0;
    int qual_linha_remover = 1;
	while (fscanf(Ponteiro_Arquivo, "%20[^;]", sala_presente) == 1)
	{
		size_t len = strcspn(sala_presente, "\n");
		if (sala_presente[len] == '\n')
		{
			// Move os caracteres uma posição para a esquerda para remover o \n
			memmove(sala_presente + len, sala_presente + len + 1, strlen(sala_presente) - len);
			qual_linha_remover++;
		}

		fscanf(Ponteiro_Arquivo, "%*c");
		//printf("_%sfim\n", sala_presente);

        if (strcmp(sala_para_remover, sala_presente) == 0)
		{
	        printf("Sala encontrada\n");
			ja_registrado = 1;
            break;
        }
	}
	//printf("_%s_\n", sala_para_remover);

//####################################################

	if (ja_registrado == 1)
    {
        rewind(Ponteiro_Arquivo);
		char buffer[4096];

        int Linha_atual = 1;
        // Lê linha por linha do arquivo original
        while (fgets(buffer, sizeof(buffer), Ponteiro_Arquivo) != NULL)
        {
        // Escreve a linha no arquivo temporário se não for a linha que quer remover
            if (Linha_atual != qual_linha_remover)
            {
                fputs(buffer, Pont_ArquivoTemp);
				printf("%s",buffer); //debug
            }
			Linha_atual++;
        }

        // Fecha os dois arquivos
        fclose(Ponteiro_Arquivo);
        fclose(Pont_ArquivoTemp);

		// Agora essa parte copia o conteudo que foi mandado para o arquivo temporario para o default que foi limpo
		Ponteiro_Arquivo = fopen(ID_do_arquivo, "w+"); //############## ABRE O ARQUIVO ##############

		FILE *Pont_ArquivoTemp = fopen("ArquivoTemporario.csv", "r");
		if (Pont_ArquivoTemp == NULL)
		{
			perror("Error opening file");
			return;
		}

		char ultima_Linha[1024]; // Guardar a ultima linha do arquivo
		// Ler todas as linhas no buffer de ultima linha
		while (fgets(buffer, sizeof(buffer), Pont_ArquivoTemp) != NULL)
		{
			strcpy(ultima_Linha, buffer); // Guarda a ultima linha
		}

		rewind(Pont_ArquivoTemp);
        // Lê linha por linha do arquivo temporario
        while (fgets(buffer, sizeof(buffer), Pont_ArquivoTemp) != NULL)
        {
			if (strcmp(buffer, ultima_Linha) == 0)
			{
            // Remove a nova linha da ultima posicao se ela estiver lá
            size_t buffer_len = strlen(buffer);
				if (buffer_len > 0 && buffer[buffer_len - 1] == '\n')
				{
                buffer[buffer_len - 1] = '\0'; // Remove a nova linha
				}
			}
			// Escreve a linha no arquivo original
			fputs(buffer, Ponteiro_Arquivo);
        }
		fclose(Ponteiro_Arquivo);
        fclose(Pont_ArquivoTemp);
		printf("Sala na linha %d foi removida.\n", qual_linha_remover);
    }

	if (ja_registrado == 0)
	{
		printf("Sala nao foi encontrada, digite uma sala valida.\n");
    }

    // Fim da função
    return;
}
//######################################################################################################################################
//######################################################################################################################################
//######################################################################################################################################
void printPlanilha()
{
	FILE *Ponteiro_Arquivo; // Aponta para um arquivo
	int Size_ID_planilha = 13;
	char ID_do_arquivo[Size_ID_planilha]; // Array que recebe o nome do Arquivo, formato exemplo: 112024.csv

    char Data_ID[Size_data];
    printf("Digite a data (DD-MM-AAAA): ");
    fgets(Data_ID, Size_data, stdin);
    Data_ID[strcspn(Data_ID, "\n")] = '\0';

	strcpy(ID_do_arquivo, Data_ID); //input do usuario para ID_do_arquivo

    //printf("_%s_\n",ID_do_arquivo); //DEBUG

	char comeco_nome[] = "planilha";
	int lencomeco = strlen(comeco_nome);
	int lenID = strlen(ID_do_arquivo);

	// Tamanho do resultado suficiente para caber os dois
	char resultado[lencomeco + lenID + 1]; //

    // Copia a primeira array para a array resultado
    for (int i = 0; i < lencomeco; i++)
    {
        resultado[i] = comeco_nome[i];
    }

    // Copia a segunda array para a array resultado, depois da primeira
    for (int i = 0; i < lenID; i++)
    {
        resultado[lencomeco + i] = ID_do_arquivo[i];
    }

    // Adiciona \0 no final das arrays somadas
    resultado[lencomeco + lenID] = '\0';
    strcat(resultado, ".csv");
	//printf("_%s_\n", resultado); //DEBUG

	Ponteiro_Arquivo = fopen(resultado, "r+"); //############## ABRE O ARQUIVO ##############
	if (Ponteiro_Arquivo == NULL)
	{
        Ponteiro_Arquivo = fopen(resultado, "w");
		FILE *Ler_default = fopen("planilhadefault.csv", "r");

		if (Ler_default == NULL)
		{
            PlanilhaDefaultExistinator();
		}

		char buffer[1024];  // Buffer temporario
    		size_t bytes_lidos;
		while ((bytes_lidos = fread(buffer, 1, sizeof(buffer), Ler_default)) > 0)
		{
            fwrite(buffer, 1, bytes_lidos, Ponteiro_Arquivo);
        }

		fclose(Ler_default); //fechar os arquivos
		fclose(Ponteiro_Arquivo); //fechar por garantia, só para ter certeza que vai funcionar

		Ponteiro_Arquivo = fopen(resultado, "r+");
		rewind(Ponteiro_Arquivo);
		if (Ponteiro_Arquivo == NULL)
		{
			perror("Erro ao abrir o arquivo, verifique o local do arquivo");
	       	return;
		}
	}
	char Linha [80];
	while((fgets(Linha, sizeof(Linha), Ponteiro_Arquivo)) != NULL) // Print no terminal o que contém o arquivo linha por linha
		{
			printf("%s",Linha);
		}

	fclose(Ponteiro_Arquivo); //############## FECHA O ARQUIVO ##############

	return;
}
//######################################################################################################################################
//######################################################################################################################################
//######################################################################################################################################
void PlanilhaDefaultExistinator() //Necessário para criar a plannilha default se ela não existir
{
	FILE *Pont_Pdefault;
    const char *NomePdefault = "planilhadefault.csv";
    const char *PlanilhaDefaultTexto =
	" ;07:10-08:00;08:00-08:50;08:50-09:40;09:40-10:30;10:30-11:20;11:20-12:10;12:10-13:00;13:00-13:50;13:50-14:40;14:40-15:30;15:30-16:20;16:20-17:10;17:10-18:00;18:00-18:50;18:50-19:40;19:40-20:30;20:30-21:20;21:20-22:10;\n"
    "A03;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "B01;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "B02;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "B03;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "B04;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "B09;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "I04;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "I06;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "I08;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "I09;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "I10;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "I11;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "I12;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "I13;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "I14;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "I15;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "K03;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "K04;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "K05;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "K09;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "K10;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "K11;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "K12;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "K13;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "K14;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "K15;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "K16;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "LIP01;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "LIP02;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "LIP03;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "LIP07;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "J01;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "J08;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "LMC;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;\n"
    "LMS;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;L;";

    // Tentar abrir para leitura, testando se ele existe mesmo
    Pont_Pdefault = fopen(NomePdefault, "r");

    if (Pont_Pdefault == NULL)
    {
        // O arquivo não existe, então cria ele
        Pont_Pdefault = fopen(NomePdefault, "w");

	if (Pont_Pdefault == NULL)
	{
        	perror("Erro ao abrir o arquivo");
        	return;
	}

        // Coloca o texto da planilha no arquivo
        fprintf(Pont_Pdefault, "%s", PlanilhaDefaultTexto);
	fflush(Pont_Pdefault);

        printf("Arquivo de planilha default foi criado.\n");
    }

    else
    {
        // Se o arquivo existe, fecha
        fclose(Pont_Pdefault);
    }
    fclose(Pont_Pdefault);
}

//Execucao (em praca publica)
int main()
{
	PlanilhaDefaultExistinator(); //Necessário para criar a plannilha default se ela não existir
	printf("\n  Seja bem-vindo ao programa de cadastro de salas!\n\n");
	registrador();
    return 0;
}

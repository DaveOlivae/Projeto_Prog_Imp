#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// Definindo os tamanhos para cada campo
#define Size_data 11
#define Size_sala 5
#define Size_horario 5
#define Size_nome 51
#define Size_cpf 13
#define Size_numero 20
#define Size_prof_responsavel 50
#define Size_monitor_sn 2
#define Size_evento 1000
//fscanf e fprintf
#define tam_data "%11[^;]"
#define tam_sala "%5[^;]"
#define tam_horario "%5[^;]"
#define tam_nome "%50[^;]"
#define tam_cpf "%13[^;]"
#define tam_numero "%20[^;]"
#define tam_prof_responsavel "%50[^;]"
#define tam_monitor_sn "%2[^;]"
#define tam_evento "%1000[^;]"

#define fscanfDSH tam_data ";" tam_sala ";" tam_horario ";" \

#define fscanfTudo tam_data ";" tam_sala ";" tam_horario ";" \
                 tam_nome ";" tam_cpf ";" tam_numero ";" \
                 tam_prof_responsavel ";" tam_monitor_sn ";" \
                 tam_evento "\n"

#define fprintfTudo "%s,%s,%s,%s,%s,%s,%s,%s,%s,%d\n"

// Declarando funções
int compDataHorSala(const char *data1, const char *data2, const char *sala1, const char *sala2, const char *horario1, const char *horario2);
void addAlug(const char *nome_do_csv);
int verifCriaArquivo;
void attAlug(const char *nome_do_csv, const char *data, const char *sala, const char *horario);
void removAlug(const char *nome_do_csv, const char *data, const char *sala, const char *horario);
int registrador();
void TrocarLouO (char Data_T[], char Sala_T[], char Hora_T[]);
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
int verificarCriarArquivo(const char *nome_do_csv) {
    FILE *pont_csv = fopen(nome_do_csv, "r+");

    if (pont_csv == NULL) {
        pont_csv = fopen(nome_do_csv, "a+");

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
void addAlug(const char *nome_do_csv) {
    struct aluguel aluguel, aluguel_existente;

    if (!verificarCriarArquivo(nome_do_csv)) {
        return;
    }

    FILE *pont_csv = fopen(nome_do_csv, "r+"); // Use apenas r+ para leitura e escrita
    if (pont_csv == NULL) {
        printf("Arquivo de registro nao encontrado. :C\n");
        return;
    }

    printf("Insira a data (DD-MM-AAAA): ");
fgets(aluguel.data, Size_data, stdin);
aluguel.data[strcspn(aluguel.data, "\n")] = '\0';
if (strlen(aluguel.data) == Size_data - 1 && aluguel.data[Size_data - 2] != '\n') {
    limparBuffer();
}

printf("Insira a sala (B02, I15, ...): ");
fgets(aluguel.sala, Size_sala, stdin);
aluguel.sala[strcspn(aluguel.sala, "\n")] = '\0';
if (strlen(aluguel.sala) == Size_sala - 1 && aluguel.sala[Size_sala - 2] != '\n') {
    limparBuffer();
}

printf("Insira o horario (formato 710 para 7:10): ");
fgets(aluguel.horario, Size_horario, stdin);
aluguel.horario[strcspn(aluguel.horario, "\n")] = '\0';
if (strlen(aluguel.horario) == Size_horario - 1 && aluguel.horario[Size_horario - 2] != '\n') {
    limparBuffer();
}

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
    
    fprintf(pont_csv, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%d\n", 
            aluguel.data, aluguel.sala, aluguel.horario, aluguel.nome, 
            aluguel.cpf, aluguel.celular, aluguel.prof_responsavel, aluguel.monitor_sn, 
            aluguel.evento, aluguel.modificado);
	
    TrocarLouO (aluguel.data, aluguel.sala, aluguel.horario); // Usa essas informacoes para usar na funcao que modifica o quadro de horario L ou O
	
    printf("Aluguel adicionado! :)\n");

    fclose(pont_csv);
}

//Atualizacao de registro
void attAlug(const char *nome_do_csv, const char *data, const char *sala, const char *horario) {
    FILE *pont_csv = fopen(nome_do_csv, "r");
    FILE *pont_temp = fopen("temp.csv", "w");
    struct aluguel aluguel;
    struct aluguel aluguel_existente;

    if (!pont_csv || !pont_temp) {
        printf("Erro ao abrir arquivos.\n");
        return;
    }

    while (fscanf(pont_csv, fscanfTudo, aluguel.data, aluguel.sala, aluguel.horario,
                 aluguel.nome, aluguel.cpf, aluguel.celular, aluguel.prof_responsavel, 
                 aluguel.monitor_sn, aluguel.evento) == 9) {
        if (strcmp(aluguel.data, data) == 0 && strcmp(aluguel.sala, sala) == 0 && strcmp(aluguel.horario, horario) == 0) {
            aluguel.modificado = 1;
            printf("Registro atualizado. :D\n");
        }

        fprintf(pont_temp, fprintfTudo, aluguel.data, aluguel.sala, aluguel.horario,
                aluguel.nome, aluguel.cpf, aluguel.celular, aluguel.prof_responsavel, 
                aluguel.monitor_sn, aluguel.evento, aluguel.modificado);
    }

    fclose(pont_csv);
    fclose(pont_temp);
    
    remove(nome_do_csv);
    rename("temp.csv", nome_do_csv);
}

//Remover registro
void removAlug(const char *nome_do_csv, const char *data, const char *sala, const char *horario) {
    FILE *pont_csv = fopen(nome_do_csv, "r");
    FILE *pont_temp = fopen("temp.csv", "w");
    struct aluguel aluguel;

    if (!pont_csv || !pont_temp) {
        printf("Erro ao abrir arquivos.\n");
        return;
    }

    while (fscanf(pont_csv, fscanfTudo, aluguel.data, aluguel.sala, aluguel.horario,
                 aluguel.nome, aluguel.cpf, aluguel.celular, aluguel.prof_responsavel, 
                 aluguel.monitor_sn, aluguel.evento) == 9) {
        if (strcmp(aluguel.data, data) == 0 && strcmp(aluguel.sala, sala) == 0 && strcmp(aluguel.horario, horario) == 0) {
            printf("Registro removido.\n");
        } else {
            fprintf(pont_temp, fprintfTudo, aluguel.data, aluguel.sala, aluguel.horario,
                    aluguel.nome, aluguel.cpf, aluguel.celular, aluguel.prof_responsavel, 
                    aluguel.monitor_sn, aluguel.evento, aluguel.modificado);
        }
    }

    fclose(pont_csv);
    fclose(pont_temp);

    remove(nome_do_csv);
    rename("temp.csv", nome_do_csv);
}

int registrador() {
    int opcao;
    char data[Size_data], sala[Size_sala], horario[Size_horario];

    do {
        printf("Selecione uma opcao:\n");
        printf("1 - ADICIONAR um registro\n");
        printf("2 - ATUALIZAR um registro\n");
        printf("3 - REMOVER um registro\n");
		printf("4 - ADICIONAR uma SALA aos horarios\n");
		printf("5 - REMOVER uma SALA dos horarios\n");
        printf("0 - SAIR\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        getchar(); //Limpeza do buffer apos o scanf

        switch (opcao) {
            case 1:
                addAlug("alugueis.csv");
                break;
            case 2:
                // Data
                printf("Data (DD-MM-AAAA): ");
                fgets(data, Size_data, stdin);
                data[strcspn(data, "\n")] = '\0';  // Remove o \n caso presente

                // Limpeza do buffer caso o usuário tenha digitado mais que o máximo permitido
                if (strlen(data) == Size_data - 1 && data[Size_data - 2] != '\n') {
                    while (getchar() != '\n'); // Limpa o resto do buffer
                }

                // Sala
                printf("Sala (B02, I15, ...): ");
                fgets(sala, Size_sala, stdin);
                sala[strcspn(sala, "\n")] = '\0';  // Remove o \n caso presente

                // Limpeza do buffer caso o usuário tenha digitado mais que o máximo permitido
                if (strlen(sala) == Size_sala - 1 && sala[Size_sala - 2] != '\n') {
                    while (getchar() != '\n'); // Limpa o resto do buffer
                }

                // Horário
                printf("Horario (formato 710 para 7:10): ");
                fgets(horario, Size_horario, stdin);
                horario[strcspn(horario, "\n")] = '\0';  // Remove o \n caso presente

                // Limpeza do buffer caso o usuário tenha digitado mais que o máximo permitido
                if (strlen(horario) == Size_horario - 1 && horario[Size_horario - 2] != '\n') {
                    while (getchar() != '\n'); // Limpa o resto do buffer
                }

                Sleep(1000);  // Pausa de 1 segundo

                    attAlug("alugueis.csv", data, sala, horario);
                    break;
            case 3:
                // Data
                printf("Data (DD-MM-AAAA): ");
                fgets(data, Size_data, stdin);
                data[strcspn(data, "\n")] = '\0';  // Remove o \n caso presente

                // Limpeza do buffer caso o usuário tenha digitado mais que o máximo permitido
                if (strlen(data) == Size_data - 1 && data[Size_data - 2] != '\n') {
                    while (getchar() != '\n'); // Limpa o resto do buffer
                }

                // Sala
                printf("Sala (B02, I15, ...): ");
                fgets(sala, Size_sala, stdin);
                sala[strcspn(sala, "\n")] = '\0';  // Remove o \n caso presente

                // Limpeza do buffer caso o usuário tenha digitado mais que o máximo permitido
                if (strlen(sala) == Size_sala - 1 && sala[Size_sala - 2] != '\n') {
                    while (getchar() != '\n'); // Limpa o resto do buffer
                }

                // Horário
                printf("Horario (formato 710 para 7:10): ");
                fgets(horario, Size_horario, stdin);
                horario[strcspn(horario, "\n")] = '\0';  // Remove o \n caso presente

                // Limpeza do buffer caso o usuário tenha digitado mais que o máximo permitido
                if (strlen(horario) == Size_horario - 1 && horario[Size_horario - 2] != '\n') {
                    while (getchar() != '\n'); // Limpa o resto do buffer
                }

                Sleep(1000);  // Pausa de 1 segundo
                    removAlug("alugueis.csv", data, sala, horario);
                    break;
			case 4:
				AdicionarSala ();
				break;
			case 5:
				RemoverSala ();
				break;
            case 0:
                printf("Programa encerrado.\n");
                break;
            default:
                printf("Opção invalida.\n");
        }
    } while (opcao != 0);

}

//######################################################################################################################################
//######################################################################################################################################
//######################################################################################################################################
void TrocarLouO (char Data_T[], char Sala_T[], char Hora_T[])
{
	FILE *Ponteiro_Arquivo; // Aponta para um arquivo
	char ID_do_arquivo[Size_nome]; // Array que recebe o nome do Arquivo, formato exemplo: 112024
    
	strcpy(ID_do_arquivo, Data_T); //input do usuario para ID_do_arquivo

	ID_do_arquivo[Size_nome - 4] = '.';
	ID_do_arquivo[Size_nome - 3] = 'c';
	ID_do_arquivo[Size_nome - 2] = 's';
	ID_do_arquivo[Size_nome - 1] = 'v';
	ID_do_arquivo[Size_nome] = '\0';

	char comeco_nome[] = "planilha";
	int lencomeco = strlen(comeco_nome);
	int lenID = strlen(ID_do_arquivo);

	// Tamanho do resultado suficiente para caber os dois
	char resultado[lencomeco + lenID - 2]; // -2 já que o final das arrays contem \0

    // Copia a primeira array para a array resultado
    for (int i = 0; i < lencomeco; i++) {
        resultado[i] = comeco_nome[i];
    }

    // Copia a segunda array para a array resultado, depois da primeira
    for (int i = 0; i < lenID; i++) {
        resultado[lencomeco + i] = ID_do_arquivo[i];
    }

    // Adiciona \0 no final das arrays somadas
    resultado[lencomeco + lenID] = '\0';
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

		fclose(Ler_default);
		Ponteiro_Arquivo = fopen(resultado, "r+");
		if (Ponteiro_Arquivo == NULL)
		{
			perror("Erro ao abrir o arquivo, verifique o local do arquivo");
	       		return;
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
	int bloco_horario = atoi(Hora_T);
	// Calcular os intervalos de 50 minutos
    bloco_horario = ((bloco_horario - 710) / 50) * 2; // Começa a contar a partir do 0, *2 por conta do ';'
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
	}

    long int Posicao_leitura = ftell(Ponteiro_Arquivo);

	fseek(Ponteiro_Arquivo, Posicao_leitura + bloco_horario, SEEK_SET);

	char char_atual = fgetc(Ponteiro_Arquivo);

    if (char_atual == EOF)
	{
        printf("Erro: Chegou ao EOF (final do arquivo) ou a uma posicao invalida.\n");
        fclose(Ponteiro_Arquivo);
        return;  // Retorna se a posicao for invalida
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
    }
    else //DEBUG ou se algo de muito errado tiver acontecido parte 2
	{
        printf("Erro: Caractere na posicao %d nao eh uma 'L' ou 'O' ou ';'.\n", posicao_horario);
		printf("Char atual: _%c_ \n", char_atual); //DEBUG
    }

	fflush(Ponteiro_Arquivo);
	fclose(Ponteiro_Arquivo); //############## FECHA O ARQUIVO ##############

	return;
}
//######################################################################################################################################
//######################################################################################################################################
//######################################################################################################################################

void AdicionarSala ()
{
	FILE *Ponteiro_Arquivo; // Aponta para um arquivo
	char ID_do_arquivo[19]; // Array que recebe o nome do Arquivo, formato exemplo: 112024
	
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
	printf("_%s_\n", linhadasala); //debug
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

	FILE *Pont_ArquivoTemp = fopen("ArquivoTemporario.csv", "w");
	if (Pont_ArquivoTemp == NULL)
    {
        perror("Error opening file");
        return;
    }
////// Parte de encontrar e remover a sala
	
	char sala_para_remover[Size_nome]; // SALA ##############################
    printf("Digite a sala a ser removida:\n");
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
		}

		fscanf(Ponteiro_Arquivo, "%*c");
		//printf("_%sfim\n", sala_presente);

        if (strcmp(sala_para_remover, sala_presente) == 0)
		{
	        printf("Sala encontrada\n");
	        fclose(Ponteiro_Arquivo);
			ja_registrado = 1;
            break;
        }
        qual_linha_remover++;
	}
	//printf("_%s_\n", sala_para_remover);

	if (ja_registrado == 0)
    {
        rewind(Ponteiro_Arquivo);
		printf("A sala foi registrada\n");
		char buffer[4096];

        int Linha_atual = 1;
        // Lê linha por linha do arquivo original
        while (fgets(buffer, sizeof(buffer), Ponteiro_Arquivo) != NULL)
        {
        // Escreve a linha no arquivo temporário se não for a linha que quer remover
            if (Linha_atual != qual_linha_remover)
            {
                fputs(buffer, Pont_ArquivoTemp);
            }
        Linha_atual++;
        }

        // Fecha os dois arquivos
        fclose(Ponteiro_Arquivo);
        fclose(Pont_ArquivoTemp);
    }
	// Remove the original file
	if (remove("planilhadefault.csv") != 0)
	{
	    perror("Erro ao deletar o Arquivo original");
	    return;
	}
	    // Renomear o arquivo temporário com o nome do arquivo original, é o novo arquivo default
	        if (rename("ArquivoTemporario.csv", "planilhadefault.csv") != 0)
            {
                perror("Erro renaming temporary file");
            } 
            else
            {
                printf("Sala na linha %d foi removida.\n", qual_linha_remover);
            }
    // Fecha a função
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

int main() 
{
	PlanilhaDefaultExistinator(); //Necessário para criar a plannilha default se ela não existir
	printf("Seja bem-vindo ao programa de cadastro de salas!\n");
	registrador();
    return 0;
}

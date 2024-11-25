#include <stdio.h>
#include <stdlib.h>
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

void TrocarLouO (char Data_T[], char Sala_T[], char Hora_T[]); // Declarando função
void AdicionarSala (char Sala_T[]);

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
    return (strcmp(data1, data2) == 0 && strcmp(sala1, sala2) == 0 && strcmp(horario1, horario2) == 0);
}

//Adicao de registro
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

    //teste de conflito
    rewind(pont_csv);
    while (fscanf(pont_csv, "%10[^;];%4[^;];%4[^\n]\n",
                  aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario) == 3) {
        if (compDataHorSala(aluguel.data, aluguel_existente.data, aluguel.sala, aluguel_existente.sala, aluguel.horario, aluguel_existente.horario)) {
            printf("Erro: Esse horário já foi agendado. :C\n");
            fclose(pont_csv);
            return;
        }
    }

    
    fprintf(pont_csv, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%d\n", 
            aluguel.data, aluguel.sala, aluguel.horario, aluguel.nome, 
            aluguel.cpf, aluguel.celular, aluguel.prof_responsavel, aluguel.monitor_sn, 
            aluguel.evento, aluguel.modificado);
	
    int TrocarLouO (aluguel.data, aluguel.sala, aluguel.horario); // Usa essas informações para usar na função que modifica o quadro de horário L ou O
	
    printf("Aluguel adicionado! :)\n");

    fclose(pont_csv);
}

//Atualizacao de registro
void attAlug(const char *nome_do_csv, const char *data, const char *sala, const char *horario) {
    FILE *pont_csv = fopen(nome_do_csv, "r");
    FILE *pont_temp = fopen("temp.csv", "w");
    FILE *pont_verif_new = fopen(nome_do_csv, "r");
    
    struct aluguel aluguel_existente, aluguel_novo;
    int encontrou = 0;

    if (pont_csv == NULL || pont_temp == NULL || pont_verif_new == NULL) {
        printf("Erro ao procurar o arquivo, tente novamente. :C\n");
        if (pont_csv) fclose(pont_csv);
        if (pont_temp) fclose(pont_temp);
        if (pont_verif_new) fclose(pont_verif_new);
        return;
    }

    while (fscanf(pont_csv, "%10[^;];%4[^;];%4[^;];%49[^;];%12[^;];%19[^;];%49[^;];%1[^;];%999[^;];%d\n",
                  aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario,
                  aluguel_existente.nome, aluguel_existente.cpf, aluguel_existente.celular,
                  aluguel_existente.prof_responsavel, aluguel_existente.monitor_sn,
                  aluguel_existente.evento, &aluguel_existente.modificado) == 10) {
                
        long posicao_p_csv = ftell(pont_csv);

        if (compDataHorSala(aluguel_existente.data, data, aluguel_existente.sala, sala, aluguel_existente.horario, horario)) {
            printf(":D Registro encontrado ! Insira as novas informaçoes ou pressione ENTER para manter os valores atuais:\n");
			int TrocarLouO (aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario); // Modifica o quadro de horário L ou O, já que é O, muda pra L antes de modificar
			
            printf("Data [%s]: ", aluguel_existente.data);
            fgets(aluguel_novo.data, Size_data, stdin);
            aluguel_novo.data[strcspn(aluguel_novo.data, "\n")] = '\0';
            if (strlen(aluguel_novo.data) == 0) strcpy(aluguel_novo.data, aluguel_existente.data);

            printf("Sala [%s]: ", aluguel_existente.sala);
            fgets(aluguel_novo.sala, Size_sala, stdin);
            aluguel_novo.sala[strcspn(aluguel_novo.sala, "\n")] = '\0';
            if (strlen(aluguel_novo.sala) == 0) strcpy(aluguel_novo.sala, aluguel_existente.sala);

            printf("Horario [%s]: ", aluguel_existente.horario);
            fgets(aluguel_novo.horario, Size_horario, stdin);
            aluguel_novo.horario[strcspn(aluguel_novo.horario, "\n")] = '\0';
            if (strlen(aluguel_novo.horario) == 0) strcpy(aluguel_novo.horario, aluguel_existente.horario);

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

            printf("Professor responsavel [%s]: ", aluguel_existente.prof_responsavel);
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

            //teste de conflito
            rewind(pont_verif_new);
            while (fscanf(pont_verif_new, "%"  Size_data  "[^;];%" Size_sala  "[^;];%" Size_horario  "[^;]\n",
                  aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario) == 3) {
                
                if (compDataHorSala(aluguel_novo.data, aluguel_existente.data, aluguel_novo.sala, aluguel_existente.sala, aluguel_novo.horario, aluguel_existente.horario)){
                    printf("O horario e data correspondentes para essa sala ja foram reservados. :C");
                    fseek(pont_csv, posicao_p_csv, SEEK_SET);
                    return;

            	fprintf(pont_temp, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%d\n",
                   	aluguel_novo.data, aluguel_novo.sala, aluguel_novo.horario,
                 	aluguel_novo.nome, aluguel_novo.cpf, aluguel_novo.celular,
	                aluguel_novo.prof_responsavel, aluguel_novo.monitor_sn,
                  	aluguel_novo.evento, aluguel_novo.modificado);
		// XXXXXXXXXXXXX daqui serão pegas as NOVAS informações do registro que foi atualizado, basta aplicar esses aluguel_novo.data, aluguel_novo.sala e aluguel_novo.horario do fprintf (eles estarão corretos tanto pra se mudou data/sala/horario quanto pra se não mudou)
		// XXXXXXXXXXXX a função das VELHAS informações terá que ser indentada nesse if pra evitar erro, você vai injetar aluguel_existente.data, aluguel_existente.sala e aluguel_existente.horario
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
    fclose(pont_verif_new);

    if (encontrou) {
        remove(nome_do_csv);
        rename("temp.csv", nome_do_csv);
        printf("Registro atualizado! C:\n");
    } 
    else {
        printf("Registro nao encontrado. :C\n");
        remove("temp.csv");
    }

//Remocao de registro
void removAlug(const char *nome_do_csv, const char *data, const char *sala, const char *horario) {
    FILE *pont_csv = fopen(nome_do_csv, "r");
    FILE *pont_temp = fopen("temp.csv", "w");
    struct Alug aluguel_existente;
    int encontrou = 0;

    if (pont_csv == NULL || pont_temp == NULL) {
        printf("Arquivo de registro nao encontrado. :C\n");
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
        printf("Registro nao encontrado. :C\n");
        remove("temp.csv");
    }
}

//Registrador (teste no terminal)
int registrador() {
    int opcao;
    char data[Size_data], sala[Size_sala], horario[Size_horario];

    do {
        printf("Selecione uma opção:\n");
        printf("1 - ADICIONAR um registro\n");
        printf("2 - ATUALIZAR um registro\n");
        printf("3 - REMOVER um registro\n");
		printf("4 - ADICIONAR uma SALA aos horarios\n");
        printf("0 - SAIR\n");
        printf("Opção: ");
        scanf("%d", &opcao);
        getchar(); //Limpeza do buffer apos o scanf

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
			case 4:
				AdicionarSala (sala);
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
        printf("Error: Reached end of file or invalid position.\n");
        fclose(Ponteiro_Arquivo);
        return;  // Exit if position is invalid
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
	else if (char_atual == ';')
	{
        printf("Error: Character at position %d is a ';' \n", posicao_horario);
    }
    else
	{
        printf("Error: Character at position %d is neither 'L' nor 'O'.\n", posicao_horario);
		printf("Char atual: _%c_ \n", char_atual);
    }

	fflush(Ponteiro_Arquivo);
	fclose(Ponteiro_Arquivo); //############## FECHA O ARQUIVO ##############

	return;
}
//######################################################################################################################################
//######################################################################################################################################
//######################################################################################################################################

void AdicionarSala (char Sala_T[])
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
	strcpy(nova_sala, Sala_T);
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
	printf("Seja bem-vindo ao programa de cadastro de salas!\n");
	registrador();
    return 0;
}

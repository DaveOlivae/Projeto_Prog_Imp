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

int TrocarLouO ();

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
            while (fscanf(pont_verif_new, "%"  #Size_data  "[^;];%" #Size_sala  "[^;];%" #Size_horario  "[^;]\n",
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
int TrocarLouO (char Data_T, char Sala_T, char Hora_T)
{
	FILE *Ponteiro_Arquivo; // Aponta para um arquivo
	char ID_do_arquivo[Size_nome]; // Array que recebe o nome do Arquivo, formato exemplo: 112024
    
	ID_do_arquivo = Data; //input do usuario para ID_do_arquivo

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

////// Parte de modifica horários ###########################################################

    char nova_sala[Size_nome]; // SALA ##############################
    nova_sala = Sala_T
	for (int i = 0; i < strlen(nova_sala); i++)
	{
        if (nova_sala[i] == '\n')
		{
            nova_sala[i] = '\0';
		}
	}
    
	int bloco_horario; // HORARIO ##############################
    // Converte a string para um int
    bloco_horario = atoi(Hora_T);
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

	//printf("_linhas pular:_%i_\n", posicao_horario); // DEBUG

    long int Posicao_leitura = ftell(Ponteiro_Arquivo);

    size_t buffer2_size = 1024 * 1024;  // buffer de 1MB
    char *buffer2 = (char *)malloc(buffer2_size);
    if (!buffer2)
    {
        perror("Failed to allocate memory");
        fclose(Ponteiro_Arquivo);
        return 1;
    }

	fseek(Ponteiro_Arquivo, Posicao_leitura + bloco_horario, SEEK_SET);

	char char_atual = fgetc(Ponteiro_Arquivo);

    if (char_atual == EOF)
	{
        printf("Error: Reached end of file or invalid position.\n");
        fclose(Ponteiro_Arquivo);
        return 1;  // Exit if position is invalid
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

	return 0;
}
//######################################################################################################################################
//######################################################################################################################################
//######################################################################################################################################

//Execucao (em praca publica)
int main() {
    registrador();
    return 0;
}

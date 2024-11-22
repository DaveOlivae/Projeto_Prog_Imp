#include <stdio.h>
#include <stdlib.h>

#define Size_nome 12 //tamanho do nome do arquivo

// status;data;sala;hora_inicio;hora_fim;monitor_s/n;CPF;nome;celular;prof_responsavel;evento

int main ()
{
	FILE *Ponteiro_Arquivo; // Aponta para um arquivo
	char ID_do_arquivo[Size_nome]; // Array que recebe o nome do Arquivo, formato exemplo: 112024

	printf("Codigo de terminal, insira o ID_do_arquivo teste \n"); //Utilizando antes de implementar o GTK

	fgets(ID_do_arquivo, sizeof(ID_do_arquivo), stdin); //Lê input do usuario para ID_do_arquivo

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
    printf("Digite a sala:\n");
	fgets(nova_sala, Size_nome, stdin);
	for (int i = 0; i < strlen(nova_sala); i++)
	{
        if (nova_sala[i] == '\n')
		{
            nova_sala[i] = '\0';
		}
	}

	int bloco_horario; // HORARIO ##############################
    char buffer[Size_nome];
	printf("Digite a posicao do horario:\n");
	if (fgets(buffer, sizeof(buffer), stdin) != NULL)
	{
        // Converte a string para um int
        bloco_horario = atoi(buffer);
	}
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

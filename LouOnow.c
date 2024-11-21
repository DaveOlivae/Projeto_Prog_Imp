#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

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
	

////// Parte de modifica horários

	//char linha_sala[Size_nome];
    //printf("Digite a sala:\n");
	//fgets(linha_sala, Size_nome, stdin);
	//for (int i = 0; i < strlen(linha_sala); i++) 
	//{
    //    if (linha_sala[i] == '\n') 
	//	{
    //        linha_sala[i] = '\0';
	//	}		
	//} 
	//printf("_%s_\n", linha_sala); //DEBUG
	
	char buffer[Size_nome];
	int posicao_horario;
	printf("Digite a posicao do horario:\n");
	if (fgets(buffer, sizeof(buffer), stdin) != NULL) 
	{
        // Convert the string to an integer
        posicao_horario = atoi(buffer);
	}
	
	printf("%i\n", posicao_horario);
	
    //Verificar se a sala já existe
    rewind(Ponteiro_Arquivo); //Ponteiro de leitura retorna ao comeco do arquivo
	
	fseek(Ponteiro_Arquivo, posicao_horario + 223, SEEK_SET);

	char char_atual = fgetc(Ponteiro_Arquivo);
    
    if (char_atual == EOF) 
	{
        printf("Error: Reached end of file or invalid position.\n");
        fclose(Ponteiro_Arquivo);
        return 1;  // Exit if position is invalid
    }
	
    // Check if the character is 'L' or 'O' and change it
    if (char_atual == 'L') 
	{
        fseek(Ponteiro_Arquivo, posicao_horario + 223, SEEK_SET);  // Seek again to the same position
        fputc('O', Ponteiro_Arquivo);  // Change 'L' to 'O'
        printf("Character at position %d changed from 'L' to 'O'.\n", posicao_horario);
    } 
    else if (char_atual == 'O') 
	{
        fseek(Ponteiro_Arquivo, posicao_horario + 223, SEEK_SET);  // Seek again to the same position
        fputc('L', Ponteiro_Arquivo);  // Change 'O' to 'L'
        printf("Character at position %d changed from 'O' to 'L'.\n", posicao_horario);
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




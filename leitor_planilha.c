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
	// printf(resultado); //DEBUG

	Ponteiro_Arquivo = fopen(resultado, "r"); //############## ABRE O ARQUIVO ##############

	char Linha [80];
	while((fgets(Linha, sizeof(Linha), Ponteiro_Arquivo)) != NULL) // Print no terminal o que contém o arquivo linha por linha
		{
			printf("%s",Linha);
		}
		
	fclose(Ponteiro_Arquivo); //############## FECHA O ARQUIVO ##############
	
	return 0;
}


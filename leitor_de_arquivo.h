#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#define Size_nome 12 //tamanho do nome do arquivo, DDMMAAAA.csv

//Ainda escrevendo e testando, no momento consegue ler um arquivo e mostrar seu conteúdo

// status;data;sala;hora_inicio;hora_fim;monitor_s/n;CPF;nome;celular;prof_responsavel;evento

struct registro
{
     char status[];
     char data[];
     char sala[];
     char hora_inicio[];
     char hora_fim[];
	 char monitor_s/n[];
	 char CPF[];
	 char nome[];
	 char celular[];
	 char prof_responsavel[];
	 char evento [];

     struct
     {
        int dia;
        int mes;
        int ano;
     }data;
	 
} reserva_main, reserva_comparar;

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
	
	Ponteiro_Arquivo = fopen(ID_do_arquivo, "r"); //############## ABRE O ARQUIVO ##############
	
	//if(Ponteiro_Arquivo == NULL) 
	//	{
	//		printf("Nao foi possivel abrir o arquivo %s", ID_do_arquivo);
	//		exit(1);
	//	}

	//printf("DEBUG");
	char Linha [80];
	while((fgets(Linha, sizeof(Linha), Ponteiro_Arquivo)) != NULL) // Print no terminal o que contém o arquivo linha por linha
		{
			printf("%s",Linha);
		}
		
	fclose(Ponteiro_Arquivo); //############## FECHA O ARQUIVO ##############
	
	return 0;
}

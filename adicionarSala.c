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

	Ponteiro_Arquivo = fopen(resultado, "a+"); //############## ABRE O ARQUIVO ##############
	

////// Parte de adicionar salas

	char nova_sala[Size_nome];
    printf("Digite a nova sala:\n");
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

	return 0;
}

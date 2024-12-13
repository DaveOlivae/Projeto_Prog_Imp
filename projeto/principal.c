#include <gtk/gtk.h>

#define Size_data 13
#define Size_sala 21
#define Size_horario 5
#define Size_nome 52
#define Size_cpf 15
#define Size_numero 21
#define Size_prof_responsavel 52
#define Size_monitor_sn 2
#define Size_evento 1001

//NAO PONHA A STRUCT DE ALUGUEL DEPOIS, ELA P R E C I S A FICAR AQUI.
struct aluguel {
    char data[Size_data];
    char sala[Size_sala];
    char horario[Size_horario];
    char horarioFim[Size_horario];
    char nome[Size_nome];
    char cpf[Size_cpf];
    char celular[Size_numero];
    char prof_responsavel[Size_prof_responsavel];
    char monitor_sn[Size_monitor_sn];
    char evento[Size_evento];
};

//Decaração de funcoes
static int verifica_horarios(const char hora_inicial[], const char hora_final[], int hora_existente_ini, int hora_existente_fim);
static int verifica_disponibilidade(const char *data, const char *sala, const char hora_inicial[],const char hora_final[]);
static int compDataHorSala(const char *data1, const char *data2, const char *sala1, const char *sala2, const char *horario1, const char *horario2);
static void addAlug(const char *data, const char *sala, const char *horario, const char *horarioFim,
            const char *nome, const char *cpf, const char *celular, const char *prof_responsavel,
            const char *monitor_sn, const char *evento);
static int verificarCriarArquivo(const char *data_do_csv);
static void limparAluguel(struct aluguel *aluguelaserlimpo);
static void limparBuffer();
static void attDSH(const char *data_do_csv, const char *data, const char *sala, const char *horario);
static void attInfos(const char *data_do_csv, const char *data, const char *sala, const char *horario);
static void removAlug(const char data[], const char sala[], const char horario[]);
static int registrador();
static int IeFnoLouO (char Data_T[], char Sala_T[], char hora_i[], char hora_f[]);
static int TrocarLouO (char Data_T[], char Sala_T[], int Hora_minutos);
static void AdicionarSala ();
static void RemoverSala ();
static void PlanilhaDefaultExistinator ();

static void remover_ponto(const char *input, char *output) {
    int j = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] != ':') {
            output[j++] = input[i];
        }
    }
    output[j] = '\0'; // Finaliza a string
}

static const char *const salas[] = {
    "A03", "B01", "B02", "B03", "B04", "B09", "I04",
    "I06", "I08", "I09", "I10", "I11", "I12", "I13",
    "I14", "I15", "K03", "K04", "K05", "K09", "K10",
    "K11", "K12", "K13", "K14", "K15", "K16", "LIP01",
    "LIP02", "LIP03", "LIP07", "J01", "J08", "LMC", "LMS", NULL
};

static char *horarios_inicio[] = {
    "07:10", "08:00", "08:50", "09:40", "10:30",
    "11:20", "12:10", "13:00", "13:50", "14:40",
    "15:30", "16:20", "17:10", "18:00", "18:50",
    "19:40", "20:30", "21:20", "22:10"
};

static char *horarios_inicio_sem_ponto[] = {
    "0710", "0800", "0850", "0940", "1030",
    "1120", "1210", "1300", "1350", "1440",
    "1530", "1620", "1710", "1800", "1850",
    "1940", "2030", "2120", "2210"
};

// data selecionada no registro
static char selected_date[13] = ""; // Formato: "YYYY-MM-DD"

static char *OqueOcupado(const char *data_do_csv, const char *data, const char *sala, const char *horario)
{
    // codigo escrito pelo excelentissimo sr. joao rafael
    FILE *pont_csv = fopen(data_do_csv, "r");

    struct aluguel aluguel_existente;

    while (fscanf(pont_csv, "%8[^;];%20[^;];%4[^;];%4[^;];%51[^;];%15[^;];%21[^;];%52[^;];%1[^;];%1000[^;];\n",
                  aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario, aluguel_existente.horarioFim,
                  aluguel_existente.nome, aluguel_existente.cpf, aluguel_existente.celular,
                  aluguel_existente.prof_responsavel, aluguel_existente.monitor_sn,
                  aluguel_existente.evento) != EOF)
    {
        int hora_int = atoi(aluguel_existente.horario);
        int hora_fim_int = atoi(aluguel_existente.horarioFim);
        int horario_int = atoi(horario);
        g_print("%s", aluguel_existente.data);
        if (strcmp(aluguel_existente.data, data) == 0 && strcmp(aluguel_existente.sala, sala) == 0 && hora_int <= horario_int && horario_int < hora_fim_int)
        {
            g_print("nice");
            static char evento[1001]; // Certifique-se de que o tamanho é suficiente para armazenar a string
            strcpy(evento, aluguel_existente.evento); // Copia a string para o array
            return evento;
        }
    }
    static char evento[] = "OCUPADO";

    return evento;
}

static int compDataHorSala(const char data1[], const char data2[], const char sala1[], const char sala2[], const char horario1[], const char horario2[]) {
    if (strcmp(data1, data2) == 0 && strcmp(sala1, sala2) == 0 && strcmp(horario1, horario2) == 0) {
        return 1;  // Retorna 1 se todas as comparações forem verdadeiras
    }
    return 0;  // Caso contrário, retorna 0
}

static void removAlug(const char data[], const char sala[], const char horario[]) {
    char data_do_csv[Size_data + 5];
    sprintf(data_do_csv, "%s.csv", data);

    FILE *pont_csv = fopen(data_do_csv, "r");
    FILE *pont_temp = fopen("temp.csv", "w");

    struct aluguel aluguel_existente;
    int start_temp = 0;
    int tudocerto = 0;

    g_print("Tentando abrir o arquivo: _%s_\n", data_do_csv);

    if (pont_csv == NULL || pont_temp == NULL) {
        g_print("Erro ao procurar o arquivo, insira uma data valida ou tente novamente. :C\n");
        if (pont_csv) fclose(pont_csv);
        if (pont_temp) fclose(pont_temp);
        return;
    }

	g_print("\ndata:_%s_", data);

	g_print("\nsala:_%s_", sala);

	g_print("\nhorario:_%s_", horario);
    //Lendo arquivo existente
    while (fscanf(pont_csv, "%8[^;];%20[^;];%4[^;];%4[^;];%51[^;];%19[^;];%21[^;];%52[^;];%1[^;];%1000[^;];\n",
                  aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario, aluguel_existente.horarioFim,
                  aluguel_existente.nome, aluguel_existente.cpf, aluguel_existente.celular,
                  aluguel_existente.prof_responsavel, aluguel_existente.monitor_sn,
                  aluguel_existente.evento) == 10) {

		g_print("\naluguel_existente.data:_%s_", aluguel_existente.data);
		g_print("\ndata:_%s_", data);
		g_print("\naluguel_existente.sala:_%s_", aluguel_existente.sala);
		g_print("\nsala:_%s_", sala);
		g_print("\naluguel_existente.horario:_%s_", aluguel_existente.horario);
		g_print("\nhorario:_%s_\n", horario);

        //Verifica se o registro atual corresponde ao que deve ser removido
        if (compDataHorSala(aluguel_existente.data, data, aluguel_existente.sala, sala, aluguel_existente.horario, horario) == 1) {
            printf("Registro encontrado! :D\nData: %s, Sala: %s, Horario: %s\n",
                   aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario);
            //verificacao de validez pra planilha

            char dataCheck[Size_data], salaCheck[Size_sala], horarioCheck[Size_horario];
            strcpy(dataCheck, data);
            strcpy(salaCheck, sala);
            strcpy(horarioCheck, horario);
            //copiar a const pra mutavel pra conseguir usar no check

            //int check_de_erro = TrocarLouO (dataCheck, salaCheck, horarioCheck); //aluguel_existente.horarioFim // obsoleto
			int check_de_erro = IeFnoLouO (dataCheck, salaCheck, horarioCheck, aluguel_existente.horarioFim);
			//printf("\ncheck de erro IeF:_%i_", check_de_erro);

            if (check_de_erro == 0){ //faz o check de erro, se der tudo certo, retornando 0, printa no final e usa o L ou O dentro do proprio check de erro
                printf("Aluguel removido! :)\n");
                tudocerto = 1;
                continue; //nao escreve no temporario
            }
            else{ // so sera acionado se existir no registro mas nao na planilha, descartavel, mas deixo caso precisemos no futuro de algo semelhante
                printf("Tente novamente. Verifique se as informacoes estao validas.");
                limparAluguel(&aluguel_existente);
                fclose(pont_csv);
                remove("temp.csv");
                return;
            }

        }
            if (start_temp == 0){
            fprintf(pont_temp, "1;2;3;4;5;6;7;8;9;10;");
            start_temp = 1;
            }
        //Copia os outros registros existentes para o arquivo temporário
        fprintf(pont_temp, "\n%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;",
                aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario, aluguel_existente.horarioFim,
                aluguel_existente.nome, aluguel_existente.cpf, aluguel_existente.celular,
                aluguel_existente.prof_responsavel, aluguel_existente.monitor_sn,
                aluguel_existente.evento);
    }

    //Fecha os arquivos se estiverem abertos
    if (pont_csv != NULL) {
        fclose(pont_csv);
    }
    if (pont_temp != NULL) {
        fclose(pont_temp);
    }

    if (tudocerto == 1) {
        remove(data_do_csv);
        rename("temp.csv", data_do_csv);
        limparAluguel(&aluguel_existente);
        printf("Registro removido com sucesso! :)\n");
    } else {
        remove("temp.csv");
        limparAluguel(&aluguel_existente);
        printf("Registro nao encontrado. :C\n");
    }
}

static int verifica_horarios(const char hora_inicial[], const char hora_final[], int hora_existente_ini, int hora_existente_fim) {
    // Verifica se os horários não conflitam
    int numH_inicial = atoi(hora_inicial);
    int numH_final = atoi(hora_final);
    if ((numH_inicial < hora_existente_ini && numH_inicial <= hora_existente_ini) ||
        (numH_inicial >= hora_existente_fim)) {
        return 1; // Horários são compatíveis
    }
    return 0; // Conflito de horários
}

static int verificarCriarArquivo(const char *data_do_csv) {
    FILE *pont_csv = fopen(data_do_csv, "r+");

    if (pont_csv == NULL) {
        pont_csv = fopen(data_do_csv, "a+");
        fprintf(pont_csv, "1;2;3;4;5;6;7;8;9;10;");
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

static int IeFnoLouO (char Data_T[], char Sala_T[], char hora_i[], char hora_f[])
{
	int houve_um_erro = 0;

	int num_hora_i = (hora_i[0] - '0') * 10 + (hora_i[1] - '0'); //converte char pra int - horas
	int num_minuto_i = (hora_i[2] - '0') * 10 + (hora_i[3] - '0'); //converte char pra int - minutos
	int bloco_horario_i = (num_hora_i * 60) + num_minuto_i;

	int num_hora_f = (hora_f[0] - '0') * 10 + (hora_f[1] - '0'); //converte char pra int - horas
	int num_minuto_f = (hora_f[2] - '0') * 10 + (hora_f[3] - '0'); //converte char pra int - minutos
	int bloco_horario_f = (num_hora_f * 60) + num_minuto_f;

    //printf("\n_bloco_horario_f:_%i_", bloco_horario_f); //DEBBUG
    //printf("\n_num_hora_f:_%i_", num_hora_f);
    //printf("\n_num_minuto_f", num_minuto_f);
	// Calcular os intervalos de 50 minutos
	bloco_horario_i = (bloco_horario_i - 430) / 50; // 430 = 710 em minutos
	bloco_horario_f = (bloco_horario_f - 430) / 50;


	int bloco_while = bloco_horario_i;
	int hora_atualnoloop = 0;
	while (bloco_while < bloco_horario_f)
	{
		hora_atualnoloop = bloco_while * 2;
		houve_um_erro = TrocarLouO (Data_T, Sala_T, hora_atualnoloop);
		if (houve_um_erro == 1)
		{
			while (bloco_while >= bloco_horario_i)
			{
				TrocarLouO (Data_T, Sala_T, hora_atualnoloop);
				bloco_while = bloco_while - 1;
				hora_atualnoloop = bloco_while * 2;
			}
			return houve_um_erro;
		}
		bloco_while = bloco_while + 1;
	}

	return houve_um_erro;
}
//######################################################################################################################################
//######################################################################################################################################
//######################################################################################################################################
static int TrocarLouO (char Data_T[], char Sala_T[], int Hora_minutos)
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

	// HORARIO ########################################################
	int bloco_horario = Hora_minutos;
	// Calcular os intervalos de 50 minutos
    //bloco_horario = ((bloco_horario - 430) / 50) * 2; // Começa a contar a partir do 0, *2 por conta do ';'
	//printf("_BLOH_%i_\n", bloco_horario);

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
		//printf("Char atual: _%c_ \n", char_atual);
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

static void AdicionarSala ()
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
static void RemoverSala ()
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
static void printPlanilha()
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
static void PlanilhaDefaultExistinator() //Necessário para criar a plannilha default se ela não existir
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

static void limparAluguel(struct aluguel *aluguelaserlimpo) {
    memset(aluguelaserlimpo, 0, sizeof(struct aluguel));  // Limpa toda a struct com zero
}

static int verifica_disponibilidade(const char *data, const char *sala, const char hora_inicial[], const char hora_final[]) {
    char nome_arquivo[Size_data + 5];
    FILE *arquivo;
    struct aluguel aluguel_existente; // corrigido por john, não tenho ctz se era essa a intenção, verficar dps com matheus

    // Montar o nome do arquivo
    snprintf(nome_arquivo, sizeof(nome_arquivo), "%s.csv", data);

    // Abrir o arquivo
    arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo %s.\n", nome_arquivo);
        return 0;
    }

    // Ler o arquivo linha a linha usando fscanf
    while (fscanf(arquivo,
                  "%8[^;];%20[^;];%4[^;];%4[^;];%51[^;];%19[^;];%21[^;];%52[^;];%1[^;];%1000[^;];\n",
                  aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario,
                  aluguel_existente.horarioFim, aluguel_existente.nome, aluguel_existente.cpf,
                  aluguel_existente.celular, aluguel_existente.prof_responsavel,
                  aluguel_existente.monitor_sn, aluguel_existente.evento) != EOF) {

        // Converter os horários existentes para inteiros
        int hora_existente_ini = atoi(aluguel_existente.horario);
        int hora_existente_fim = atoi(aluguel_existente.horarioFim);

        // Comparar a sala e os horários
        if (strcmp(sala, aluguel_existente.sala) == 0) {
            if (!verifica_horarios(hora_inicial, hora_final, hora_existente_ini, hora_existente_fim)) {
                fclose(arquivo);
                limparAluguel(&aluguel_existente);
                printf("Conflito encontrado, insira hora inicial e final validas.");
                return 0; // Conflito encontrado
            }
        }
    }
    limparAluguel(&aluguel_existente);
    printf("Sem conflitos.");
    fclose(arquivo);
    return 1; // Sem conflito
}

static void addAlug(const char *data, const char *sala, const char *horario, const char *horarioFim,
            const char *nome, const char *cpf, const char *celular, const char *prof_responsavel,
            const char *monitor_sn, const char *evento) {

    struct aluguel aluguel;
    char data_do_csv[Size_data + 5];
    sprintf(data_do_csv, "%s.csv", data);
    g_print("%s", data_do_csv);

    if (!verificarCriarArquivo(data_do_csv)) {
        return;
    }

    FILE *pont_csv = fopen(data_do_csv, "r+"); //ponteiro spawna no começo do arquivo

    //teste de conflito
    if(verifica_disponibilidade(data, sala, horario, horarioFim) == 0){
        g_print("nice");
        fclose(pont_csv);
        limparAluguel(&aluguel);
        return;
    }

	strcpy(aluguel.data, data);
    strcpy(aluguel.sala, sala);
    strcpy(aluguel.horario, horario);
    strcpy(aluguel.horarioFim, horarioFim);
    strcpy(aluguel.nome, nome);
    strcpy(aluguel.cpf, cpf);
    strcpy(aluguel.celular, celular);
    strcpy(aluguel.prof_responsavel, prof_responsavel);
    strcpy(aluguel.monitor_sn, monitor_sn);
    strcpy(aluguel.evento, evento);

    if (pont_csv == NULL) {
        g_print("Erro: o arquivo não foi aberto corretamente.\n");
        return;
    }

    fclose(pont_csv);
    g_print("fechou");
    pont_csv = NULL;

    pont_csv = fopen(data_do_csv, "a+");
    if (pont_csv == NULL) {
        g_print("Erro ao reabrir o arquivo %s\n", data_do_csv);
        return;  // Retorna caso o arquivo não tenha sido aberto corretamente
    }
    //CHECK DE INFORMAÇOES

    //copiar a const pra mutavel pra conseguir usar no check

    //int check_de_erro = TrocarLouO (dataCheck, aluguel.sala, aluguel.horario); //aluguel.horarioFim // obsoleto

	int check_de_erro = IeFnoLouO (aluguel.data, aluguel.sala, aluguel.horario, aluguel.horarioFim);


    if (check_de_erro==0){ //faz o check de erro, se der tudo certo, retornando 0, printa no final e usa o L ou O dentro do proprio check de erro
        fprintf(pont_csv, "\n%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;",
            aluguel.data, aluguel.sala, aluguel.horario, aluguel.horarioFim, aluguel.nome,
            aluguel.cpf, aluguel.celular, aluguel.prof_responsavel, aluguel.monitor_sn,
            aluguel.evento);
        limparAluguel(&aluguel);
        printf("Aluguel adicionado! :)\n");
    }
    else{
        limparAluguel(&aluguel);
        printf("Tente novamente. Verifique se as informacoes estao validas.");
        return;
    }

    fclose(pont_csv);
 }

// Callback para capturar a data selecionada
static void date_selected(GtkCalendar *calendar, gpointer user_data) {

    // Obtém a data selecionada como GDateTime
    GDateTime *date = gtk_calendar_get_date(GTK_CALENDAR(calendar));

    gint year = g_date_time_get_year(date);
    gint month = g_date_time_get_month(date);
    gint day = g_date_time_get_day_of_month(date);

    snprintf(selected_date, sizeof(selected_date), "%02d%02d%04d", day, month, year);
    g_print("Data selecionada: %s\n", selected_date);

    // Formata a data
    gchar *date_text = g_date_time_format(date, "%d/%m/%Y");

    // Atualiza o texto do botão com a data selecionada
    GtkWidget *button = GTK_WIDGET(user_data);
    gtk_button_set_label(GTK_BUTTON(button), date_text);

    // Libera a memória
    g_free(date_text);
    g_date_time_unref(date);

    // Fecha o popover
    GtkWidget *popover = gtk_widget_get_parent(GTK_WIDGET(calendar));
    gtk_popover_popdown(GTK_POPOVER(popover));
}

static void popover_botao(GtkButton *button, gpointer user_data) {
    GtkPopover *popover = GTK_POPOVER(user_data);
    gtk_popover_popup(popover);
}

// funcao quando o botao registrar eh clicado com a opção sim ativada

static void botao_clicado_sim(GtkButton *button, gpointer user_data) {
    // grid
    GtkGrid *grid = GTK_GRID(user_data);

    // eh monitoria? sim
    const char monitoria_quaestio[] = "sim";

    // entry da materia e texto
    GtkWidget *entry_materia = gtk_grid_get_child_at(GTK_GRID(grid), 1, 6);

    const char *materia = gtk_editable_get_text(GTK_EDITABLE(entry_materia));
    char materia_array[Size_evento];

    strncpy(materia_array, materia, sizeof(materia_array) - 1);  // Usar strncpy para evitar buffer overflow
    materia_array[sizeof(materia_array) - 1] = '\0';

    // entry do nome e texto
    GtkWidget *nome_entry = gtk_grid_get_child_at(GTK_GRID(grid), 1, 7);

    const char *nome = gtk_editable_get_text(GTK_EDITABLE(nome_entry));
    char nome_array[Size_nome];

    strncpy(nome_array, nome, sizeof(nome_array) - 1);  // Usar strncpy para evitar buffer overflow
    nome_array[sizeof(nome_array) - 1] = '\0';

    // entry do prof e texto
    GtkWidget *prof_entry = gtk_grid_get_child_at(GTK_GRID(grid), 1, 8);

    const char *prof = gtk_editable_get_text(GTK_EDITABLE(prof_entry));
    char prof_array[Size_prof_responsavel];

    strncpy(prof_array, prof, sizeof(prof_array) - 1);  // Usar strncpy para evitar buffer overflow
    prof_array[sizeof(prof_array) - 1] = '\0';

    // entry do telefone e texto
    GtkWidget *telefone_entry = gtk_grid_get_child_at(GTK_GRID(grid), 1, 9);

    const char *telefone = gtk_editable_get_text(GTK_EDITABLE(telefone_entry));
    char telefone_array[Size_numero];

    strncpy(telefone_array, telefone, sizeof(telefone_array) - 1);  // Usar strncpy para evitar buffer overflow
    telefone_array[sizeof(telefone_array) - 1] = '\0';

    // entry do cpf e texto
    GtkWidget *cpf_entry = gtk_grid_get_child_at(GTK_GRID(grid), 1, 10);

    const char *cpf = gtk_editable_get_text(GTK_EDITABLE(cpf_entry));
    char cpf_array[Size_cpf];

    strncpy(cpf_array, cpf, sizeof(cpf_array) - 1);  // Usar strncpy para evitar buffer overflow
    cpf_array[sizeof(cpf_array) - 1] = '\0';

    // drop down horario de inicio e texto
    GtkWidget *horario_ini_drop = gtk_grid_get_child_at(GTK_GRID(grid), 1, 2);

    guint endereco_hor_ini = gtk_drop_down_get_selected(GTK_DROP_DOWN(horario_ini_drop));

    GtkStringList *lista = GTK_STRING_LIST(gtk_drop_down_get_model(GTK_DROP_DOWN(horario_ini_drop)));

    const char *horario_inicio_c_ponto = gtk_string_list_get_string(lista, endereco_hor_ini);

    char horario_inicio[10];

    remover_ponto(horario_inicio_c_ponto, horario_inicio);

    const char *horario_inicio_const = horario_inicio;
    char horario_inicio_array[Size_horario];

    strncpy(horario_inicio_array, horario_inicio_const, sizeof(horario_inicio_array) - 1);  // Usar strncpy para evitar buffer overflow
    horario_inicio_array[sizeof(horario_inicio_array) - 1] = '\0';

    // drop down horario de fim e texto
    GtkWidget *horario_fim_drop = gtk_grid_get_child_at(GTK_GRID(grid), 1, 3);

    guint endereco_hor_fim = gtk_drop_down_get_selected(GTK_DROP_DOWN(horario_fim_drop));

    GtkStringList *lista2 = GTK_STRING_LIST(gtk_drop_down_get_model(GTK_DROP_DOWN(horario_fim_drop)));

    const char *horario_fim_c_ponto = gtk_string_list_get_string(lista2, endereco_hor_fim);

    char horario_fim[10];

    remover_ponto(horario_fim_c_ponto, horario_fim);

    const char *horario_fim_const = horario_fim;
    char horario_fim_array[Size_horario];

    strncpy(horario_fim_array, horario_fim_const, sizeof(horario_fim_array) - 1);  // Usar strncpy para evitar buffer overflow
    horario_fim_array[sizeof(horario_fim_array) - 1] = '\0';

    // drop down sala e texto
    GtkWidget *sala_drop = gtk_grid_get_child_at(GTK_GRID(grid), 1, 11);

    guint endereco_sala_drop = gtk_drop_down_get_selected(GTK_DROP_DOWN(sala_drop));

    GtkStringList *lista3 = GTK_STRING_LIST(gtk_drop_down_get_model(GTK_DROP_DOWN(sala_drop)));

    const char *sala = gtk_string_list_get_string(lista3, endereco_sala_drop);
    char sala_array[Size_sala];

    strncpy(sala_array, sala, sizeof(sala_array) - 1);  // Usar strncpy para evitar buffer overflow
    sala_array[sizeof(sala_array) - 1] = '\0';

    addAlug(selected_date, sala_array, horario_inicio_const, horario_fim_const, nome_array, cpf_array, telefone_array, prof_array, monitoria_quaestio, materia_array);
}

// ################# JANELA DE REGISTRAR CASO MONITORIA SIM ##############################################################

static void check_clicado_sim(GtkCheckButton *check_button, gpointer user_data) {
    GtkGrid *grid = GTK_GRID(user_data);

    // Definindo o espaçamento entre as células do grid
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);  // Espaçamento entre as linhas
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);  // Espaçamento entre as colunas

    GtkWidget *iter = gtk_grid_get_child_at(GTK_GRID(grid), 0, 6);

    while (iter != NULL) {
        GtkWidget *next = gtk_widget_get_next_sibling (iter);
        gtk_grid_remove (GTK_GRID(grid), iter);
        iter = next;
    }

    GtkWidget *materia = gtk_label_new("Materia");
    gtk_grid_attach(GTK_GRID(grid), materia, 0, 6, 1, 1);

    GtkWidget *entry_materia = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_materia, 1, 6, 1, 1);

    GtkWidget *nome_monitor = gtk_label_new("Nome do Monitor");
    gtk_grid_attach(GTK_GRID(grid), nome_monitor, 0, 7, 1, 1);
    gtk_widget_set_margin_top(nome_monitor, 1);
    gtk_widget_set_margin_bottom(nome_monitor, 1);
    gtk_widget_set_margin_start(nome_monitor, 10);
    gtk_widget_set_margin_end(nome_monitor, 1);

    GtkWidget *entry_monitor = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_monitor, 1, 7, 1, 1);

    GtkWidget *nome_prof = gtk_label_new("Nome do Professor");
    gtk_grid_attach(GTK_GRID(grid), nome_prof, 0, 8, 1, 1);
    gtk_widget_set_margin_top(nome_prof, 1);
    gtk_widget_set_margin_bottom(nome_prof, 1);
    gtk_widget_set_margin_start(nome_prof, 10);
    gtk_widget_set_margin_end(nome_prof, 1);

    GtkWidget *entry_prof = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_prof, 1, 8, 1, 1);

    GtkWidget *telefone = gtk_label_new("Telefone");
    gtk_grid_attach(GTK_GRID(grid), telefone, 0, 9, 1, 1);

    GtkWidget *entry_telefone = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_telefone, 1, 9, 1, 1);

    GtkWidget *cpf = gtk_label_new("Cpf");
    gtk_grid_attach(GTK_GRID(grid), cpf, 0, 10, 1, 1);

    GtkWidget *entry_cpf = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_cpf, 1, 10, 1, 1);

    GtkWidget *sala = gtk_label_new("Sala");
    gtk_grid_attach(GTK_GRID(grid), sala, 0, 11, 1, 1);

    GtkWidget *sala_drop = gtk_drop_down_new_from_strings(salas);
    gtk_grid_attach(GTK_GRID(grid), sala_drop, 1, 11, 1, 1);

    GtkWidget *botao_reg = gtk_button_new_with_label("Registrar");
    gtk_grid_attach(GTK_GRID(grid), botao_reg, 0, 12, 2, 1);
    gtk_widget_set_margin_top(botao_reg, 1);
    gtk_widget_set_margin_bottom(botao_reg, 10);
    gtk_widget_set_margin_start(botao_reg, 10);
    gtk_widget_set_margin_end(botao_reg, 1);

    g_signal_connect(botao_reg, "clicked", G_CALLBACK(botao_clicado_sim), grid);

}

static void check_clicado_nao(GtkCheckButton *check_button, gpointer user_data) {
    GtkGrid *grid = GTK_GRID(user_data);

    // Definindo o espaçamento entre as células do grid
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);  // Espaçamento entre as linhas
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);  // Espaçamento entre as colunas

    GtkWidget *iter = gtk_grid_get_child_at(GTK_GRID(grid), 0, 6);

    while (iter != NULL) {
        GtkWidget *next = gtk_widget_get_next_sibling (iter);
        gtk_grid_remove (GTK_GRID(grid), iter);
        iter = next;
    }

    GtkWidget *nome_evento = gtk_label_new("Nome do evento");
    gtk_grid_attach(GTK_GRID(grid), nome_evento, 0, 6, 1, 1);
    gtk_widget_set_margin_top(nome_evento, 1);
    gtk_widget_set_margin_bottom(nome_evento, 1);
    gtk_widget_set_margin_start(nome_evento, 10);
    gtk_widget_set_margin_end(nome_evento, 1);

    GtkWidget *entry_nome = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_nome, 1, 6, 1, 1);

    GtkWidget *nome_responsavel = gtk_label_new("Nome do Responsavel");
    gtk_grid_attach(GTK_GRID(grid), nome_responsavel, 0, 7, 1, 1);
    gtk_widget_set_margin_top(nome_responsavel, 1);
    gtk_widget_set_margin_bottom(nome_responsavel, 1);
    gtk_widget_set_margin_start(nome_responsavel, 10);
    gtk_widget_set_margin_end(nome_responsavel, 1);

    GtkWidget *entry_responsavel = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_responsavel, 1, 7, 1, 1);

    GtkWidget *telefone = gtk_label_new("Telefone");
    gtk_grid_attach(GTK_GRID(grid), telefone, 0, 8, 1, 1);

    GtkWidget *entry_telefone = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_telefone, 1, 8, 1, 1);

    GtkWidget *cpf = gtk_label_new("Cpf/Cnpj");
    gtk_grid_attach(GTK_GRID(grid), cpf, 0, 9, 1, 1);

    GtkWidget *entry_cpf = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_cpf, 1, 9, 1, 1);

    GtkWidget *sala = gtk_label_new("Sala");
    gtk_grid_attach(GTK_GRID(grid), sala, 0, 10, 1, 1);

    GtkWidget *sala_drop = gtk_drop_down_new_from_strings(salas);
    gtk_grid_attach(GTK_GRID(grid), sala_drop, 1, 10, 1, 1);

    GtkWidget *botao_reg = gtk_button_new_with_label("Registrar");
    gtk_grid_attach(GTK_GRID(grid), botao_reg, 0, 11, 2, 1);
    gtk_widget_set_margin_top(botao_reg, 1);
    gtk_widget_set_margin_bottom(botao_reg, 10);
    gtk_widget_set_margin_start(botao_reg, 10);
    gtk_widget_set_margin_end(botao_reg, 1);
}



// ############################################# JANELA REGISTRAR HORARIO ##################################################

static void registro (GtkWidget *widget, gpointer data) {

    // abre uma janela pra registrar horarios

    GtkWidget *window_registro;
    GtkWidget *grid_registro;

    // setup da janela de modificar
    window_registro = gtk_window_new ();
    gtk_window_set_title (GTK_WINDOW (window_registro), "Registro");
    gtk_window_set_default_size(GTK_WINDOW(window_registro), 400, 300);

    // grid
    grid_registro = gtk_grid_new();
    gtk_window_set_child(GTK_WINDOW(window_registro), grid_registro);

    // Definindo o espaçamento entre as células do grid
    gtk_grid_set_row_spacing(GTK_GRID(grid_registro), 10);  // Espaçamento entre as linhas
    gtk_grid_set_column_spacing(GTK_GRID(grid_registro), 10);  // Espaçamento entre as colunas

    // titulo
    GtkWidget *titulo_registro = gtk_label_new("Registro de Evento");
    gtk_grid_attach(GTK_GRID(grid_registro), titulo_registro, 0, 0, 2, 1);
    gtk_widget_set_margin_top(titulo_registro, 30);
    gtk_widget_set_margin_bottom(titulo_registro, 30);
    gtk_widget_set_margin_start(titulo_registro, 10);
    gtk_widget_set_margin_end(titulo_registro, 3);

    // label data
    GtkWidget *label_data = gtk_label_new("Data");
    gtk_grid_attach(GTK_GRID(grid_registro), label_data, 0, 1, 1, 1);

    // botao selecione data
    GtkWidget *button = gtk_button_new_with_label("Selecione uma data");
    gtk_grid_attach(GTK_GRID(grid_registro), button, 1, 1, 1, 1);

    // Cria o popover
    GtkWidget *popover = gtk_popover_new();
    gtk_widget_set_parent(popover, button);

    // calendario
    GtkWidget *calendario = gtk_calendar_new();
    gtk_popover_set_child(GTK_POPOVER(popover), calendario);

    // Conecta o botão ao popover
    g_signal_connect(button, "clicked", G_CALLBACK(popover_botao), popover);

    // Conecta o sinal do calendário para capturar a data selecionada
    g_signal_connect(calendario, "day-selected", G_CALLBACK(date_selected), button);

    // label horario de inicio
    GtkWidget *hor_inilab = gtk_label_new("Horario de Inicio");
    gtk_grid_attach(GTK_GRID(grid_registro), hor_inilab, 0, 2, 1, 1);

    // --------------- drop down e seus horarios --------------------------------

    // Usa GStrvBuilder para criar um array de strings NULL-terminated
    GStrvBuilder *builder = g_strv_builder_new();

    for (int i = 0; i < 18; i++) {
        g_strv_builder_add(builder, horarios_inicio[i]);
    }

    // Cria o GtkStringList a partir do array de strings
    char **strv = g_strv_builder_end(builder);
    GtkStringList *string_list = gtk_string_list_new((const char *const *)strv);
    g_strfreev(strv); // Libera o array de strings

    GtkWidget *hor_ini = gtk_drop_down_new(G_LIST_MODEL(string_list), NULL);
    gtk_grid_attach(GTK_GRID(grid_registro), hor_ini, 1, 2, 1, 1);

    guint posicao_selecionada = gtk_drop_down_get_selected(GTK_DROP_DOWN(hor_ini));

    // guint posicao_selecionada = horario_selecionado(GTK_DROP_DOWN(hor_ini));

    // label horario fim
    GtkWidget *hor_fimlab = gtk_label_new("Horario do Fim");
    gtk_grid_attach(GTK_GRID(grid_registro), hor_fimlab, 0, 3, 1, 1);

    //drop down horario de fim

    GStrvBuilder *builder2 = g_strv_builder_new();

    for (int i = 0; i < 19; i++) {
        g_strv_builder_add(builder2, horarios_inicio[i]);
    }

    char **strv2 = g_strv_builder_end(builder2);
    GtkStringList *string_list2 = gtk_string_list_new((const char *const *)strv2);
    g_strfreev(strv2);

    GtkWidget *hor_fim = gtk_drop_down_new(G_LIST_MODEL(string_list2), NULL);
    gtk_grid_attach(GTK_GRID(grid_registro), hor_fim, 1, 3, 1, 1);

    // label monitoria?
    GtkWidget *monitoria_label = gtk_label_new("Monitoria?");
    gtk_grid_attach(GTK_GRID(grid_registro), monitoria_label, 0, 4, 2, 1);

    // check buttons sim e nao
    GtkWidget *radio_button1 = gtk_check_button_new_with_label("Sim");
    gtk_grid_attach(GTK_GRID(grid_registro), radio_button1, 0, 5, 1, 1);

    GtkWidget *radio_button2 = gtk_check_button_new_with_label("Nao");
    gtk_check_button_set_group(GTK_CHECK_BUTTON(radio_button2), GTK_CHECK_BUTTON(radio_button1));
    gtk_grid_attach(GTK_GRID(grid_registro), radio_button2, 1, 5, 1, 1);

    g_signal_connect(radio_button1, "toggled", G_CALLBACK(check_clicado_sim), grid_registro);
    g_signal_connect(radio_button2, "toggled", G_CALLBACK(check_clicado_nao), grid_registro);

    gtk_window_present (GTK_WINDOW (window_registro));

}

// ##################################################### JANELA DE REMOVER HORARIO ##################################################

static void remover (GtkButton *button, gpointer user_data) {
    GtkGrid *grid = GTK_GRID(user_data);

    // pegar data
    GtkWidget *data = gtk_grid_get_child_at(GTK_GRID(grid), 1, 2);
    const char *data_lab = gtk_button_get_label(GTK_BUTTON(data));

    char data_array[Size_data];

    strncpy(data_array, data_lab, sizeof(data_array) - 1);  // Usar strncpy para evitar buffer overflow
    data_array[sizeof(data_array) - 1] = '\0';

    // drop down horario de inicio e texto
    GtkWidget *horario_ini_drop = gtk_grid_get_child_at(GTK_GRID(grid), 1, 3);

    guint endereco_hor_ini = gtk_drop_down_get_selected(GTK_DROP_DOWN(horario_ini_drop));

    GtkStringList *lista = GTK_STRING_LIST(gtk_drop_down_get_model(GTK_DROP_DOWN(horario_ini_drop)));

    const char *horario_inicio_c_ponto = gtk_string_list_get_string(lista, endereco_hor_ini);

    char horario_inicio[10];

    remover_ponto(horario_inicio_c_ponto, horario_inicio);

    const char *horario_inicio_const = horario_inicio;
    char horario_inicio_array[Size_horario];

    strncpy(horario_inicio_array, horario_inicio_const, sizeof(horario_inicio_array) - 1);  // Usar strncpy para evitar buffer overflow
    horario_inicio_array[sizeof(horario_inicio_array) - 1] = '\0';

    // drop down horario de fim e texto
    GtkWidget *horario_fim_drop = gtk_grid_get_child_at(GTK_GRID(grid), 1, 4);

    guint endereco_hor_fim = gtk_drop_down_get_selected(GTK_DROP_DOWN(horario_fim_drop));

    GtkStringList *lista2 = GTK_STRING_LIST(gtk_drop_down_get_model(GTK_DROP_DOWN(horario_fim_drop)));

    const char *horario_fim_c_ponto = gtk_string_list_get_string(lista2, endereco_hor_fim);

    char horario_fim[10];

    remover_ponto(horario_fim_c_ponto, horario_fim);

    const char *horario_fim_const = horario_fim;
    char horario_fim_array[Size_horario];

    strncpy(horario_fim_array, horario_fim_const, sizeof(horario_fim_array) - 1);  // Usar strncpy para evitar buffer overflow
    horario_fim_array[sizeof(horario_fim_array) - 1] = '\0';

    // drop down sala e texto
    GtkWidget *sala_drop = gtk_grid_get_child_at(GTK_GRID(grid), 1, 1);

    guint endereco_sala_drop = gtk_drop_down_get_selected(GTK_DROP_DOWN(sala_drop));

    GtkStringList *lista3 = GTK_STRING_LIST(gtk_drop_down_get_model(GTK_DROP_DOWN(sala_drop)));

    const char *sala = gtk_string_list_get_string(lista3, endereco_sala_drop);
    char sala_array[Size_sala];

    strncpy(sala_array, sala, sizeof(sala_array) - 1);  // Usar strncpy para evitar buffer overflow
    sala_array[sizeof(sala_array) - 1] = '\0';

    removAlug(data_array, sala_array, horario_inicio_array);
}

static void modificar (GtkWidget *widget, gpointer data) {

    // abre uma janela pra modificar horarios

    GtkWidget *window_modificar;
    GtkWidget *grid_modificar;
    GtkWidget *botao_modificar2;

    // setup da janela de modificar
    window_modificar = gtk_window_new ();
    gtk_window_set_title (GTK_WINDOW (window_modificar), "Remover");
    gtk_window_set_default_size(GTK_WINDOW(window_modificar), 400, 300);

    // setup da grid
    grid_modificar = gtk_grid_new();
    gtk_window_set_child(GTK_WINDOW(window_modificar), grid_modificar);

    // Definindo o espaçamento entre as células do grid
    gtk_grid_set_row_spacing(GTK_GRID(grid_modificar), 10);  // Espaçamento entre as linhas
    gtk_grid_set_column_spacing(GTK_GRID(grid_modificar), 10);  // Espaçamento entre as colunas

    // titulo
    GtkWidget *titulo_modificar = gtk_label_new("Remover Registro");
    gtk_grid_attach(GTK_GRID(grid_modificar), titulo_modificar, 0, 0, 2, 1);
    gtk_widget_set_margin_top(titulo_modificar, 30);
    gtk_widget_set_margin_bottom(titulo_modificar, 1);
    gtk_widget_set_margin_start(titulo_modificar, 1);
    gtk_widget_set_margin_end(titulo_modificar, 1);

    // label sala
    GtkWidget *sala_label = gtk_label_new("Sala");
    gtk_grid_attach(GTK_GRID(grid_modificar), sala_label, 0, 1, 1, 1);
    gtk_widget_set_margin_top(sala_label, 1);
    gtk_widget_set_margin_bottom(sala_label, 1);
    gtk_widget_set_margin_start(sala_label, 30);
    gtk_widget_set_margin_end(sala_label, 1);

    // drop down sala
    GtkWidget *sala_drop = gtk_drop_down_new_from_strings(salas);
    gtk_grid_attach(GTK_GRID(grid_modificar), sala_drop, 1, 1, 1, 1);

    // label data
    GtkWidget *data_label = gtk_label_new("Data");
    gtk_grid_attach(GTK_GRID(grid_modificar), data_label, 0, 2, 1, 1);
    gtk_widget_set_margin_top(data_label, 1);
    gtk_widget_set_margin_bottom(data_label, 1);
    gtk_widget_set_margin_start(data_label, 30);
    gtk_widget_set_margin_end(data_label, 1);

    // label horario inicio
    GtkWidget *hor_ini_label = gtk_label_new("Horario de Inicio");
    gtk_grid_attach(GTK_GRID(grid_modificar), hor_ini_label, 0, 3, 1, 1);
    gtk_widget_set_margin_top(hor_ini_label, 1);
    gtk_widget_set_margin_bottom(hor_ini_label, 1);
    gtk_widget_set_margin_start(hor_ini_label, 30);
    gtk_widget_set_margin_end(hor_ini_label, 1);

    // Usa GStrvBuilder para criar um array de strings NULL-terminated
    GStrvBuilder *builder = g_strv_builder_new();

    for (int i = 0; i < 18; i++) {
        g_strv_builder_add(builder, horarios_inicio[i]);
    }

    // Cria o GtkStringList a partir do array de strings
    char **strv = g_strv_builder_end(builder);
    GtkStringList *string_list = gtk_string_list_new((const char *const *)strv);
    g_strfreev(strv); // Libera o array de strings

    // drop down horario inicio
    GtkWidget *hor_ini_drop = gtk_drop_down_new(G_LIST_MODEL(string_list), NULL);
    gtk_grid_attach(GTK_GRID(grid_modificar), hor_ini_drop, 1, 3, 1, 1);

    // label horario fim
    GtkWidget *hor_fim_label = gtk_label_new("Horario de Fim");
    gtk_grid_attach(GTK_GRID(grid_modificar), hor_fim_label, 0, 4, 1, 1);
    gtk_widget_set_margin_top(hor_fim_label, 1);
    gtk_widget_set_margin_bottom(hor_fim_label, 1);
    gtk_widget_set_margin_start(hor_fim_label, 30);
    gtk_widget_set_margin_end(hor_fim_label, 1);

    // Usa GStrvBuilder para criar um array de strings NULL-terminated
    GStrvBuilder *builder2 = g_strv_builder_new();

    for (int i = 0; i < 19; i++) {
        g_strv_builder_add(builder2, horarios_inicio[i]);
    }

    // Cria o GtkStringList a partir do array de strings
    char **strv2 = g_strv_builder_end(builder2);
    GtkStringList *string_list2 = gtk_string_list_new((const char *const *)strv2);
    g_strfreev(strv2); // Libera o array de strings

    // drop down horario fim
    GtkWidget *hor_fim_drop = gtk_drop_down_new(G_LIST_MODEL(string_list2), NULL);
    gtk_grid_attach(GTK_GRID(grid_modificar), hor_fim_drop, 1, 4, 1, 1);

    // ############## setup do calendario da tela de modificar ###############

    // cria o botao de selecionar a data
    GtkWidget *botao = gtk_button_new_with_label("Selecione a Data");
    gtk_grid_attach(GTK_GRID(grid_modificar), botao, 1, 2, 1, 1);

    // Cria o popover
    GtkWidget *popover = gtk_popover_new();
    gtk_widget_set_parent(popover, botao);

    // calendario
    GtkWidget *calendario = gtk_calendar_new();
    gtk_popover_set_child(GTK_POPOVER(popover), calendario);

    // Conecta o botão ao popover
    g_signal_connect(botao, "clicked", G_CALLBACK(popover_botao), popover);

    // Conecta o sinal do calendário para capturar a data selecionada
    g_signal_connect(calendario, "day-selected", G_CALLBACK(date_selected), botao);

    // setup do botao verificar evento
    botao_modificar2 = gtk_button_new_with_label("Remover Evento");
    gtk_grid_attach(GTK_GRID(grid_modificar), botao_modificar2, 0, 5, 2, 1);
    gtk_widget_set_margin_top(botao_modificar2, 1);
    gtk_widget_set_margin_bottom(botao_modificar2, 30);
    gtk_widget_set_margin_start(botao_modificar2, 30);
    gtk_widget_set_margin_end(botao_modificar2, 1);

    g_signal_connect(botao_modificar2, "clicked", G_CALLBACK(remover), grid_modificar);

    gtk_window_present (GTK_WINDOW (window_modificar));
}

// ################################################### JANELA PRINCIPAL ####################################################

static void principal(GtkApplication *app, gpointer user_data) {
    FILE *fptr = fopen("planilha13122024.csv", "r");

    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *botao_registro;
    GtkWidget *botao_modificar;
    GtkWidget *sala[35];
    GtkWidget *horario[18];
    GtkWidget *labels[630];
    GtkWidget *botoes[630];
    GtkWidget *scr;

    char *horarios[] = {
        " 07:10 - 08:00 ", " 08:00 - 08:50 ", " 08:50 - 09:40 ", " 09:40 - 10:30 ",
        " 10:30 - 11:20 ", " 11:20 - 12:10 ", " 12:10 - 13:00 ", " 13:00 - 13:50 ",
        " 13:50 - 14:40 ", " 14:40 - 15:30 ", " 15:30 - 16:20 ", " 16:20 - 17:10 ",
        " 17:10 - 18:00 ", " 18:00 - 18:50 ", " 18:50 - 19:40 ", " 19:40 - 20:30 ",
        " 20:30 - 21:20 ", " 21:20 - 22:10 "
    };

    // setup da janela
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Controle de Salas");
    gtk_window_maximize(GTK_WINDOW(window));

    // setup do scroll
    scr = gtk_scrolled_window_new();
    gtk_window_set_child(GTK_WINDOW(window), scr);

    // setup do grid
    grid = gtk_grid_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scr), grid);

    // setup das labels das salas
    for (int i = 0; i < 35; i++){
        sala[i] = gtk_label_new(salas[i]);
        gtk_grid_attach(GTK_GRID(grid), sala[i], 0, i+2, 1, 1);
        gtk_widget_set_margin_top(sala[i], 3);
        gtk_widget_set_margin_bottom(sala[i], 3);
        gtk_widget_set_margin_start(sala[i], 50);
        gtk_widget_set_margin_end(sala[i], 3);
    };

    // setup dos horarios
    for (int i = 0; i < 18; i++) {
        horario[i] = gtk_label_new(horarios[i]);
        gtk_grid_attach(GTK_GRID(grid), horario[i], i+1, 1, 1, 1);
        gtk_widget_set_margin_top(horario[i], 30);
        gtk_widget_set_margin_bottom(horario[i], 3);
        gtk_widget_set_margin_start(horario[i], 5);
        gtk_widget_set_margin_end(horario[i], 5);
    }

    // ---------------- drop down da data

    // cria o botao de selecionar a data
    GtkWidget *botao = gtk_button_new_with_label("Selecione a Data");
    gtk_grid_attach(GTK_GRID(grid), botao, 2, 0, 2, 1);
    gtk_widget_set_margin_top(botao, 30);
    gtk_widget_set_margin_bottom(botao, 10);

    // Cria o popover
    GtkWidget *popover = gtk_popover_new();
    gtk_widget_set_parent(popover, botao);

    // calendario
    GtkWidget *calendario = gtk_calendar_new();
    gtk_popover_set_child(GTK_POPOVER(popover), calendario);

    // Conecta o botão ao popover
    g_signal_connect(botao, "clicked", G_CALLBACK(popover_botao), popover);

    // Conecta o sinal do calendário para capturar a data selecionada
    g_signal_connect(calendario, "day-selected", G_CALLBACK(date_selected), botao);

    // setup do botao de registro
    botao_registro = gtk_button_new_with_label("Registrar");
    g_signal_connect(botao_registro, "clicked", G_CALLBACK(registro), NULL);
    gtk_grid_attach(GTK_GRID(grid), botao_registro, 5, 0, 2, 1);
    gtk_widget_set_margin_top(botao_registro, 30);
    gtk_widget_set_margin_bottom(botao_registro, 10);
    gtk_widget_set_margin_start(botao_registro, 3);
    gtk_widget_set_margin_end(botao_registro, 3);

    // setup do botao de modificar
    botao_modificar = gtk_button_new_with_label("Modificar");
    g_signal_connect(botao_modificar, "clicked", G_CALLBACK(modificar), NULL);
    gtk_grid_attach(GTK_GRID(grid), botao_modificar, 8, 0, 2, 1);
    gtk_widget_set_margin_top(botao_modificar, 30);
    gtk_widget_set_margin_bottom(botao_modificar, 10);
    gtk_widget_set_margin_start(botao_modificar, 3);
    gtk_widget_set_margin_end(botao_modificar, 3);


    // ------------------- setup dos botoes -------------------------------

    // Variáveis
    char line[1024]; // Buffer para ler as linhas do arquivo

    int linha_atual = 0; // Variável para contar as linhas
    int posicao1 = 0;    // Para percorrer as células do arquivo

    fgets(line, sizeof(line), fptr);

    // Lê as linhas subsequentes (as salas e as células com 'L' ou 'O')
    while (fgets(line, sizeof(line), fptr)) {
        posicao1 = 4;
        // Processa a linha para cada sala
        for (int j = 0; j < 18; j++) {

            if (line[posicao1] == 'L') {
                labels[j + linha_atual] = gtk_label_new("LIVRE");

                gtk_grid_attach(GTK_GRID(grid), labels[j + linha_atual], j + 1, linha_atual + 2, 1, 1);
            } else {

                //
                char sala_atual[Size_sala];

                int i = 0; // contador
                while (line[i] != ';') {
                    sala_atual[i] = line[i];
                    i++;
                }

                sala_atual[i] = '\0';

                char *horario_inicio_atual = horarios_inicio_sem_ponto[j];

                char *estado = OqueOcupado("13122024.csv", "13122024", sala_atual, horario_inicio_atual);

                labels[j + linha_atual] = gtk_label_new(estado);

                gtk_grid_attach(GTK_GRID(grid), labels[j + linha_atual], j + 1, linha_atual + 2, 1, 1);
            }
            posicao1 += 2;
        }
        linha_atual++; // Incrementa a linha atual
    }

    fclose(fptr); // Fecha o arquivo


    gtk_window_present (GTK_WINDOW (window));

}


void attInfos(const char *data_do_csv, const char *data, const char *sala, const char *horario) {
    FILE *pont_csv = fopen(data_do_csv, "r");
    FILE *pont_temp = fopen("temp.csv", "w");

    struct aluguel aluguel_existente, aluguel_novo;
    int start_temp = 0;
    int encontrou = 0;

    printf("Tentando abrir o arquivo: %s\n", data_do_csv);

    if (pont_csv == NULL || pont_temp == NULL) {
        printf("Erro ao procurar o arquivo, insira uma data valida ou tente novamente. :C\n");
        if (pont_csv) fclose(pont_csv);
        if (pont_temp) fclose(pont_temp);
        return;
    }

    while (fscanf(pont_csv, "%8[^;];%20[^;];%4[^;];%4[^;];%51[^;];%15[^;];%21[^;];%52[^;];%1[^;];%1000[^;];\n",
                  aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario, aluguel_existente.horarioFim,
                  aluguel_existente.nome, aluguel_existente.cpf, aluguel_existente.celular,
                  aluguel_existente.prof_responsavel, aluguel_existente.monitor_sn,
                  aluguel_existente.evento) == 10) {

    if (compDataHorSala(aluguel_existente.data, data, aluguel_existente.sala, sala, aluguel_existente.horario, horario)) {
        aluguel_novo = aluguel_existente;

        printf(":D Registro encontrado! Insira as novas informações ou pressione ENTER para manter os valores atuais:\n");

            printf("Nome do solicitante [%s]: ", aluguel_novo.nome);
            fgets(aluguel_novo.nome, Size_nome, stdin);
            aluguel_novo.nome[strcspn(aluguel_novo.nome, "\n")] = '\0';
            if (strlen(aluguel_novo.nome) == 0) {
                strcpy(aluguel_novo.nome, aluguel_existente.nome);
            }
            if (strlen(aluguel_novo.nome) == Size_nome - 1 && aluguel_novo.nome[Size_nome - 2] != '\n'){
                    limparBuffer();}

            printf("CPF [%s]: ", aluguel_novo.cpf);
            fgets(aluguel_novo.cpf, Size_cpf, stdin);
            aluguel_novo.cpf[strcspn(aluguel_novo.cpf, "\n")] = '\0';
            if (strlen(aluguel_novo.cpf) == 0) {
                strcpy(aluguel_novo.cpf, aluguel_existente.cpf);
            }
            if (strlen(aluguel_novo.cpf) == Size_cpf - 1 && aluguel_novo.cpf[Size_cpf - 2] != '\n'){
                    limparBuffer();}

            printf("Celular para contato [%s]: ", aluguel_novo.celular);
            fgets(aluguel_novo.celular, Size_numero, stdin);
            aluguel_novo.celular[strcspn(aluguel_novo.celular, "\n")] = '\0';
            if (strlen(aluguel_novo.celular) == 0) {
                strcpy(aluguel_novo.celular, aluguel_existente.celular);
            }
            if (strlen(aluguel_novo.celular) == Size_numero - 1 && aluguel_novo.celular[Size_numero - 2] != '\n'){
                    limparBuffer();}

            printf("Professor responsavel [%s]: ", aluguel_novo.prof_responsavel);
            fgets(aluguel_novo.prof_responsavel, Size_prof_responsavel, stdin);
            aluguel_novo.prof_responsavel[strcspn(aluguel_novo.prof_responsavel, "\n")] = '\0';
            if (strlen(aluguel_novo.prof_responsavel) == 0) {
                strcpy(aluguel_novo.prof_responsavel, aluguel_existente.prof_responsavel);
            }
            if (strlen(aluguel_novo.prof_responsavel) == Size_prof_responsavel - 1 && aluguel_novo.prof_responsavel[Size_prof_responsavel - 2] != '\n'){
                    limparBuffer();}

            printf("Monitor? (S/N) [%s]: ", aluguel_novo.monitor_sn);
            fgets(aluguel_novo.monitor_sn, Size_monitor_sn, stdin);
            aluguel_novo.monitor_sn[strcspn(aluguel_novo.monitor_sn, "\n")] = '\0';
            if (strlen(aluguel_novo.monitor_sn) == 0) {
                strcpy(aluguel_novo.monitor_sn, aluguel_existente.monitor_sn);
            }
            if (strlen(aluguel_novo.monitor_sn) == Size_monitor_sn - 1 && aluguel_novo.monitor_sn[Size_monitor_sn - 2] != '\n'){
                    limparBuffer();}

            printf("Ocorrencia [%s]: ", aluguel_novo.evento);
            fgets(aluguel_novo.evento, Size_evento, stdin);
            aluguel_novo.evento[strcspn(aluguel_novo.evento, "\n")] = '\0';
            if (strlen(aluguel_novo.evento) == 0) {
                strcpy(aluguel_novo.evento, aluguel_existente.evento);
            }
            if (strlen(aluguel_novo.evento) == Size_evento - 1 && aluguel_novo.evento[Size_evento - 2] != '\n'){
                    limparBuffer();}

            encontrou = 1;

        } else {
            if (start_temp == 0){
            fprintf(pont_temp, "1;2;3;4;5;6;7;8;9;10;");
            start_temp = 1;}

            // Se não for o registro para alterar, copia o registro atual para o arquivo temporário
            fprintf(pont_temp, "\n%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;",
            aluguel_existente.data, aluguel_existente.sala, aluguel_existente.horario, aluguel_existente.horarioFim, aluguel_existente.nome,
            aluguel_existente.cpf, aluguel_existente.celular, aluguel_existente.prof_responsavel, aluguel_existente.monitor_sn,
            aluguel_existente.evento);
        }
    }

    if (encontrou) {
        fprintf(pont_temp, "\n%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;",
            aluguel_novo.data, aluguel_novo.sala, aluguel_novo.horario, aluguel_novo.horarioFim, aluguel_novo.nome,
            aluguel_novo.cpf, aluguel_novo.celular, aluguel_novo.prof_responsavel, aluguel_novo.monitor_sn,
            aluguel_novo.evento);

        fclose(pont_csv);
        fclose(pont_temp);

        remove(data_do_csv);
        rename("temp.csv", data_do_csv);

        limparAluguel(&aluguel_existente);
        limparAluguel(&aluguel_novo);

        printf("Registro atualizado! C:\n");
    } else {
        fclose(pont_csv);
        fclose(pont_temp);

        limparAluguel(&aluguel_existente);
        limparAluguel(&aluguel_novo);

        printf("Registro nao encontrado. :C\n");
        remove("temp.csv");
    }
}

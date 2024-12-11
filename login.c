#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Size_login 50
#define Size_senha 30
#define adm_login "adm"
#define adm_senha "1234"
#define csv_login "login.csv"

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

//Verificacao de existencia do arquivo
int arquivoExiste(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo) {
        fclose(arquivo);
        return 1;
    }
    return 0;
}

//Login
int logar(const char *loginInserido, const char *senha_inserida) {
    char login[Size_login], senha[Size_senha];
    FILE *arquivo = fopen(csv_login, "r");

    if (!arquivo) {
        printf("Erro ao abrir o arquivo de usuarios.\n");
        return 0;
    }

    while (fscanf(arquivo, "%[^,],%s\n", login, senha) != EOF) {
        if (strcmp(loginInserido, login) == 0 && strcmp(senha_inserida, senha) == 0) {
            fclose(arquivo);
            return 1;
        }
    }

    fclose(arquivo);
    return 0;  //caso o login falhe
}

//Adicao de usuarios
void adicionarUsuario() {
    char login[Size_login], senha[Size_senha];
    printf("Digite o nome de usuário: ");
    fgets(login, Size_login, stdin);
    login[strcspn(login, "\n")] = '\0';
    if (strlen(login) == Size_login - 1 && login[Size_login - 2] != '\n') {
        limparBuffer();
    }

    // Verificar se o login já existe
    FILE *arquivo = fopen(csv_login, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo para verificação. :C\n");
        return;
    }

    char linha[Size_login + Size_senha + 3];
    while (fgets(linha, sizeof(linha), arquivo)) {
        char loginExistente[Size_login], senhaExistente[Size_senha];
        sscanf(linha, "%[^;];%[^;];", loginExistente, senhaExistente);
        if (strcmp(loginExistente, login) == 0) {
            printf("Erro: o login '%s' já existe. Por favor, escolha outro nome de usuário.\n", login);
            fclose(arquivo);
            return;
        }
    }
    fclose(arquivo);

    // Solicitar senha
    printf("Digite a senha: ");
    fgets(senha, Size_senha, stdin);
    senha[strcspn(senha, "\n")] = '\0';
    if (strlen(senha) == Size_senha - 1 && senha[Size_senha - 2] != '\n') {
        limparBuffer();
    }

    // Adicionar novo usuário
    arquivo = fopen(csv_login, "a");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo para adicionar usuario. :C\n");
        return;
    }

    fprintf(arquivo, "%s;%s;\n", login, senha);
    fclose(arquivo);
    printf("Usuario adicionado com sucesso! :D\n");
}

//Alteracao de usuarios
void alterarSenha() {
    char login[Size_login], senha[Size_senha], senhaNova[Size_senha];
    printf("Insira o nome de usuario cuja senha deseja alterar: ");
    fgets(login, Size_login, stdin);
    login[strcspn(login, "\n")] = '\0';
    if (strlen(login) == Size_login - 1 && login[Size_login - 2] != '\n') {
        limparBuffer();
    }

    printf("Insira a senha atual: ");
    fgets(senha, Size_senha, stdin);
    senha[strcspn(senha, "\n")] = '\0';
    if (strlen(senha) == Size_senha - 1 && senha[Size_senha - 2] != '\n') {
        limparBuffer();
    }

    FILE *arquivo = fopen(csv_login, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo para leitura. :C\n");
        return;
    }

    FILE *temp = fopen("temp.csv", "w");
    if (!temp) {
        printf("Erro ao criar arquivo temporário. :C\n");
        fclose(arquivo);
        return;
    }

    int encontrado = 0;
    char linha[Size_login + Size_senha + 3];
    while (fgets(linha, sizeof(linha), arquivo)) {
        char loginAnt[Size_login], senhaAnt[Size_senha];
        sscanf(linha, "%[^;];%[^;];", loginAnt, senhaAnt);

        if (strcmp(loginAnt, login) == 0 && strcmp(senhaAnt, senha) == 0) {
            printf("Insira a nova senha: ");
            fgets(senhaNova, Size_senha, stdin);
            senhaNova[strcspn(senhaNova, "\n")] = '\0';
            if (strlen(senhaNova) == Size_senha - 1 && senhaNova[Size_senha - 2] != '\n') {
                limparBuffer();
            }
            fprintf(temp, "%s;%s;\n", loginAnt, senhaNova);
            encontrado = 1;
        } else {
            fputs(linha, temp);
        }
    }

    fclose(arquivo);
    fclose(temp);

    if (encontrado) {
        remove(csv_login);
        rename("temp.csv", csv_login);
        printf("Senha alterada! :D\n");
    } else {
        remove("temp.csv");
        printf("Usuario ou senha incorretos. :/\n");
    }
}

//Remocao de usuario
void removerUsuario() {
    char login[Size_login];
    printf("Insira o login a ser removido: ");
    fgets(login, Size_login, stdin);
    login[strcspn(login, "\n")] = '\0';
    if (strlen(login) == Size_login - 1 && login[Size_login - 2] != '\n') {
        limparBuffer();
    }

    FILE *arquivo = fopen(csv_login, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo para remover usuario. :/\n");
        return;
    }

    FILE *pont_temp = fopen("temp.csv", "w");
    if (!pont_temp) {
        printf("Erro ao criar o arquivo temporario. :/\n");
        fclose(arquivo);
        return;
    }

    int encontrado = 0;
    char linha[Size_login + Size_senha + 3];

    while (fgets(linha, sizeof(linha), arquivo)) {
        char loginAnt[Size_login];
        sscanf(linha, "%[^;];", loginAnt);

        if (strcmp(loginAnt, login) != 0) {
            fputs(linha, pont_temp);
        } else {
            encontrado = 1;
        }
    }

    fclose(arquivo);
    fclose(pont_temp);

    if (encontrado) {
        remove(csv_login);
        rename("temp.csv", csv_login);
        printf("Usuario removido! :D\n");
    } else {
        remove("temp.csv");
        printf("Usuario nao encontrado. :/\n");
    }
}

//terminal
int main() {
    char login[Size_login], senha[Size_senha];
    char opcao;
    
    if (!arquivoExiste(csv_login)) {
        FILE *arquivo = fopen(csv_login, "w");
        fclose(arquivo);
    }
    while (1 == 1)
    {
        printf("Insira seu login: ");
        fgets(login, Size_login, stdin);
        login[strcspn(login, "\n")] = '\0';
        if (strlen(login) == Size_login - 1 && login[Size_login - 2] != '\n') {
            limparBuffer();
        }

        printf("Insira sua senha: ");
        fgets(senha, Size_senha, stdin);
        senha[strcspn(senha, "\n")] = '\0';
        if (strlen(senha) == Size_senha - 1 && senha[Size_senha - 2] != '\n') {
            limparBuffer();
        }

        if (strcmp(login, adm_login) == 0 && strcmp(senha, adm_senha) == 0) 
        {
            printf("Logado como administrador.\n");
    
            do {
                printf("\nMenu:\n");
                printf("(1) Adicionar Usuário\n");
                printf("(2) Alterar Senha de Usuário\n");
                printf("(3) Remover Usuário\n");
                printf("(4) Sair\n");
                printf("Insira a opção: ");
                opcao = getchar();
                limparBuffer();
    
                switch (opcao) {
                    case '1':
                        adicionarUsuario();
                        return 1;
                        break;
                    case '2':
                        alterarSenha();
                        break;
                    case '3':
                        removerUsuario();
                        break;
                    case '4':
                        printf("Seção encerrada.\n");
                        return 0;
                    default:
                        printf("Opção invalida. Insira inteiros de 1 a 4.\n");
                }
            } while (opcao != '4');
        }
            
        else if (logar(login, senha)) 
        {
            printf("Login realizado ! :D\n");
            do {
                printf("\nMenu:\n");
                printf("(1) Adicionar Usuário\n");
                printf("(2) Alterar Senha de Usuário\n");
                printf("(3) Remover Usuário\n");
                printf("(4) Sair\n");
                printf("Insira a opção: ");
                if (scanf("%d", &opcao) != 1) {
                    // Caso a entrada não seja válida
                    printf("Erro: Entrada inválida. Por favor, insira um número inteiro.\n");
                    limparBuffer();
                }
    
                switch (opcao) {
                    case 1:
                        adicionarUsuario();
                        break;
                    case 2:
                        alterarSenha();
                        break;
                    case 3:
                        removerUsuario();
                        break;
                    case 4:
                        printf("Seção encerrada.\n");
                        return 0;
                    default:
                        printf("Opção invalida. Insira inteiros de 1 a 4.\n");
                }
            } while (opcao != 4);
        } 
        else 
        {
            printf("Usuario ou senha invalidos. :/\n");
        }
    }
}

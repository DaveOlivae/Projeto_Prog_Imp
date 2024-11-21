#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 100
#define ADMIN_USER "admin"
#define ADMIN_PASS "admin123"
#define FILE_NAME "usuarios.csv"

// Função para verificar se o arquivo de usuários existe
int arquivoExiste(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo) {
        fclose(arquivo);
        return 1;
    }
    return 0;
}

// Função para fazer login
int fazerLogin(const char *usuario, const char *senha) {
    char login[MAX_LEN], senhaArq[MAX_LEN];
    FILE *arquivo = fopen(FILE_NAME, "r");

    if (!arquivo) {
        printf("Erro ao abrir o arquivo de usuários.\n");
        return 0;
    }

    while (fscanf(arquivo, "%[^,],%s\n", login, senhaArq) != EOF) {
        if (strcmp(usuario, login) == 0 && strcmp(senha, senhaArq) == 0) {
            fclose(arquivo);
            return 1;  // Login bem-sucedido
        }
    }

    fclose(arquivo);
    return 0;  // Login falhou
}

// Função para adicionar um novo usuário
void adicionarUsuario() {
    char login[MAX_LEN], senha[MAX_LEN];
    printf("Digite o nome de usuário: ");
    scanf("%s", login);
    printf("Digite a senha: ");
    scanf("%s", senha);

    FILE *arquivo = fopen(FILE_NAME, "a");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo para adicionar usuário.\n");
        return;
    }

    fprintf(arquivo, "%s,%s\n", login, senha);
    fclose(arquivo);
    printf("Usuário adicionado com sucesso!\n");
}

// Função para alterar a senha de um usuário
void alterarSenha() {
    char login[MAX_LEN], senha[MAX_LEN], novaSenha[MAX_LEN];
    printf("Digite o nome de usuário para alterar a senha: ");
    scanf("%s", login);
    printf("Digite a senha atual: ");
    scanf("%s", senha);

    FILE *arquivo = fopen(FILE_NAME, "r+");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo para alterar senha.\n");
        return;
    }

    int encontrado = 0;
    char linha[MAX_LEN * 2];
    long posicao;

    while ((posicao = ftell(arquivo)) != -1 && fgets(linha, sizeof(linha), arquivo)) {
        char loginArq[MAX_LEN], senhaArq[MAX_LEN];
        sscanf(linha, "%[^,],%s", loginArq, senhaArq);

        if (strcmp(loginArq, login) == 0 && strcmp(senhaArq, senha) == 0) {
            printf("Digite a nova senha: ");
            scanf("%s", novaSenha);
            
            fseek(arquivo, posicao, SEEK_SET);
            fprintf(arquivo, "%s,%s\n", loginArq, novaSenha);
            encontrado = 1;
            break;
        }
    }

    fclose(arquivo);
    if (encontrado) {
        printf("Senha alterada com sucesso!\n");
    } else {
        printf("Usuário ou senha incorretos.\n");
    }
}

// Função para remover um usuário
void removerUsuario() {
    char login[MAX_LEN];
    printf("Digite o nome de usuário para remover: ");
    scanf("%s", login);

    FILE *arquivo = fopen(FILE_NAME, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo para remover usuário.\n");
        return;
    }

    FILE *temp = fopen("temp.csv", "w");
    if (!temp) {
        printf("Erro ao criar arquivo temporário.\n");
        fclose(arquivo);
        return;
    }

    int encontrado = 0;
    char linha[MAX_LEN * 2];

    while (fgets(linha, sizeof(linha), arquivo)) {
        char loginArq[MAX_LEN];
        sscanf(linha, "%[^,],", loginArq);

        if (strcmp(loginArq, login) != 0) {
            fputs(linha, temp);
        } else {
            encontrado = 1;
        }
    }

    fclose(arquivo);
    fclose(temp);

    if (encontrado) {
        remove(FILE_NAME);
        rename("temp.csv", FILE_NAME);
        printf("Usuário removido com sucesso!\n");
    } else {
        remove("temp.csv");
        printf("Usuário não encontrado.\n");
    }
}

// Função principal
int main() {
    char usuario[MAX_LEN], senha[MAX_LEN];
    int opcao;
    
    if (!arquivoExiste(FILE_NAME)) {
        FILE *arquivo = fopen(FILE_NAME, "w");
        fclose(arquivo);
    }

    printf("Login (admin): %s / Senha: %s\n", ADMIN_USER, ADMIN_PASS);

    // Login de Administrador
    printf("Digite seu login: ");
    scanf("%s", usuario);
    printf("Digite sua senha: ");
    scanf("%s", senha);

    if (strcmp(usuario, ADMIN_USER) == 0 && strcmp(senha, ADMIN_PASS) == 0) {
        printf("Login de administrador bem-sucedido!\n");

        do {
            printf("\nMenu:\n");
            printf("1. Adicionar Usuário\n");
            printf("2. Alterar Senha de Usuário\n");
            printf("3. Remover Usuário\n");
            printf("4. Sair\n");
            printf("Escolha uma opção: ");
            scanf("%d", &opcao);

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
                    printf("Saindo...\n");
                    break;
                default:
                    printf("Opção inválida!\n");
            }
        } while (opcao != 4);
    } else {
        if (fazerLogin(usuario, senha)) {
            printf("Login bem-sucedido!\n");
        } else {
            printf("Usuário ou senha inválidos.\n");
        }
    }

    return 0;
}
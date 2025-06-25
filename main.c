#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>

#define MAX_CLIENTES 1024
#define MAX_PRODUTOS 100

// Estrutura para Clientes
typedef struct {
    char cpf[15];
    char c_nome[50];
    char tel[20];
    char end[100];
    char proj[50];
} Cliente;

// Estrutura para Produtos
typedef struct {
    int id;
    char nome[50];
    char fornecedor[50];
    float precocusto;
    float precovenda;
    float margemlucro;
    int ativo; // 1 = ativo, 0 = excluído
} Produto;

// Funções de Clientes
void addCliente(Cliente c[], int *n) {
    if (*n >= MAX_CLIENTES) {
        printf("Limite de cadastros de clientes atingido!\n");
        return;
    }

    Cliente novo;

    printf("CPF: ");
    scanf(" %[^\n]", novo.cpf);
    printf("Nome: ");
    scanf(" %[^\n]", novo.c_nome);
    printf("Telefone: ");
    scanf(" %[^\n]", novo.tel);
    printf("Endereço: ");
    scanf(" %[^\n]", novo.end);
    printf("Projeto: ");
    scanf(" %[^\n]", novo.proj);

    c[*n] = novo;
    (*n)++;
    printf("Cliente cadastrado com sucesso!\n");
}

void pesquisarCliente(Cliente c[], int n) {
    char cpf[15];
    printf("CPF para pesquisar: ");
    scanf(" %[^\n]", cpf);

    for (int i = 0; i < n; i++) {
        if (strcmp(c[i].cpf, cpf) == 0) {
            printf("CPF: %s\n", c[i].cpf);
            printf("Nome: %s\n", c[i].c_nome);
            printf("Telefone: %s\n", c[i].tel);
            printf("Endereço: %s\n", c[i].end);
            printf("Projeto: %s\n", c[i].proj);
            return;
        }
    }

    printf("Cliente não encontrado.\n");
}

void editarCliente(Cliente c[], int n) {
    char cpf[15];
    printf("CPF do cliente para editar: ");
    scanf(" %[^\n]", cpf);

    for (int i = 0; i < n; i++) {
        if (strcmp(c[i].cpf, cpf) == 0) {
            printf("Novo nome: ");
            scanf(" %[^\n]", c[i].c_nome);
            printf("Novo telefone: ");
            scanf(" %[^\n]", c[i].tel);
            printf("Novo endereço: ");
            scanf(" %[^\n]", c[i].end);
            printf("Novo projeto: ");
            scanf(" %[^\n]", c[i].proj);
            printf("Cliente editado com sucesso!\n");
            return;
        }
    }
    printf("Cliente não encontrado.\n");
}

void excluirCliente(Cliente c[], int *n) {
    char cpf[15];
    printf("CPF do cliente para excluir: ");
    scanf(" %[^\n]", cpf);

    for (int i = 0; i < *n; i++) {
        if (strcmp(c[i].cpf, cpf) == 0) {
            for (int j = i; j < *n - 1; j++) {
                c[j] = c[j + 1];
            }
            (*n)--;
            printf("Cliente excluído com sucesso!\n");
            return;
        }
    }
    printf("Cliente não encontrado.\n");
}

// Funções de Produtos
void cadastrarProduto(Produto produtos[], int *totalProdutos) {
    if (*totalProdutos >= MAX_PRODUTOS) {
        printf("Limite de produtos cadastrados atingido!\n");
        return;
    }

    Produto p;
    p.id = *totalProdutos + 1;
    p.ativo = 1;

    printf("Nome do Produto: ");
    getchar();
    fgets(p.nome, sizeof(p.nome), stdin);
    p.nome[strcspn(p.nome, "\n")] = '\0';

    printf("Nome do Fornecedor: ");
    fgets(p.fornecedor, sizeof(p.fornecedor), stdin);
    p.fornecedor[strcspn(p.fornecedor, "\n")] = '\0';

    printf("Preco de Custo: ");
    scanf("%f", &p.precocusto);

    printf("Preco de Venda: ");
    scanf("%f", &p.precovenda);

    p.margemlucro = ((p.precovenda - p.precocusto) / p.precovenda) * 100.0;

    produtos[*totalProdutos] = p;
    (*totalProdutos)++;

    printf("Produto cadastrado com sucesso! ID: %d\n", p.id);
}

void listarProdutos(Produto produtos[], int totalProdutos) {
    if (totalProdutos == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    printf("\n    Lista de Produtos   \n");
    int encontrou = 0;
    for (int i = 0; i < totalProdutos; i++) {
        Produto p = produtos[i];
        if (p.ativo) {
            printf("ID: %d\n", p.id);
            printf("Produto: %s\n", p.nome);
            printf("Fornecedor: %s\n", p.fornecedor);
            printf("Preco de Custo: R$ %.2f\n", p.precocusto);
            printf("Preco de Venda: R$ %.2f\n", p.precovenda);
            printf("Margem de Lucro: %.2f%%\n", p.margemlucro);
            printf("----------------------------\n");
            encontrou = 1;
        }
    }
    if (!encontrou) {
        printf("Nenhum produto ativo cadastrado.\n");
    }
}

void excluirProduto(Produto produtos[], int totalProdutos) {
    if (totalProdutos == 0) {
        printf("Nenhum produto cadastrado para excluir.\n");
        return;
    }

    int idExcluir;
    printf("Digite o ID do produto que deseja excluir: ");
    scanf("%d", &idExcluir);

    int encontrado = 0;
    for (int i = 0; i < totalProdutos; i++) {
        if (produtos[i].id == idExcluir && produtos[i].ativo) {
            produtos[i].ativo = 0;
            printf("Produto ID %d excluído com sucesso.\n", idExcluir);
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) {
        printf("Produto com ID %d não encontrado ou já excluído.\n", idExcluir);
    }
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    system("chcp 1252 > null"); // Para acentuação no Windows

    // Dados de login
    char usuario_adm[] = "admin";
    char senha_adm[] = "admin";
    char usuario[20];
    char senha[20];

    // Dados dos cadastros
    Cliente clientes[MAX_CLIENTES];
    int totalClientes = 0;

    Produto produtos[MAX_PRODUTOS];
    int totalProdutos = 0;

    // === LOGIN ===
    printf("=== Sistema de Login ===\n");
    printf("Usuário: ");
    scanf("%s", usuario);
    printf("Senha: ");
    scanf("%s", senha);

    if (strcmp(usuario, usuario_adm) != 0 || strcmp(senha, senha_adm) != 0) {
        printf("Usuário ou senha incorretos. Encerrando o programa.\n");
        return 0;
    }

    printf("\nLogin concluído! Bem-vindo ao sistema ZenCODE, %s.\n", usuario);

    // MENU
    int opcao;

    do {
        printf("\n     MENU PRINCIPAL     \n");
        printf("1. Gerenciar Clientes\n");
        printf("2. Gerenciar Produtos\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                int opCliente;
                do {
                    printf("\n--- Menu Clientes ---\n");
                    printf("1. Adicionar Cliente\n");
                    printf("2. Pesquisar Cliente\n");
                    printf("3. Editar Cliente\n");
                    printf("4. Excluir Cliente\n");
                    printf("0. Voltar\n");
                    printf("Escolha uma opção: ");
                    scanf("%d", &opCliente);

                    switch (opCliente) {
                        case 1: addCliente(clientes, &totalClientes); break;
                        case 2: pesquisarCliente(clientes, totalClientes); break;
                        case 3: editarCliente(clientes, totalClientes); break;
                        case 4: excluirCliente(clientes, &totalClientes); break;
                        case 0: printf("Voltando...\n"); break;
                        default: printf("Opção inválida.\n");
                    }
                } while (opCliente != 0);
                break;
            }

            case 2: {
                int opProduto;
                do {
                    printf("\n--- Menu Produtos ---\n");
                    printf("1. Cadastrar Produto\n");
                    printf("2. Listar Produtos\n");
                    printf("3. Excluir Produto\n");
                    printf("0. Voltar\n");
                    printf("Escolha uma opção: ");
                    scanf("%d", &opProduto);

                    switch (opProduto) {
                        case 1: cadastrarProduto(produtos, &totalProdutos); break;
                        case 2: listarProdutos(produtos, totalProdutos); break;
                        case 3: excluirProduto(produtos, totalProdutos); break;
                        case 0: printf("Voltando...\n"); break;
                        default: printf("Opção inválida.\n");
                    }
                } while (opProduto != 0);
                break;
            }

            case 0:
                printf("Encerrando o sistema.\n");
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

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
    int ativo;
    int quantidade;
    char localizacao[50];
} Produto;

void salvarClientes(Cliente c[], int totalClientes) {
    FILE *f = fopen("clientes.txt", "w");
    if (f == NULL) return;
    for (int i = 0; i < totalClientes; i++) {
        fprintf(f, "%s|%s|%s|%s|%s\n", c[i].cpf, c[i].c_nome, c[i].tel, c[i].end, c[i].proj);
    }
    fclose(f);
}

void carregarClientes(Cliente c[], int *totalClientes) {
    FILE *f = fopen("clientes.txt", "r");
    if (f == NULL) return;
    while (!feof(f)) {
        Cliente temp;
        if (fscanf(f, " %[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", temp.cpf, temp.c_nome, temp.tel, temp.end, temp.proj) == 5) {
            c[*totalClientes] = temp;
            (*totalClientes)++;
        }
    }
    fclose(f);
}

void salvarProdutos(Produto p[], int totalProdutos) {
    FILE *f = fopen("produtos.txt", "w");
    if (f == NULL) return;
    for (int i = 0; i < totalProdutos; i++) {
        fprintf(f, "%d|%s|%s|%f|%f|%f|%d|%d|%s\n", p[i].id, p[i].nome, p[i].fornecedor,
                p[i].precocusto, p[i].precovenda, p[i].margemlucro, p[i].ativo, p[i].quantidade, p[i].localizacao);
    }
    fclose(f);
}

void carregarProdutos(Produto p[], int *totalProdutos) {
    FILE *f = fopen("produtos.txt", "r");
    if (f == NULL) return;
    while (!feof(f)) {
        Produto temp;
        if (fscanf(f, "%d|%[^|]|%[^|]|%f|%f|%f|%d|%d|%[^\n]\n", &temp.id, temp.nome, temp.fornecedor,
                   &temp.precocusto, &temp.precovenda, &temp.margemlucro, &temp.ativo, &temp.quantidade, temp.localizacao) == 9) {
            p[*totalProdutos] = temp;
            (*totalProdutos)++;
        }
    }
    fclose(f);
}

void addCliente(Cliente c[], int *n) {
    if (*n >= MAX_CLIENTES) return;
    Cliente novo;
    printf("CPF: "); scanf(" %[^\n]%*c", novo.cpf);
    printf("Nome: "); scanf(" %[^\n]%*c", novo.c_nome);
    printf("Telefone: "); scanf(" %[^\n]%*c", novo.tel);
    printf("Endereço: "); scanf(" %[^\n]%*c", novo.end);
    printf("Projeto: "); scanf(" %[^\n]%*c", novo.proj);
    c[*n] = novo;

    (*n)++;
    printf("Cliente cadastrado com sucesso!\n");
}

void pesquisarCliente(Cliente c[], int n) {
    char cpf[15];
    printf("CPF para pesquisar: ");
    scanf(" %[^\n]%*c", cpf);
    for (int i = 0; i < n; i++) {
        if (strcmp(c[i].cpf, cpf) == 0) {
            printf("CPF: %s\nNome: %s\nTelefone: %s\nEndereço: %s\nProjeto: %s\n", c[i].cpf, c[i].c_nome, c[i].tel, c[i].end, c[i].proj);
            return;
        }
    }
    printf("Cliente não encontrado.\n");
}

void editarCliente(Cliente c[], int n) {
    char cpf[15];
    printf("CPF do cliente para editar: ");
    scanf(" %[^\n]%*c", cpf);
    for (int i = 0; i < n; i++) {
        if (strcmp(c[i].cpf, cpf) == 0) {
            printf("Novo nome: "); scanf(" %[^\n]%*c", c[i].c_nome);
            printf("Novo telefone: "); scanf(" %[^\n]%*c", c[i].tel);
            printf("Novo endereço: "); scanf(" %[^\n]%*c", c[i].end);
            printf("Novo projeto: "); scanf(" %[^\n]%*c", c[i].proj);
            printf("Cliente editado com sucesso!\n");
            return;
        }
    }
    printf("Cliente não encontrado.\n");
}

void excluirCliente(Cliente c[], int *n) {
    char cpf[15];
    printf("CPF do cliente para excluir: ");
    scanf(" %[^\n]%*c", cpf);
    for (int i = 0; i < *n; i++) {
        if (strcmp(c[i].cpf, cpf) == 0) {
            // Move os clientes seguintes para "fechar" o espaço do excluído
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
void cadastrarProduto(Produto produtos[], int *totalProdutos) {
    if (*totalProdutos >= MAX_PRODUTOS) return;
    Produto p;
    p.id = *totalProdutos + 1;
    p.ativo = 1;
    printf("Nome do Produto: "); getchar(); fgets(p.nome, sizeof(p.nome), stdin);
    p.nome[strcspn(p.nome, "\n")] = '\0';
    printf("Nome do Fornecedor: "); fgets(p.fornecedor, sizeof(p.fornecedor), stdin);
    p.fornecedor[strcspn(p.fornecedor, "\n")] = '\0';
    printf("Preço de Custo: "); scanf("%f", &p.precocusto);
    printf("Preço de Venda: "); scanf("%f", &p.precovenda);
    p.margemlucro = ((p.precovenda - p.precocusto) / p.precovenda) * 100.0;
    printf("Quantidade em estoque: "); scanf("%d", &p.quantidade);
    printf("Local de armazenamento: "); getchar(); fgets(p.localizacao, sizeof(p.localizacao), stdin);
    p.localizacao[strcspn(p.localizacao, "\n")] = '\0';
    produtos[*totalProdutos] = p;
    (*totalProdutos)++;
    printf("Produto cadastrado com sucesso! ID: %d\n", p.id);
}

void listarProdutos(Produto produtos[], int totalProdutos) {
    if (totalProdutos == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }
    printf("\nLista de Produtos\n");
    for (int i = 0; i < totalProdutos; i++) {
        Produto p = produtos[i];
        if (p.ativo) {
            printf("ID: %d\nProduto: %s\nFornecedor: %s\nCusto: %.2f\nVenda: %.2f\nLucro: %.2f%%\nQuantidade: %d\nLocal: %s\n--------------------------\n",
                   p.id, p.nome, p.fornecedor, p.precocusto, p.precovenda, p.margemlucro, p.quantidade, p.localizacao);
        }
    }
}

void excluirProduto(Produto produtos[], int totalProdutos) {
    int idExcluir;
    printf("Digite o ID do produto que deseja excluir: ");
    scanf("%d", &idExcluir);
    for (int i = 0; i < totalProdutos; i++) {
        if (produtos[i].id == idExcluir && produtos[i].ativo) {
            produtos[i].ativo = 0;
            printf("Produto ID %d excluído com sucesso.\n", idExcluir);
            return;
        }
    }
    printf("Produto com ID %d não encontrado ou já excluído.\n", idExcluir);
}

void criarOrcamento(Cliente clientes[], int totalClientes, Produto produtos[], int totalProdutos) {
    if (totalClientes == 0 || totalProdutos == 0) {
        printf("Necessário ter pelo menos um cliente e um produto cadastrado.\n");
        return;
    }
    char cpf[15];
    printf("CPF do cliente: ");
    scanf(" %[^\n]%*c", cpf);
    int clienteEncontrado = -1;
    for (int i = 0; i < totalClientes; i++) {
        if (strcmp(clientes[i].cpf, cpf) == 0) {
            clienteEncontrado = i;
            break;
        }
    }
    if (clienteEncontrado == -1) {
        printf("Cliente não encontrado.\n");
        return;
    }
    int idProduto;
    printf("ID do produto: ");
    scanf("%d", &idProduto);
    int produtoEncontrado = -1;
    for (int i = 0; i < totalProdutos; i++) {
        if (produtos[i].id == idProduto && produtos[i].ativo) {
            produtoEncontrado = i;
            break;
        }
    }
    if (produtoEncontrado == -1) {
        printf("Produto não encontrado ou inativo.\n");
        return;
    }
    int qtd;
    printf("Quantidade desejada: ");
    scanf("%d", &qtd);
    if (qtd > produtos[produtoEncontrado].quantidade) {
        printf("Estoque insuficiente. Disponível: %d unidades.\n", produtos[produtoEncontrado].quantidade);
        return;
    }
    float total = qtd * produtos[produtoEncontrado].precovenda;
    produtos[produtoEncontrado].quantidade -= qtd;
    printf("\n--- ORÇAMENTO GERADO ---\n");
    printf("Cliente: %s\nProduto: %s\nPreço unitário: R$ %.2f\nQuantidade: %d\nValor Total: R$ %.2f\nLocal: %s\n-------------------------\n",
           clientes[clienteEncontrado].c_nome, produtos[produtoEncontrado].nome,
           produtos[produtoEncontrado].precovenda, qtd, total, produtos[produtoEncontrado].localizacao);
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    system("chcp 1252 > nul");

    char usuario_adm[] = "admin";
    char senha_adm[] = "admin";
    char usuario[20], senha[20];

    Cliente clientes[MAX_CLIENTES];
    int totalClientes = 0;
    carregarClientes(clientes, &totalClientes);

    Produto produtos[MAX_PRODUTOS];
    int totalProdutos = 0;
    carregarProdutos(produtos, &totalProdutos);

    printf("=== Sistema de Login ===\nUsuário: ");
    scanf("%s", usuario);
    printf("Senha: ");
    scanf("%s", senha);

    if (strcmp(usuario, usuario_adm) != 0 || strcmp(senha, senha_adm) != 0) {
        printf("Usuário ou senha incorretos.\n");
        return 0;
    }

    printf("\nLogin concluído! Bem-vindo ao sistema ZenCODE, %s.\n", usuario);

    int opcao;
    do {
        printf("\nMENU PRINCIPAL\n1. Gerenciar Clientes\n2. Gerenciar Produtos\n3. Criar Orçamento\n0. Sair\nEscolha uma opção: ");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1: {
                int opCliente;
                do {
                    printf("\n--- Menu Clientes ---\n1. Adicionar Cliente\n2. Pesquisar Cliente\n3. Editar Cliente\n4. Excluir Cliente\n0. Voltar\nEscolha uma opção: ");
                    scanf("%d", &opCliente);
                    switch (opCliente) {
                        case 1: addCliente(clientes, &totalClientes); break;
                        case 2: pesquisarCliente(clientes, totalClientes); break;
                        case 3: editarCliente(clientes, totalClientes); break;
                        case 4: excluirCliente(clientes, &totalClientes); break;
                        case 0: break;
                    }
                } while (opCliente != 0);
                break;
            }
            case 2: {
                int opProduto;
                do {
                    printf("\n--- Menu Produtos ---\n1. Cadastrar Produto\n2. Listar Produtos\n3. Excluir Produto\n0. Voltar\nEscolha uma opção: ");
                    scanf("%d", &opProduto);
                    switch (opProduto) {
                        case 1: cadastrarProduto(produtos, &totalProdutos); break;
                        case 2: listarProdutos(produtos, totalProdutos); break;
                        case 3: excluirProduto(produtos, totalProdutos); break;
                        case 0: break;
                    }
                } while (opProduto != 0);
                break;
            }
            case 3:
                criarOrcamento(clientes, totalClientes, produtos, totalProdutos);
                break;
            case 0:
                printf("Encerrando o sistema.\n");
                break;
        }
    } while (opcao != 0);

    salvarClientes(clientes, totalClientes);
    salvarProdutos(produtos, totalProdutos);
    return 0;
}
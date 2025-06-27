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

#define MAX_ORCAMENTOS 1024

// Extensão da struct Orcamento
typedef struct {
    int id;
    Cliente cliente;
    Produto produto;
    int qtd;
    float total;
} Orcamento;


void salvarOrcamentos(Orcamento o[], int totalOrcamentos) {
    FILE *f = fopen("orcamentos.txt", "w");
    if (!f) return;
    for (int i = 0; i < totalOrcamentos; i++) {
        fprintf(f, "%d|%s|%s|%d|%.2f\n",
            o[i].id,
            o[i].cliente.cpf,
            o[i].produto.nome,
            o[i].qtd,
            o[i].total
        );
    }
    fclose(f);
}

void carregarOrcamentos(Orcamento o[], int *totalOrcamentos) {
    FILE *f = fopen("orcamentos.txt", "r");
    if (!f) return;
    while (!feof(f)) {
        Orcamento temp;
        char cpf[15], nomeProd[50];
        if (fscanf(f, "%d|%14[^|]|%49[^|]|%d|%f\n",
                   &temp.id, cpf, nomeProd,
                   &temp.qtd, &temp.total) == 5) {
            // Apenas armazenamos CPF e nome do produto para referência;
            // se quiser linkar objetos completos, procure nos arrays
            strcpy(temp.cliente.cpf, cpf);
            strcpy(temp.produto.nome, nomeProd);
            o[*totalOrcamentos] = temp;
            (*totalOrcamentos)++;
        }
    }
    fclose(f);
}

void criarOrcamento(Cliente clientes[], int totalClientes,
                   Produto produtos[], int totalProdutos,
                   Orcamento orcamentos[], int *totalOrcamentos) {
    if (totalClientes == 0 || totalProdutos == 0) {
        printf("Necessário ter pelo menos um cliente e um produto cadastrados.\n");
        return;
    }
    if (*totalOrcamentos >= MAX_ORCAMENTOS) {
        printf("Limite de orçamentos atingido.\n");
        return;
    }
    char cpf[15];
    printf("CPF do cliente: ");
    scanf(" %14[^\n]", cpf);
    int ci = -1;
    for (int i = 0; i < totalClientes; i++)
        if (strcmp(clientes[i].cpf, cpf) == 0) { ci = i; break; }
    if (ci < 0) { printf("Cliente não encontrado.\n"); return; }

    int idP;
    printf("ID do produto: ");
    scanf("%d", &idP);
    int pi = -1;
    for (int i = 0; i < totalProdutos; i++)
        if (produtos[i].id == idP && produtos[i].ativo) { pi = i; break; }
    if (pi < 0) { printf("Produto não encontrado ou inativo.\n"); return; }

    int qtd; printf("Quantidade desejada: "); scanf("%d", &qtd);
    if (qtd > produtos[pi].quantidade) {
        printf("Estoque insuficiente. Disponível: %d.\n", produtos[pi].quantidade);
        return;
    }

    // Monta o orçamento
    Orcamento o;
    o.id       = (*totalOrcamentos) + 1;
    o.cliente  = clientes[ci];
    o.produto  = produtos[pi];
    o.qtd      = qtd;
    o.total    = qtd * produtos[pi].precovenda;

    // Atualiza estoque
    produtos[pi].quantidade -= qtd;

    // Armazena no array
    orcamentos[*totalOrcamentos] = o;
    (*totalOrcamentos)++;

    printf("\n--- ORÇAMENTO #%d ---\nCliente: %s\nProduto: %s\nUnitário: R$ %.2f\nQtd: %d\nTotal: R$ %.2f\n---------------------\n",
           o.id, o.cliente.c_nome, o.produto.nome,
           o.produto.precovenda, o.qtd, o.total);
}

void pesquisarOrcamento(Orcamento o[], int totalOrcamentos) {
    char cpf[15];
    printf("CPF do cliente para pesquisar: ");
    scanf(" %14[^\n]", cpf);
    for (int i = 0; i < totalOrcamentos; i++) {
        if (strcmp(o[i].cliente.cpf, cpf) == 0) {
            printf("ID: %d\nCliente: %s\nProduto: %s\nQuantidade: %d\nTotal: R$ %.2f\n--------------------------\n",
                   o[i].id, o[i].cliente.c_nome, o[i].produto.nome, o[i].qtd, o[i].total);
            return;
        }
    }
    printf("Orçamento não encontrado para o CPF %s.\n", cpf);
}

void editarOrcamento(Orcamento o[], int totalOrcamentos) {
    int id; printf("ID do orçamento para editar: "); scanf("%d", &id);
    for (int i = 0; i < totalOrcamentos; i++) {
        if (o[i].id == id) {
            int novaQtd;
            printf("Nova quantidade (atual %d): ", o[i].qtd);
            scanf("%d", &novaQtd);
            o[i].qtd   = novaQtd;
            o[i].total = novaQtd * o[i].produto.precovenda;
            printf("Orçamento #%d atualizado: Qtd = %d, Total = R$ %.2f\n",
                   id, o[i].qtd, o[i].total);
            return;
        }
    }
    printf("Orçamento com ID %d não encontrado.\n", id);
}

void excluirOrcamento(Orcamento o[], int *totalOrcamentos) {
    int id; printf("ID do orçamento para excluir: "); scanf("%d", &id);
    for (int i = 0; i < *totalOrcamentos; i++) {
        if (o[i].id == id) {
            // desloca
            for (int j = i; j < *totalOrcamentos - 1; j++)
                o[j] = o[j + 1];
            (*totalOrcamentos)--;
            printf("Orçamento #%d excluído com sucesso.\n", id);
            return;
        }
    }
    printf("Orçamento com ID %d não encontrado.\n", id);
}

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
    printf("CPF: ");
    scanf(" %[^\n]%*c", novo.cpf);
    printf("Nome: ");
    scanf(" %[^\n]%*c", novo.c_nome);
    printf("Telefone: ");
    scanf(" %[^\n]%*c", novo.tel);
    printf("Endereço: ");
    scanf(" %[^\n]%*c", novo.end);
    printf("Projeto: ");
    scanf(" %[^\n]%*c", novo.proj);
    c[*n] = novo;

    (*n)++;
    printf("Cliente cadastrado com sucesso!\n");
}

void pesquisarCliente(Cliente c[], int n) {
    char cpf[15];
    printf("CPF para pesquisar: ");
    scanf(" %14[^\n]", cpf);
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
    scanf(" %14[^\n]", cpf);
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
    scanf(" %14[^\n]", cpf);
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

//TODO fazer o mesmo com os outros tipos de dado


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

    Orcamento orcamentos[MAX_ORCAMENTOS];
    int totalOrcamentos = 0;
    carregarOrcamentos(orcamentos, &totalOrcamentos);


    //TODO
    // ORÇAMENTO
    //printf("=== Sistema de Login ===\nUsuário: ");
    //scanf("%s", usuario);
    ///printf("Senha: ");
   // scanf("%s", senha);

//if (strcmp(usuario, usuario_adm) != 0 || strcmp(senha, senha_adm) != 0) {
//        printf("Usuário ou senha incorretos.\n");
//return 0;
    //}

    printf("\nLogin concluído! Bem-vindo ao sistema ZenCODE, %s.\n", usuario);

    int opcao;
    do {

        printf("\nMENU PRINCIPAL\n1. Gerenciar Clientes\n2. Gerenciar Produtos\n3. Gerenciar Orçamentos\n0. Sair\nEscolha uma opção: ");
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
                        case 0: system("cls"); break;
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
            case 3: {
                int opOrcamento;
                do {

                    printf("\n--- Menu Orçamento ---\n1. Cadastrar Orçamento\n2. Pesquisar Orçamento\n3. Editar Orçamento\n4. Excluir Orçamento\n0. Voltar\nEscolha uma opção: ");
                    scanf("%d", &opOrcamento);
                    switch (opOrcamento) {
                        case 1: criarOrcamento(clientes, totalClientes, produtos, totalProdutos, orcamentos, &totalOrcamentos); break;
                        case 2: pesquisarOrcamento(orcamentos, totalOrcamentos);  break;
                        case 3: editarOrcamento(orcamentos, totalOrcamentos);  break;
                        case 4: excluirOrcamento(orcamentos, &totalOrcamentos); break;
                        case 0: break;
                    }
                } while (opOrcamento != 0);
                break;
                //criarOrcamento(clientes, totalClientes, produtos, totalProdutos);
                //break;
            }
            case 0:
                printf("Encerrando o sistema.\n");
                break;
        }
    } while (opcao != 0);

    salvarClientes(clientes, totalClientes);
    salvarProdutos(produtos, totalProdutos);
    salvarOrcamentos(orcamentos, totalOrcamentos);
    return 0;
}
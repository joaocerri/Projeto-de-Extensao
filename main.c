#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>

#define MAX_CLIENTES 1024
#define MAX_PRODUTOS 100
#define MAX_ORCAMENTOS 1024

// Estrutura para Clientes
typedef struct {
    char cpf[13];
    char c_nome[50];
    char tel[10];
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



// Extensão da struct Orcamento
typedef struct {
    int id;
    Cliente cliente;
    Produto produto;
    int qtd;
    float total;
    int id_produto;
} Orcamento;
void lerCampo(char *campo, char *nomeCampo);


void cls() {
    system("cls");
}
void lerCpf(char *str)
{
    int erro;
    do
    {
        erro = 0;

        printf("\nDigite o CPF: ");
        scanf(" %12[^\n]", str);
        fflush(stdin);

        if(strlen(str) != 11)
        {
            erro = 1;
            printf("\nERRO: O CPF deve possuir 11 (onze) dígitos!\n");
        }

    }while(erro == 1);
}
const char* getNomeCliente(Cliente clientes[], int totalClientes, char *cpf) {
    for (int i = 0; i < totalClientes; i++) {
        if (strcmp(clientes[i].cpf, cpf) == 0) {
            return clientes[i].c_nome;
        }
    }
    return "Sem Nome";
}
const char* getNomeProduto(Produto produtos[], int totalProdutos, int id) {
    for (int i = 0; i < totalProdutos; i++) {
        if (produtos[i].id == id && produtos[i].ativo) {
            return produtos[i].nome;
        }
    }
    return "Sem Nome";
}
void salvarOrcamentos(Orcamento o[], int totalOrcamentos) {
    FILE *f = fopen("orcamentos.txt", "w");
    if (!f) return;
    for (int i = 0; i < totalOrcamentos; i++) {
        fprintf(f, "%d|%s|%d|%.2f|%d\n",
            o[i].id,
            o[i].cliente.cpf,
            o[i].qtd,
            o[i].total,
            o[i].id_produto
        );
    }
    fclose(f);
}

void carregarOrcamentos(Orcamento o[], int *totalOrcamentos) {
    FILE *f = fopen("orcamentos.txt", "r");
    if (!f) return;
    while (!feof(f)) {
        Orcamento temp;
        char cpf[13];
        if (fscanf(f, "%d|%12[^|]|%d|%f|%d\n",
                   &temp.id, cpf,
                   &temp.qtd, &temp.total, &temp.id_produto) == 5) {
            // Apenas armazenamos CPF e nome do produto para referência;
            // se quiser linkar objetos completos, procure nos arrays
            strcpy(temp.cliente.cpf, cpf);
            // não precisa mais porque agora o nome do produto aparece dinamicamente
            //strcpy(temp.produto.nome, nomeProd);
            o[*totalOrcamentos] = temp;
            (*totalOrcamentos)++;
        }
    }
    fclose(f);
}

void criarOrcamento(Orcamento orcamentos[], int *totalOrcamentos, Cliente clientes[], int totalClientes, Produto produtos[], int totalProdutos) {
    cls();
    printf("1 - Criar Orçamento\n");
    if (totalClientes == 0 || totalProdutos == 0) {
        printf("Necessário ter pelo menos um cliente e um produto cadastrados.\n");
        return;
    }
    if (*totalOrcamentos >= MAX_ORCAMENTOS) {
        printf("\nErro: Os orçamentos atingiram sua capacidade máxima!\n");
        return;
    }
    char cpf[13];
    printf("CPF do cliente: ");
    lerCpf(cpf);
    //scanf(" %12[^\n]", cpf);
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
    o.id         = (*totalOrcamentos) + 1;
    o.cliente    = clientes[ci];
    o.produto    = produtos[pi];
    o.qtd        = qtd;
    o.total      = qtd * produtos[pi].precovenda;
    o.id_produto = produtos[pi].id;

    // Atualiza estoque
    produtos[pi].quantidade -= qtd;

    //pega o nome atualizado do cliente
    strcpy(o.cliente.c_nome, getNomeCliente(clientes, totalClientes, cpf));
    //pega o nome atualizado do produto
    strcpy(o.produto.nome, getNomeProduto(produtos, totalProdutos, o.id_produto));

    // Armazena no array
    orcamentos[*totalOrcamentos] = o;
    (*totalOrcamentos)++;

    printf("\n----- ORÇAMENTO #%d -----\n", o.id);
    printf("Cliente:        %s\n", o.cliente.c_nome);
    printf("Produto:        %s\n", o.produto.nome);
    printf("Valor unitário: R$ %.2f\n", o.produto.precovenda);
    printf("Quantidade:     %d\n", o.qtd);
    printf("Total:          R$ %.2f\n", o.total);
    printf("--------------------------\n");
}

void pesquisarOrcamento(Orcamento o[], int totalOrcamentos, Cliente clientes[], int totalClientes, Produto produtos[], int totalProdutos) {
    cls();
    char cpf[13];
    printf("2 - Pesquisar Orçamento\n");
    printf("CPF do cliente para pesquisar: ");

    lerCpf(cpf);

    for (int i = 0; i < totalOrcamentos; i++) {
        if (strcmp(o[i].cliente.cpf, cpf) == 0) {
            //pega o nome atualizado do cliente
            strcpy(o[i].cliente.c_nome, getNomeCliente(clientes, totalClientes, cpf));
            //pega o nome atualizado do produto
            strcpy(o[i].produto.nome, getNomeProduto(produtos, totalProdutos, o[i].id_produto));

            printf("\n----- ORÇAMENTO #%d -----\n", o[i].id);
            printf("Cliente:    %s\n", o[i].cliente.c_nome);
            printf("Produto:    %s\n", o[i].produto.nome);
            printf("Quantidade: %d\n", o[i].qtd);
            printf("Total:      R$ %.2f\n", o[i].total);
            printf("--------------------------\n");
            return;
        }
    }
    printf("Orçamento não encontrado para o CPF %s.\n", cpf);
}

void editarOrcamento(Orcamento o[], int totalOrcamentos,  Cliente clientes[], int totalClientes, Produto produtos[], int totalProdutos) {
    cls();
    int id;
    printf("3 - Editar Orçamento\n");
    printf("ID do orçamento a editar: ");
    scanf("%d", &id);

    for (int i = 0; i < totalOrcamentos; i++) {
        if (o[i].id == id) {
            int novaQtd;
            printf("Quantidade atual: %d\n", o[i].qtd);
            printf("Nova quantidade: ");
            scanf("%d", &novaQtd);

            // Encontra o produto vinculado no estoque
            for (int j = 0; j < totalProdutos; j++) {

                if (produtos[j].id == o[i].id_produto) {
                    // Devolve a quantidade antiga
                    produtos[j].quantidade += o[i].qtd;

                    // Verifica se há estoque suficiente para a nova quantidade
                    if (novaQtd > produtos[j].quantidade) {
                        printf("Estoque insuficiente. Quantidade disponível: %d\n", produtos[j].quantidade);
                        // Reverte o estoque à quantidade anterior
                        produtos[j].quantidade -= o[i].qtd;
                        return;
                    }

                    // Atualiza estoque e orçamento
                    produtos[j].quantidade -= novaQtd;
                    o[i].qtd = novaQtd;
                    o[i].total = novaQtd * produtos[j].precovenda;
                    o[i].produto = produtos[j]; // Atualiza informações do produto no orçamento

                    //pega o nome atualizado do cliente
                    strcpy(o[i].cliente.c_nome, getNomeCliente(clientes, totalClientes, o[i].cliente.cpf));
                    //pega o nome atualizado do produto
                    strcpy(o[i].produto.nome, getNomeProduto(produtos, totalProdutos, o[i].id_produto));

                    printf("Orçamento atualizado com sucesso:\n");
                    printf("\n----- ORÇAMENTO #%d -----\n", o[i].id);
                    printf("Cliente:        %s\n", o[i].cliente.c_nome);
                    printf("Produto:        %s\n", o[i].produto.nome);
                    printf("Valor unitário: R$ %.2f\n", o[i].produto.precovenda);
                    printf("Quantidade:     %d\n", o[i].qtd);
                    printf("Total:          R$ %.2f\n", o[i].total);
                    printf("--------------------------\n");
                    return;
                }
            }

            printf("Produto vinculado ao orçamento não encontrado.\n");
            return;
        }
    }

    printf("Orçamento com ID %d não encontrado.\n", id);
}

void excluirOrcamento(Orcamento o[], int *total, Produto produtos[], int totalProdutos) {
    cls();
    int id;
    printf("4 - Excluir Orçamento\n");
    printf("ID do orçamento a excluir: ");
    scanf("%d", &id);

    for (int i = 0; i < *total; i++) {
        if (o[i].id == id) {
            // Devolve a quantidade ao estoque do produto correspondente
            for (int j = 0; j < totalProdutos; j++) {
                if (produtos[j].id == o[i].id_produto) {
                    produtos[j].quantidade += o[i].qtd;
                    break;
                }
            }

            // Remove orçamento do array
            for (int j = i; j < *total - 1; j++) {
                o[j] = o[j + 1];
            }
            (*total)--;
            printf("Orçamento excluído e produto devolvido ao estoque.\n");
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

void cadastrarCliente(Cliente c[], int *n) {
    cls();
    printf("1 - Adicionar Cliente\n");
    if (*n >= MAX_CLIENTES) {
        printf("\nErro: Os cadastros atingiram sua capacidade máxima!\n");
        return;
    }
    Cliente novo;
    printf("CPF: ");
    lerCpf(novo.cpf);

    // Verifica se já existe um cliente com o mesmo CPF
    for (int i = 0; i < *n; i++) {
        if (strcmp(c[i].cpf, novo.cpf) == 0) {
            printf("Erro: Já existe um cliente com este CPF.\n");
            return;
        }
    }
    lerCampo(novo.c_nome, "Nome");

    printf("Telefone: ");
    scanf(" %9[^\n]", novo.tel);

    printf("Endereço: ");
    scanf(" %99[^\n]", novo.end);

    lerCampo(novo.proj, "Projeto");

    c[*n] = novo;
    (*n)++;
    printf("Cliente cadastrado com sucesso:\n");
    printf("\n--------------------------\n");
    printf("CPF:      %s\n", novo.cpf);
    printf("Nome:     %s\n", novo.c_nome);
    printf("Telefone: %s\n", novo.tel);
    printf("Endereço: %s\n", novo.end);
    printf("Projeto:  %s\n", novo.proj);
    printf("--------------------------\n");
}

void pesquisarCliente(Cliente c[], int n) {
    char cpf[15];
    cls();
    printf("2 - Pesquisar Cliente\n");

    printf("CPF para pesquisar: ");
    lerCpf(cpf);
    for (int i = 0; i < n; i++) {
        if (strcmp(c[i].cpf, cpf) == 0) {
            printf("\nCliente Encontrado: \n");
            printf("\n--------------------------\n");
            printf("CPF:      %s\n", c[i].cpf);
            printf("Nome:     %s\n", c[i].c_nome);
            printf("Telefone: %s\n", c[i].tel);
            printf("Endereço: %s\n", c[i].end);
            printf("Projeto:  %s\n", c[i].proj);
            printf("--------------------------\n");
            return;
        }
    }
    printf("Cliente não encontrado.\n");
}

void editarCliente(Cliente c[], int n) {
    char cpf[15];

    cls();
    printf("3 - Editar Cliente\n");

    printf("CPF do cliente para editar: ");
    lerCpf(cpf);
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

void excluirCliente(Cliente c[], int *n, Orcamento o[], int totalOrcamentos) {
    char cpf[15];

    cls();
    printf("4 - Excluir Cliente\n");

    printf("CPF do cliente para excluir: ");
    lerCpf(cpf);

     for (int i = 0; i < totalOrcamentos; i++) {
        if (strcmp(o[i].cliente.cpf, cpf) == 0) {
            printf("Erro: Cliente possui orçamentos vinculados e não pode ser excluído.\n");
            return;
        }
    }

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

void lerNomeProduto(char *str) {
    int erro;

    do
    {
        erro = 0;

        printf("\nNome produto: ");
        scanf(" %49[^\n]", str);
        fflush(stdin);

        if(strlen(str) > 48)
        {
            erro = 1;
            printf("ERRO: O nome pode possuir no máximo 48 caracteres!\n");
        }
        if(strlen(str) < 5) {
            erro = 1;
            printf("ERRO: O nome deve possuir pelo menos 5 caracteres!\n");
        }

    } while(erro == 1);
}
void lerCampo(char *campo, char *nomeCampo) {
    int erro;

    do
    {
        erro = 0;

        printf("\n%s: ", nomeCampo);
        scanf(" %49[^\n]", campo);
        fflush(stdin);

        if(strlen(campo) > 48)
        {
            erro = 1;
            printf("ERRO: O campo \'%s\' pode possuir no máximo 48 caracteres!\n", nomeCampo);
        }
        if(strlen(campo) < 5) {
            erro = 1;
            printf("ERRO: O campo \'%s\' deve possuir pelo menos 5 caracteres!\n", nomeCampo);
        }

    } while(erro == 1);
}
void cadastrarProduto(Produto produtos[], int *totalProdutos) {
    cls();
    printf("1 - Cadastrar Produto\n");

    if (*totalProdutos >= MAX_PRODUTOS) {
        printf("\nErro: Os produtos atingiram sua capacidade máxima!\n");
        return;
    }
    Produto p;
    p.id = *totalProdutos + 1;
    p.ativo = 1;

    lerCampo(p.nome, "Nome do Produto");
    lerCampo(p.fornecedor, "Nome do Fornecedor");

    printf("Preço de Custo: ");
    scanf("%f", &p.precocusto);

    printf("Preço de Venda: ");
    scanf("%f", &p.precovenda);
    p.margemlucro = ((p.precovenda - p.precocusto) / p.precovenda) * 100.0;

    printf("Quantidade em estoque: ");
    scanf("%d", &p.quantidade);

    lerCampo(p.localizacao, "Local de armazenamento");

    produtos[*totalProdutos] = p;
    (*totalProdutos)++;
    printf("Produto cadastrado com sucesso!\n");
    printf("\n------- Produto #%d -------\n", p.id);
    printf("Produto:     %s\n", p.nome);
    printf("Fornecedor:  %s\n", p.fornecedor);
    printf("Custo:       R$ %.2f\n", p.precocusto);
    printf("Venda:       R$ %.2f\n", p.precovenda);
    printf("Lucro:       %.2f%%\n", p.margemlucro);
    printf("Quantidade:  %d\n", p.quantidade);
    printf("Localização: %s\n", p.localizacao);
    printf("--------------------------\n");
}

void listarProdutos(Produto produtos[], int totalProdutos) {
    cls();
    printf("2 - Listar Produtos\n");
    if (totalProdutos == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }
    printf("\nLista de Produtos:\n");
    for (int i = 0; i < totalProdutos; i++) {
        Produto p = produtos[i];
        if (p.ativo) {
            printf("\n------- Produto #%d -------\n", p.id);
            printf("Produto:     %s\n", p.nome);
            printf("Fornecedor:  %s\n", p.fornecedor);
            printf("Custo:       R$ %.2f\n", p.precocusto);
            printf("Venda:       R$ %.2f\n", p.precovenda);
            printf("Lucro:       %.2f%%\n", p.margemlucro);
            printf("Quantidade:  %d\n", p.quantidade);
            printf("Localização: %s\n", p.localizacao);
            printf("--------------------------\n");
        }
    }
}
void editarProduto(Produto produtos[], int totalProdutos) {
    cls();
    printf("3 - Editar Produto\n");

    int id;
    printf("Digite o ID do produto para editar: ");
    scanf("%d", &id);

    for (int i = 0; i < totalProdutos; i++) {
        if (produtos[i].id == id && produtos[i].ativo) {

            printf("Novo preço de custo: ");
            scanf("%f", &produtos[i].precocusto);
            fflush(stdin);

            printf("Novo preço de venda: ");
            scanf("%f", &produtos[i].precovenda);
            fflush(stdin);

            // Recalcula margem de lucro
            produtos[i].margemlucro = ((produtos[i].precovenda - produtos[i].precocusto) / produtos[i].precovenda) * 100.0;

            printf("Nova quantidade em estoque: ");
            scanf("%d", &produtos[i].quantidade);
            fflush(stdin);

            printf("\nProduto editado com sucesso!\n");
            printf("\n------- Produto #%d -------\n", produtos[i].id);
            printf("Produto:     %s\n", produtos[i].nome);
            printf("Fornecedor:  %s\n", produtos[i].fornecedor);
            printf("Custo:       R$ %.2f\n", produtos[i].precocusto);
            printf("Venda:       R$ %.2f\n", produtos[i].precovenda);
            printf("Lucro:       %.2f%%\n",produtos[i].margemlucro);
            printf("Quantidade:  %d\n", produtos[i].quantidade);
            printf("Localização: %s\n", produtos[i].localizacao);
            printf("--------------------------\n");
            return;
        }
    }
    printf("Produto com ID %d não encontrado ou inativo.\n", id);
}
void excluirProduto(Produto produtos[], int totalProdutos, Orcamento o[], int totalOrcamentos) {
    int idExcluir;

    cls();
    printf("3 - Excluir Produto\n");


    printf("Digite o ID do produto que deseja excluir: ");
    scanf("%d", &idExcluir);
    // Verifica se o produto está vinculado a algum orçamento
    for (int i = 0; i < totalOrcamentos; i++) {
        if (o[i].id_produto == idExcluir) {
            printf("Erro: Produto está vinculado a orçamentos e não pode ser excluído.\n");
            return;
        }
    }
    for (int i = 0; i < totalProdutos; i++) {
        if (produtos[i].id == idExcluir && produtos[i].ativo) {
            produtos[i].ativo = 0;
            printf("Produto ID %d excluído com sucesso.\n", idExcluir);
            return;
        }
    }
    printf("Produto com ID %d não encontrado ou já excluído.\n", idExcluir);
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

    Orcamento orcamentos[MAX_ORCAMENTOS];
    int totalOrcamentos = 0;
    carregarOrcamentos(orcamentos, &totalOrcamentos);


    //printf("=== Sistema de Login ===\nUsuário: ");
    //scanf("%s", usuario);
    ///printf("Senha: ");
   // scanf("%s", senha);

    //if (strcmp(usuario, usuario_adm) != 0 || strcmp(senha, senha_adm) != 0) {
    //        printf("Usuário ou senha incorretos.\n");
    //return 0;
    //}

    printf("\nLogin concluído! Bem-vindo ao sistema ZenCODE, %s.\n", "ADMIN");

    int opcao;
    do {

        printf("\n--- Menu Principal ---\n1. Gerenciar Clientes\n2. Gerenciar Produtos\n3. Gerenciar Orçamentos\n0. Sair\nEscolha uma opção: ");
        scanf("%d", &opcao);
        system("cls");
        switch (opcao) {
            case 1: {
                int opCliente;
                do {

                    printf("\n--- Menu Clientes ---\n1. Adicionar Cliente\n2. Pesquisar Cliente\n3. Editar Cliente\n4. Excluir Cliente\n0. Voltar\nEscolha uma opção: ");
                    scanf("%d", &opCliente);

                    switch (opCliente) {
                        case 1: cadastrarCliente(clientes, &totalClientes); break;
                        case 2: pesquisarCliente(clientes, totalClientes); break;
                        case 3: editarCliente(clientes, totalClientes); break;
                        case 4: excluirCliente(clientes, &totalClientes, orcamentos, totalOrcamentos); break;
                        case 0: cls(); break;
                    }
                } while (opCliente != 0);
                break;
            }
            case 2: {
                int opProduto;
                do {

                    printf("\n--- Menu Produtos ---\n1. Cadastrar Produto\n2. Listar Produtos\n3. Editar Produto\n4. Excluir Produto\n0. Voltar\nEscolha uma opção: ");
                    scanf("%d", &opProduto);
                    switch (opProduto) {
                        case 1: cadastrarProduto(produtos, &totalProdutos); break;
                        case 2: listarProdutos(produtos, totalProdutos); break;
                        case 3: editarProduto(produtos, totalProdutos); break;
                        case 4: excluirProduto(produtos, totalProdutos, orcamentos, totalOrcamentos); break;
                        case 0: cls(); break;
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
                        case 1: criarOrcamento(orcamentos, &totalOrcamentos, clientes, totalClientes, produtos, totalProdutos); break;
                        case 2: pesquisarOrcamento(orcamentos, totalOrcamentos, clientes, totalClientes, produtos, totalProdutos);  break;
                        case 3: editarOrcamento(orcamentos, totalOrcamentos, clientes, totalClientes, produtos, totalProdutos);  break;
                        case 4: excluirOrcamento(orcamentos, &totalOrcamentos, produtos, totalProdutos); break;
                        case 0: cls(); break;
                    }
                } while (opOrcamento != 0);
                break;
            default:
                printf("\n Opção inválida!!");
                break;
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

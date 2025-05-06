#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 10  // Tamanho da tabela hash

// --------------------- Estruturas -----------------------

typedef struct {
    int dia, mes, ano;
} Data;

typedef struct {
    char nome[50];
    int cpf;
    Data dataNasc;
    char historico[300];
} Prontuario;

// Estrutura de nó para lista encadeada (tratamento de colisoes)
typedef struct no {
    Prontuario p;
    struct no* proximo;
} No;

// Tabela hash com listas encadeadas para cada posicao
No* tabela[TAM];

// --------------------- Funcoes Auxiliares -----------------------

// Imprime uma data no formato dd/mm/aaaa
void imprimirData(Data d) {
    printf("%02d/%02d/%04d\n", d.dia, d.mes, d.ano);
}

// Imprime os dados de um prontuario
void imprimirProntuario(Prontuario p) {
    printf("\nNome: %s", p.nome);
    printf("CPF: %d\n", p.cpf);
    printf("Data de Nascimento: ");
    imprimirData(p.dataNasc);
    printf("Historico Medico: %s\n", p.historico);
}

// Le uma data do usuario
Data lerData() {
    Data d;
    printf("Digite a data (dd mm aaaa): ");
    scanf_s("%d%d%d", &d.dia, &d.mes, &d.ano);
    getchar();
    return d;
}

// Le os dados de um prontuario do usuario
Prontuario lerProntuario() {
    Prontuario p;
    printf("\n\n--- Cadastro de Paciente ---\n");
    printf("Nome: ");
    fgets(p.nome, 50, stdin);
    printf("CPF: ");
    scanf_s("%d", &p.cpf);
    getchar();
    printf("Data de Nascimento:\n");
    p.dataNasc = lerData();
    printf("Historico Medico (resumo): ");
    fgets(p.historico, 300, stdin);
    return p;
}

// --------------------- Tabela Hash com Lista Encadeada -----------------------

// Funcao hash baseada no CPF
int funcaoHash(int cpf) {
    return cpf % TAM;
}

// Inicializa a tabela com NULL
void inicializarTabela() {
    for (int i = 0; i < TAM; i++) {
        tabela[i] = NULL;
    }
}

// Insere um prontuario na tabela hash
void inserir() {
    Prontuario p = lerProntuario();
    int id = funcaoHash(p.cpf);

    // Verifica se ja existe um prontuario com o mesmo CPF
    No* atual = tabela[id];
    while (atual != NULL) {
        if (atual->p.cpf == p.cpf) {
            printf("\nEsse CPF ja existe. Deseja adicionar outro prontuario para o mesmo CPF? (s/n): ");
            char resposta;
            scanf_s(" %c", &resposta, 1);
            getchar();
            if (resposta != 's' && resposta != 'S') {
                printf("Insercao cancelada.\n");
                return;
            }
            break;
        }
        atual = atual->proximo;
    }

    // Cria novo no da lista encadeada
    No* novo = (No*)malloc(sizeof(No));
    if (novo == NULL) {
        printf("Erro ao alocar memoria!\n");
        return;
    }

    // Insere no inicio da lista
    novo->p = p;
    novo->proximo = tabela[id];
    tabela[id] = novo;

    printf("\nProntuario inserido na posicao %d da tabela (encadeado).\n", id);
}


// Busca e imprime todos os prontuarios com o CPF informado
void buscarTodos(int cpf) {
    int id = funcaoHash(cpf);
    No* atual = tabela[id];
    int encontrados = 0;

    while (atual != NULL) {
        if (atual->p.cpf == cpf) {
            if (encontrados == 0) {
                printf("\n--- Prontuarios Encontrados ---\n");
            }
            imprimirProntuario(atual->p);
            printf("----------------------\n");
            encontrados++;
        }
        atual = atual->proximo;
    }

    if (encontrados == 0) {
        printf("\nNenhum prontuario encontrado com o CPF informado.\n");
    }
}

void remover(int cpf) {
    int id = funcaoHash(cpf);
    No* atual = tabela[id];
    No* anterior = NULL;
    int removidos = 0;

    while (atual != NULL) {
        if (atual->p.cpf == cpf) {
            No* temp = atual;

            if (anterior == NULL) {
                tabela[id] = atual->proximo;  // remove primeiro da lista
            }
            else {
                anterior->proximo = atual->proximo;  // remove no meio ou fim
            }

            atual = atual->proximo;
            free(temp);
            removidos++;
        }
        else {
            anterior = atual;
            atual = atual->proximo;
        }
    }

    if (removidos > 0) {
        printf("\n%d prontuario(s) com CPF %d removido(s) da tabela.\n", removidos, cpf);
    }
    else {
        printf("\nNenhum prontuario encontrado com o CPF informado.\n");
    }
}


// Imprime todos os prontuarios da tabela
void imprimirTabela() {
    printf("\n--- Todos os Prontuarios ---\n");
    for (int i = 0; i < TAM; i++) {
        printf("\n[%d]:\n", i);
        No* atual = tabela[i];
        while (atual != NULL) {
            imprimirProntuario(atual->p);
            atual = atual->proximo;
            printf("----------------------\n");
        }
    }
}

// --------------------- Menu Principal -----------------------

int main() {
    int opcao, cpf;
    Prontuario* resultado;

    inicializarTabela();

    do {
        // Menu principal do sistema
        printf("\n\n--- MENU ---\n");
        printf("1 - Cadastrar paciente\n");
        printf("2 - Buscar por CPF\n");
        printf("3 - Listar todos os prontuarios\n");
        printf("4 - Remover prontuario por CPF\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf_s("%d", &opcao);
        getchar();

        switch (opcao) {
        case 1:
            inserir();
            break;
        case 2:
            printf("Digite o CPF do paciente: ");
            scanf_s("%d", &cpf);
            getchar();
            buscarTodos(cpf);
            break;
        case 3:
            imprimirTabela();
            break;
        case 4:
            printf("\nDigite o CPF do paciente a ser removido: ");
            scanf_s("%d", &cpf);
            getchar();
            remover(cpf);
            break;
        case 0:
            printf("Encerrando...\n");
            break;
        default:
            printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 10

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

// Lista encadeada para tratar colisões:
// Cada posição da tabela pode ter vários nós encadeados
typedef struct no {
    Prontuario p;
    struct no* proximo;
} No;

// A tabela hash agora é um vetor de ponteiros para listas:
No* tabela[TAM];

// --------------------- Funções Auxiliares -----------------------

void imprimirData(Data d) {
    printf("%02d/%02d/%04d\n", d.dia, d.mes, d.ano);
}

void imprimirProntuario(Prontuario p) {
    printf("\nNome: %s", p.nome);
    printf("CPF: %d\n", p.cpf);
    printf("Data de Nascimento: ");
    imprimirData(p.dataNasc);
    printf("Historico Medico: %s\n", p.historico);
}

Data lerData() {
    Data d;
    printf("Digite a data (dd mm aaaa): ");
    scanf_s("%d%d%d", &d.dia, &d.mes, &d.ano);
    getchar();
    return d;
}

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
    printf("Histórico Medico (resumo): ");
    fgets(p.historico, 300, stdin);
    return p;
}

// --------------------- Tabela Hash com Lista Encadeada -----------------------

int funcaoHash(int cpf) {
    return cpf % TAM;
}

void inicializarTabela() {
    for (int i = 0; i < TAM; i++) {
        tabela[i] = NULL;
    }
}

void inserir() {
    Prontuario p = lerProntuario();
    int id = funcaoHash(p.cpf);

    // Criar novo nó da lista encadeada:
    No* novo = (No*)malloc(sizeof(No));
    if (novo == NULL) {
        printf("Erro ao alocar memoria!\n");
        return;
    }

    novo->p = p;
    novo->proximo = tabela[id];  // Insere no início da lista da posição id
    tabela[id] = novo;

    printf("\nProntuario inserido na posicao %d da tabela (encadeado).\n", id);
}

Prontuario* buscar(int cpf) {
    int id = funcaoHash(cpf);
    No* atual = tabela[id];

    // Percorre a lista encadeada da posição hash
    while (atual != NULL) {
        if (atual->p.cpf == cpf) {
            return &atual->p;
        }
        atual = atual->proximo;
    }

    return NULL;
}

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
        printf("\n\n--- MENU ---\n");
        printf("1 - Cadastrar paciente\n");
        printf("2 - Buscar por CPF\n");
        printf("3 - Listar todos os prontuarios\n");
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
            resultado = buscar(cpf);
            if (resultado != NULL) {
                printf("\n--- Prontuario Encontrado ---\n");
                imprimirProntuario(*resultado);
            }
            else {
                printf("Prontuario nao encontrado!\n");
            }
            break;
        case 3:
            imprimirTabela();
            break;
        case 0:
            printf("Encerrando...\n");
            break;
        default:
            printf("Opção invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}

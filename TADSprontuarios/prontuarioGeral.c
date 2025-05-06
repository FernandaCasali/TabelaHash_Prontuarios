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

// Lista encadeada para tratar colisoes:
// Cada posicao da tabela pode ter varios nos encadeados
typedef struct no {
    Prontuario p;
    struct no* proximo;
} No;

// A tabela hash agora tem um vetor de ponteiros para listas:
No* tabela[TAM];

// --------------------- Funcoes Auxiliares -----------------------

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
    printf("Historico Medico (resumo): ");
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

    // Verifica se já existe prontuário com esse CPF
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

    // Criar novo nó da lista encadeada:
    No* novo = (No*)malloc(sizeof(No));
    if (novo == NULL) {
        printf("Erro ao alocar memoria!\n");
        return;
    }

    novo->p = p;
    novo->proximo = tabela[id];  // Insere no início da lista
    tabela[id] = novo;

    printf("\nProntuario inserido na posicao %d da tabela (encadeado).\n", id);
}


void buscarTodos(int cpf) {
    int id = funcaoHash(cpf);
    No* atual = tabela[id];
    int encontrados = 0;

    // Percorre a lista encadeada e imprime todos os prontuários com o mesmo CPF
    while (atual != NULL) {
        if (atual->p.cpf == cpf) {
            if (encontrados == 0) {
                printf("\n--- Prontuários Encontrados ---\n");
            }
            imprimirProntuario(atual->p);
            printf("----------------------\n");
            encontrados++;
        }
        atual = atual->proximo;
    }

    if (encontrados == 0) {
        printf("Nenhum prontuário encontrado com o CPF informado.\n");
    }
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
            buscarTodos(cpf);
            break;
        case 3:
            imprimirTabela();
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
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

typedef struct no {
    Prontuario p;
    struct no* proximo;
} No;

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

// --------------------- Tabela Hash -----------------------

int funcaoHash(int cpf) {
    return cpf % TAM;
}

void inicializarTabela() {
    for (int i = 0; i < TAM; i++) {
        tabela[i] = NULL;
    }
}

void atualizar(int cpf) {
    int id = funcaoHash(cpf);
    No* atual = tabela[id];

    while (atual != NULL) {
        if (atual->p.cpf == cpf) {
            printf("\n--- Atualizar Prontuario ---\n");

            printf("Nome atual: %s", atual->p.nome);
            printf("Deseja alterar o nome? (s/n): ");
            char opcao;
            scanf_s(" %c", &opcao, 1);
            getchar();
            if (opcao == 's' || opcao == 'S') {
                printf("Novo nome: ");
                fgets(atual->p.nome, 50, stdin);
            }

            printf("Data de nascimento atual: ");
            imprimirData(atual->p.dataNasc);
            printf("Deseja alterar a data de nascimento? (s/n): ");
            scanf_s(" %c", &opcao, 1);
            getchar();
            if (opcao == 's' || opcao == 'S') {
                atual->p.dataNasc = lerData();
            }

            printf("Historico atual: %s", atual->p.historico);
            printf("Deseja alterar o historico medico? (s/n): ");
            scanf_s(" %c", &opcao, 1);
            getchar();
            if (opcao == 's' || opcao == 'S') {
                printf("Novo historico: ");
                fgets(atual->p.historico, 300, stdin);
            }

            printf("\nProntuario atualizado com sucesso!\n");
            return;
        }
        atual = atual->proximo;
    }

    printf("\nNenhum prontuario encontrado com o CPF informado.\n");
}

void inserir() {
    int cpf;
    printf("\nDigite o CPF do paciente: ");
    scanf_s("%d", &cpf);
    getchar();

    int id = funcaoHash(cpf);
    No* atual = tabela[id];

    while (atual != NULL) {
        if (atual->p.cpf == cpf) {
            printf("\nJá existe um prontuario com esse CPF.\n");
            printf("Deseja atualizar o prontuario existente? (s/n): ");
            char escolha;
            scanf_s(" %c", &escolha, 1);
            getchar();

            if (escolha == 's' || escolha == 'S') {
                atualizar(cpf);
            }
            else {
                printf("Insercao cancelada.\n");
            }
            return;
        }
        atual = atual->proximo;
    }

    // Criar novo prontuário
    Prontuario p;
    p.cpf = cpf;

    printf("Nome: ");
    fgets(p.nome, 50, stdin);

    printf("Data de Nascimento:\n");
    p.dataNasc = lerData();

    printf("Histórico Medico (resumo): ");
    fgets(p.historico, 300, stdin);

    No* novo = (No*)malloc(sizeof(No));
    if (novo == NULL) {
        printf("Erro ao alocar memoria!\n");
        return;
    }

    novo->p = p;
    novo->proximo = tabela[id];
    tabela[id] = novo;

    printf("\n Prontuario inserido com sucesso na posicao %d da tabela.\n", id);
}

// Busca todos os prontuarios com o mesmo CPF
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
                tabela[id] = atual->proximo;
            }
            else {
                anterior->proximo = atual->proximo;
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

    inicializarTabela();

    do {
        printf("\n\n--- MENU ---\n");
        printf("1 - Cadastrar paciente\n");
        printf("2 - Buscar por CPF\n");
        printf("3 - Listar todos os prontuarios\n");
        printf("4 - Remover prontuario por CPF\n");
        printf("5 - Atualizar prontuario por CPF\n");
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
        case 5:
            printf("Digite o CPF do paciente a ser atualizado: ");
            scanf_s("%d", &cpf);
            getchar();
            atualizar(cpf);
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

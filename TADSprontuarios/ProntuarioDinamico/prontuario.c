#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prontuario.h"

No* tabela[TAM];

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
            scanf_s(" %c", &opcao);
            getchar();
            if (opcao == 's' || opcao == 'S') {
                printf("Novo nome: ");
                fgets(atual->p.nome, 50, stdin);
            }

            printf("Data de nascimento atual: ");
            imprimirData(atual->p.dataNasc);
            printf("Deseja alterar a data de nascimento? (s/n): ");
            scanf_s(" %c", &opcao);
            getchar();
            if (opcao == 's' || opcao == 'S') {
                atual->p.dataNasc = lerData();
            }

            printf("Historico atual: %s", atual->p.historico);
            printf("Deseja alterar o historico medico? (s/n): ");
            scanf_s(" %c", &opcao);
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
    int existe = 0;

    while (atual != NULL) {
        if (atual->p.cpf == cpf) {
            existe = 1;
            break;
        }
        atual = atual->proximo;
    }

    if (existe) {
        printf("\nEsse CPF ja existe. Deseja:\n");
        printf("a - Atualizar prontuario existente\n");
        printf("b - Adicionar novo prontuario (mantendo o existente)\n");
        printf("Escolha (a/b): ");

        char escolha;
        scanf_s(" %c", &escolha);
        getchar();

        if (escolha == 'a' || escolha == 'A') {
            atualizar(cpf);
            return;
        }
        else if (escolha != 'b' && escolha != 'B') {
            printf("Insercao cancelada.\n");
            return;
        }
    }

    Prontuario p;
    p.cpf = cpf;

    printf("Nome: ");
    fgets(p.nome, 50, stdin);

    printf("Data de Nascimento:\n");
    p.dataNasc = lerData();

    printf("Historico Medico (resumo): ");
    fgets(p.historico, 300, stdin);

    No* novo = (No*)malloc(sizeof(No));
    if (novo == NULL) {
        printf("Erro ao alocar memoria!\n");
        return;
    }

    novo->p = p;
    novo->proximo = tabela[id];
    tabela[id] = novo;

    printf("\nProntuario inserido na posicao %d da tabela (encadeado).\n", id);
}

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
        printf("\n%d prontuario(s) com CPF %d removido(s).\n", removidos, cpf);
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

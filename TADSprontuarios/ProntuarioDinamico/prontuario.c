#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prontuario.h"

No* tabela[TAM];

// Imprime uma data no formato dd/mm/aaaa
void imprimirData(Data d) {
    printf("%02d/%02d/%04d\n", d.dia, d.mes, d.ano);
}

// Imprime as informacoes de um prontuario
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
    printf("(dd mm aaaa): ");
    scanf_s("%d%d%d", &d.dia, &d.mes, &d.ano);
    getchar();
    return d;
}

// Funcao hash que calcula o indice com base no CPF do prontuario
int funcaoHash(int cpf) {
    return cpf % TAM;  // A funcao hash e baseada no modulo do CPF pelo tamanho da tabela
}

// Inicializa a tabela hash com NULL, indicando que as posicoes estao vazias
void inicializarTabela() {
    for (int i = 0; i < TAM; i++) {
        tabela[i] = NULL;  // Marca todas as posicoes como NULL (tabela vazia)
    }
}

// Atualiza um prontuario existente baseado no CPF
void atualizar(int cpf) {
    int id = funcaoHash(cpf);  // Calcula o indice usando a funcao hash
    No* atual = tabela[id];  // Obtem o no na posicao correspondente na tabela

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
            return;  // Retorna apos a atualizacao
        }
        atual = atual->proximo;  // Vai para o proximo no, caso o CPF nao seja encontrado
    }

    printf("\nNenhum prontuario encontrado com o CPF informado.\n");
}

// Insere um novo prontuario na tabela hash
void inserir() {
    int cpf;
    printf("\nDigite o CPF do paciente: ");
    scanf_s("%d", &cpf);
    getchar();

    int id = funcaoHash(cpf);  // Calcula o indice usando a funcao hash
    No* atual = tabela[id];  // Obtem o no na posicao correspondente na tabela

    // Verifica se ja existe um prontuario com o mesmo CPF
    while (atual != NULL) {
        if (atual->p.cpf == cpf) {
            printf("\nJa existe um prontuario com esse CPF.\n");
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
        atual = atual->proximo;  // Vai para o proximo no
    }

    // Caso o CPF nao exista, cria um novo prontuario
    Prontuario p;
    p.cpf = cpf;

    printf("Nome: ");
    fgets(p.nome, 50, stdin);

    printf("Data de Nascimento:\n");
    p.dataNasc = lerData();

    printf("Historico Medico (resumo): ");
    fgets(p.historico, 300, stdin);

    // Cria um novo no para armazenar o prontuario
    No* novo = (No*)malloc(sizeof(No));
    if (novo == NULL) {
        printf("Erro ao alocar memoria!\n");
        return;  // Retorna caso haja erro na alocacao de memoria
    }

    novo->p = p;  // Atribui os dados do prontuario ao novo no
    novo->proximo = tabela[id];  // Aponta para o proximo no (se houver)
    tabela[id] = novo;  // Insere o novo no na tabela, na posicao calculada

    printf("\n Prontuario inserido com sucesso na posicao %d da tabela.\n", id);
}

// Busca e imprime todos os prontuarios com o mesmo CPF
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

// Remove todos os prontuarios com o CPF informado
void remover(int cpf) {
    int id = funcaoHash(cpf);
    No* atual = tabela[id];
    No* anterior = NULL;
    int removidos = 0;

    // Percorre a lista encadeada 
    while (atual != NULL) {
        if (atual->p.cpf == cpf) {
            No* temp = atual;  // Salva o no a ser removido
            if (anterior == NULL) {
                tabela[id] = atual->proximo;  // Caso seja o primeiro no da lista
            }
            else {
                anterior->proximo = atual->proximo;  // Remove o no da lista
            }
            atual = atual->proximo;  // Vai para o proximo no
            free(temp);  // Libera a memoria do no removido
            removidos++;  // Incrementa o contador de removidos
        }
        else {
            anterior = atual;
            atual = atual->proximo;  // Vai para o proximo no
        }
    }

    if (removidos > 0) {
        printf("\n%d prontuario(s) com CPF %d removido(s).\n", removidos, cpf);
    }
    else {
        printf("\nNenhum prontuario encontrado com o CPF informado.\n");
    }
}

// Imprime todos os prontuarios armazenados na tabela
void imprimirTabela() {
    printf("\n--- Todos os Prontuarios ---\n");
    for (int i = 0; i < TAM; i++) {  // Percorre a tabela de indices
        printf("\n[%d]:\n", i);
        No* atual = tabela[i];  // Obtem o no na posicao atual
        while (atual != NULL) {  // Imprime todos os prontuarios em cada lista encadeada
            imprimirProntuario(atual->p);
            atual = atual->proximo;
            printf("----------------------\n");
        }
    }
}

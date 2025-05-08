#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "prontuario.h"  // Sua implementacao da tabela hash com lista

#define NUM_TESTES 5000

void gerarNomeAleatorio(char* nome) {
    printf(nome, "Paciente_%d\n", rand() % 100000);
}

void gerarHistoricoAleatorio(char* historico) {
    printf(historico, "Historico medico gerado automaticamente.\n");
}

Data gerarDataAleatoria() {
    Data d;
    d.dia = rand() % 28 + 1;
    d.mes = rand() % 12 + 1;
    d.ano = rand() % 50 + 1950;
    return d;
}

void testeAtualizacao(int total) {
    for (int i = 0; i < total; i++) {
        int cpf = 100000000 + i;
        // Simula atualizacao silenciosa, sem interacao com o usuario:
        int id = funcaoHash(cpf);
        No* atual = tabela[id];
        while (atual != NULL) {
            if (atual->p.cpf == cpf) {
                // Atualiza alguns campos diretamente
                gerarNomeAleatorio(atual->p.nome);
                atual->p.dataNasc = gerarDataAleatoria();
                gerarHistoricoAleatorio(atual->p.historico);
                break;
            }
            atual = atual->proximo;
        }
    }
}


void testeInsercao(int total) {
    for (int i = 0; i < total; i++) {
        Prontuario p;
        p.cpf = 100000000 + i;
        gerarNomeAleatorio(p.nome);
        p.dataNasc = gerarDataAleatoria();
        gerarHistoricoAleatorio(p.historico);

        int id = funcaoHash(p.cpf);
        No* novo = (No*)malloc(sizeof(No));
        novo->p = p;
        novo->proximo = tabela[id];
        tabela[id] = novo;
    }
}

void testeBusca(int total) {
    for (int i = 0; i < total; i++) {
        int cpf = 100000000 + i;
        buscarTodos(cpf);
    }
}

void testeRemocao(int total) {
    for (int i = 0; i < total; i++) {
        int cpf = 100000000 + i;
        remover(cpf);
    }
}

int main() {
    srand(time(NULL));
    inicializarTabela();

    clock_t t_inicio, t_fim;
    double tempo_insercao, tempo_busca, tempo_remocao, tempo_total;

    t_inicio = clock();

    // Insercao
    clock_t t0 = clock();
    testeInsercao(NUM_TESTES);
    clock_t t1 = clock();
    tempo_insercao = (double)(t1 - t0) / CLOCKS_PER_SEC;

    // Busca
    t0 = clock();
    testeBusca(NUM_TESTES);
    t1 = clock();
    tempo_busca = (double)(t1 - t0) / CLOCKS_PER_SEC;

    double tempo_atualizacao;
    // Atualizacao
    t0 = clock();
    testeAtualizacao(NUM_TESTES);
    t1 = clock();
    tempo_atualizacao = (double)(t1 - t0) / CLOCKS_PER_SEC;


    // Remocao
    t0 = clock();
    testeRemocao(NUM_TESTES);
    t1 = clock();
    tempo_remocao = (double)(t1 - t0) / CLOCKS_PER_SEC;

    t_fim = clock();
    tempo_total = (double)(t_fim - t_inicio) / CLOCKS_PER_SEC;

    printf("\n--- Resultados do Benchmark ---\n");
    printf("Tempo de insercao: %.6f segundos\n", tempo_insercao);
    printf("Tempo de busca:    %.6f segundos\n", tempo_busca);
    printf("Tempo de atualizacao: %.6f segundos\n", tempo_atualizacao);
    printf("Tempo de remocao:  %.6f segundos\n", tempo_remocao);
    printf("Tempo total:       %.6f segundos\n", tempo_total);

    return 0;
}

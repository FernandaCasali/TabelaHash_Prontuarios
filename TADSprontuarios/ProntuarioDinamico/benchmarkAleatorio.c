#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "prontuario.h"  // Sua TAD com a hash encadeada

#define NUM_TESTES 100  // Você pode ajustar aqui

void gerarNomeSequencial(char* nome, int numero) {
    printf("Paciente %d\n", numero);;
}

void gerarHistoricoAleatorio(char* historico) {
    printf(historico, "Historico medico gerado automaticamente.");
}

Data gerarDataAleatoria() {
    Data d;
    d.dia = rand() % 28 + 1;
    d.mes = rand() % 12 + 1;
    d.ano = rand() % 50 + 1950;
    return d;
}

int gerarCpfAleatorio() {
    return 10000 + rand() % 90000;  // Gera CPF fictício de 5 dígitos
}

void testeInsercao(int total, int* cpfs) {
    for (int i = 0; i < total; i++) {
        Prontuario p;
        p.cpf = cpfs[i];
        gerarNomeSequencial(p.nome, i + 1);
        p.dataNasc = gerarDataAleatoria();
        gerarHistoricoAleatorio(p.historico);

        int id = funcaoHash(p.cpf);
        No* novo = (No*)malloc(sizeof(No));
        novo->p = p;
        novo->proximo = tabela[id];
        tabela[id] = novo;
    }
}

void testeBusca(int total, int* cpfs) {
    for (int i = 0; i < total; i++) {
        buscarTodos(cpfs[i]);
    }
}

void testeAtualizacao(int total, int* cpfs) {
    for (int i = 0; i < total; i++) {
        int id = funcaoHash(cpfs[i]);
        No* atual = tabela[id];
        while (atual != NULL) {
            if (atual->p.cpf == cpfs[i]) {
                gerarNomeSequencial(atual->p.nome, i + 1);
                atual->p.dataNasc = gerarDataAleatoria();
                gerarHistoricoAleatorio(atual->p.historico);
                break;
            }
            atual = atual->proximo;
        }
    }
}

void testeRemocao(int total, int* cpfs) {
    for (int i = 0; i < total; i++) {
        remover(cpfs[i]);
    }
}

double medirTempo(void (*funcao)(int, int*), int total, int* cpfs) {
    LARGE_INTEGER inicio, fim, freq;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&inicio);

    funcao(total, cpfs);

    QueryPerformanceCounter(&fim);
    return (double)(fim.QuadPart - inicio.QuadPart) / freq.QuadPart;
}

int main() {
    srand(time(NULL));
    inicializarTabela();

    // Gerar CPFs aleatórios
    int* cpfs = malloc(NUM_TESTES * sizeof(int));
    for (int i = 0; i < NUM_TESTES; i++) {
        cpfs[i] = gerarCpfAleatorio();
    }

    printf("Iniciando benchmark...\n");

    double tempo_insercao = medirTempo(testeInsercao, NUM_TESTES, cpfs);
    double tempo_busca = medirTempo(testeBusca, NUM_TESTES, cpfs);
    double tempo_atualizacao = medirTempo(testeAtualizacao, NUM_TESTES, cpfs);
    double tempo_remocao = medirTempo(testeRemocao, NUM_TESTES, cpfs);

    double tempo_total = tempo_insercao + tempo_busca + tempo_atualizacao + tempo_remocao;

    printf("\n--- Resultados do Benchmark ---\n");
    printf("Tempo de insercao:    %.6f segundos\n", tempo_insercao);
    printf("Tempo de busca:       %.6f segundos\n", tempo_busca);
    printf("Tempo de atualizacao: %.6f segundos\n", tempo_atualizacao);
    printf("Tempo de remocao:     %.6f segundos\n", tempo_remocao);
    printf("Tempo total:          %.6f segundos\n", tempo_total);

    free(cpfs);
    return 0;
}

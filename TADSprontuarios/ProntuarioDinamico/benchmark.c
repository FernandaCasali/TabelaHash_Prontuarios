#include <windows.h>  // Para alta precisão de tempo
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "prontuario.h"  // Sua TAD de tabela hash com lista encadeada

#define NUM_TESTES 100

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

void testeInsercao(int total) {
    for (int i = 0; i < total; i++) {
        Prontuario p;
        p.cpf = 100000000 + i;
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

void testeBusca(int total) {
    for (int i = 0; i < total; i++) {
        int cpf = 100000000 + i;
        buscarTodos(cpf);
    }
}

void testeAtualizacao(int total) {
    for (int i = 0; i < total; i++) {
        int cpf = 100000000 + i;
        int id = funcaoHash(cpf);
        No* atual = tabela[id];
        while (atual != NULL) {
            if (atual->p.cpf == cpf) {
                gerarNomeSequencial(atual->p.nome, i + 1);
                atual->p.dataNasc = gerarDataAleatoria();
                gerarHistoricoAleatorio(atual->p.historico);
                break;
            }
            atual = atual->proximo;
        }
    }
}

void testeRemocao(int total) {
    for (int i = 0; i < total; i++) {
        int cpf = 100000000 + i;
        remover(cpf);
    }
}

double medirTempo(void (*funcao)(int), int total) {
    LARGE_INTEGER inicio, fim, freq;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&inicio);

    funcao(total);

    QueryPerformanceCounter(&fim);

    return (double)(fim.QuadPart - inicio.QuadPart) / freq.QuadPart;
}

int main() {
    srand(time(NULL));
    inicializarTabela();

    printf("Iniciando benchmark...\n");

    printf("Inserindo %d elementos...\n", NUM_TESTES);
    double tempo_insercao = medirTempo(testeInsercao, NUM_TESTES);
    printf("Insercao concluída.\n");

    printf("Buscando %d elementos...\n", NUM_TESTES);
    double tempo_busca = medirTempo(testeBusca, NUM_TESTES);
    printf("Busca concluída.\n");

    printf("Atualizando %d elementos...\n", NUM_TESTES);
    double tempo_atualizacao = medirTempo(testeAtualizacao, NUM_TESTES);
    printf("Atualizacao concluída.\n");

    printf("Removendo %d elementos...\n", NUM_TESTES);
    double tempo_remocao = medirTempo(testeRemocao, NUM_TESTES);
    printf("Remocao concluída.\n");

    double tempo_total = tempo_insercao + tempo_busca + tempo_atualizacao + tempo_remocao;

    printf("\n--- Resultados do Benchmark ---\n");
    printf("Tempo de insercao:    %.6f segundos\n", tempo_insercao);
    printf("Tempo de busca:       %.6f segundos\n", tempo_busca);
    printf("Tempo de atualizacao: %.6f segundos\n", tempo_atualizacao);
    printf("Tempo de remocao:     %.6f segundos\n", tempo_remocao);
    printf("Tempo total:          %.6f segundos\n", tempo_total);

    return 0;
}

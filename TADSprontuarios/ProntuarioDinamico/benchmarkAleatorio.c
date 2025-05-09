#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "prontuario.h"

#define NUM_TESTES 1000000

int cpfs_testes[NUM_TESTES];
int indices[NUM_TESTES];

// Funções auxiliares
void gerarNomeSequencial(char* nome, int numero) {
    strcpy_s(nome, 50, "Paciente ");
    char numeroStr[12];
    sprintf_s(numeroStr, 12, "%d", numero);
    strcat_s(nome, 50, numeroStr);
}

void gerarHistoricoAleatorio(char* historico) {
    strcpy_s(historico, 300, "Historico medico gerado automaticamente.");
}

Data gerarDataAleatoria() {
    Data d;
    d.dia = rand() % 28 + 1;
    d.mes = rand() % 12 + 1;
    d.ano = rand() % 50 + 1950;
    return d;
}

Prontuario gerarProntuario(int numero) {
    Prontuario p;
    p.cpf = 10000 + numero;
    cpfs_testes[numero] = p.cpf;
    gerarNomeSequencial(p.nome, numero);
    p.dataNasc = gerarDataAleatoria();
    gerarHistoricoAleatorio(p.historico);
    return p;
}

void embaralharIndices(int* vetor, int tamanho) {
    for (int i = tamanho - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = vetor[i];
        vetor[i] = vetor[j];
        vetor[j] = temp;
    }
}

// Versão benchmark
void inserir_benchmark(Prontuario p) {
    int id = funcaoHash(p.cpf);
    No* atual = tabela[id];

    while (atual != NULL) {
        if (atual->p.cpf == p.cpf) {
            atual->p = p;
            return;
        }
        atual = atual->proximo;
    }

    No* novo = (No*)malloc(sizeof(No));
    novo->p = p;
    novo->proximo = tabela[id];
    tabela[id] = novo;
}

Prontuario* buscar_benchmark(int cpf) {
    int id = funcaoHash(cpf);
    No* atual = tabela[id];
    while (atual != NULL) {
        if (atual->p.cpf == cpf) {
            return &atual->p;
        }
        atual = atual->proximo;
    }
    return NULL;
}

void remover_benchmark(int cpf) {
    int id = funcaoHash(cpf);
    No* atual = tabela[id];
    No* anterior = NULL;

    while (atual != NULL) {
        if (atual->p.cpf == cpf) {
            if (anterior == NULL) {
                tabela[id] = atual->proximo;
            }
            else {
                anterior->proximo = atual->proximo;
            }
            free(atual);
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }
}

// Testes embaralhados
void testeInsercao() {
    embaralharIndices(indices, NUM_TESTES);
    for (int i = 0; i < NUM_TESTES; i++) {
        int idx = indices[i];
        Prontuario p = gerarProntuario(idx);
        inserir_benchmark(p);
    }
}

void testeBusca() {
    embaralharIndices(indices, NUM_TESTES);
    for (int i = 0; i < NUM_TESTES; i++) {
        buscar_benchmark(cpfs_testes[indices[i]]);
    }
}

void testeAtualizacao() {
    embaralharIndices(indices, NUM_TESTES);
    for (int i = 0; i < NUM_TESTES; i++) {
        Prontuario* p = buscar_benchmark(cpfs_testes[indices[i]]);
        if (p != NULL) {
            snprintf(p->historico, 300, "Historico atualizado no teste %d", i);
        }
    }
}

void testeRemocao() {
    embaralharIndices(indices, NUM_TESTES);
    for (int i = 0; i < NUM_TESTES; i++) {
        remover_benchmark(cpfs_testes[indices[i]]);
    }
}

// Medição de tempo
double medirTempo(void (*funcao)()) {
    LARGE_INTEGER inicio, fim, freq;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&inicio);

    funcao();

    QueryPerformanceCounter(&fim);
    return (double)(fim.QuadPart - inicio.QuadPart) / freq.QuadPart;
}

// Programa principal
int main() {
    srand((unsigned int)time(NULL));
    inicializarTabela();

    // Inicializa os índices
    for (int i = 0; i < NUM_TESTES; i++) {
        indices[i] = i;
    }

    printf("=== Benchmark Encadeamento Separado ===\n");
    printf("TAM: %d | Testes: %d | Fator de carga: %.2f%%\n\n", TAM, NUM_TESTES, (NUM_TESTES / (float)TAM) * 100);

    printf("1. Teste de Insercao...\n");
    double t_insercao = medirTempo(testeInsercao);

    printf("2. Teste de Busca...\n");
    double t_busca = medirTempo(testeBusca);

    printf("3. Teste de Atualizacao...\n");
    double t_atualizacao = medirTempo(testeAtualizacao);

    printf("4. Teste de Remocao...\n");
    double t_remocao = medirTempo(testeRemocao);

    printf("\n=== Resultados de %d entradas ===\n", NUM_TESTES);
    printf("Insercao:    %.6f s\n", t_insercao);
    printf("Busca:       %.6f s\n", t_busca);
    printf("Atualizacao: %.6f s\n", t_atualizacao);
    printf("Remocao:     %.6f s\n", t_remocao);
    printf("-------------------------\n");
    printf("Total:       %.6f s\n", t_insercao + t_busca + t_atualizacao + t_remocao);

    return 0;
}

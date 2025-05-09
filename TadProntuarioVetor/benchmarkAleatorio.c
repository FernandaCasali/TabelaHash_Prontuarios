#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "prontuario.h"

#define NUM_TESTES 100  // Numero de elementos para teste (deve ser menor que TAM)

int cpfs_testes[NUM_TESTES];
int indices[NUM_TESTES];  // Indices embaralhados

// Embaralha os indices com Fisher-Yates
void embaralharIndices(int* vetor, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = vetor[i];
        vetor[i] = vetor[j];
        vetor[j] = temp;
    }
}

// Gera nome sequencial com numero
void gerarNomeSequencial(char* nome, int numero) {
    strcpy_s(nome, 50, "Paciente ");
    char numeroStr[12];
    sprintf_s(numeroStr, sizeof(numeroStr), "%d", numero);
    strcat_s(nome, 50, numeroStr);
}

// Gera historico padrao
void gerarHistoricoAleatorio(char* historico) {
    strcpy_s(historico, 100, "Historico medico gerado automaticamente.");
}

// Gera data aleatoria
Data gerarDataAleatoria() {
    Data d;
    d.dia = rand() % 28 + 1;
    d.mes = rand() % 12 + 1;
    d.ano = rand() % 50 + 1950;
    return d;
}

// Gera um prontuário com base no numero
Prontuario gerarProntuario(int numero) {
    Prontuario p;
    p.cpf = 10000 + numero;
    cpfs_testes[numero] = p.cpf;
    gerarNomeSequencial(p.nome, numero);
    p.dataNas = gerarDataAleatoria();
    gerarHistoricoAleatorio(p.historico);
    return p;
}

// Insercao com sondagem linear
void inserir_benchmark(Prontuario t[], Prontuario p) {
    int id = funcaoHash(p.cpf);
    int original = id;
    int posRemovida = -1;

    while (t[id].cpf != 0) {
        if (t[id].cpf == p.cpf) {
            t[id] = p;
            return;
        }
        if (t[id].cpf == -1 && posRemovida == -1) {
            posRemovida = id;
        }
        id = (id + 1) % TAM;
        if (id == original) break;
    }

    if (posRemovida != -1) {
        t[posRemovida] = p;
    }
    else if (t[id].cpf == 0) {
        t[id] = p;
    }
}

// Remocao com marcacao
void remover_benchmark(Prontuario t[], int cpf) {
    int id = funcaoHash(cpf);
    int original = id;

    while (t[id].cpf != 0) {
        if (t[id].cpf == cpf) {
            t[id].cpf = -1;
            return;
        }
        id = (id + 1) % TAM;
        if (id == original) break;
    }
}

// Teste de insercao (ordem aleatoria)
void testeInsercao(Prontuario tabela[]) {
    for (int i = 0; i < NUM_TESTES; i++) {
        Prontuario p = gerarProntuario(indices[i]);
        inserir_benchmark(tabela, p);
    }
}

// Teste de busca (ordem aleatoria)
void testeBusca(Prontuario tabela[]) {
    for (int i = 0; i < NUM_TESTES; i++) {
        buscar(tabela, cpfs_testes[indices[i]]);
    }
}

// Teste de atualizacao (ordem aleatoria)
void testeAtualizacao(Prontuario tabela[]) {
    for (int i = 0; i < NUM_TESTES; i++) {
        Prontuario* p = buscar(tabela, cpfs_testes[indices[i]]);
        if (p != NULL) {
            snprintf(p->historico, 300, "Historico atualizado no teste %d", i);
        }
    }
}

// Teste de remocao (ordem aleatoria)
void testeRemocao(Prontuario tabela[]) {
    for (int i = 0; i < NUM_TESTES; i++) {
        remover_benchmark(tabela, cpfs_testes[indices[i]]);
    }
}

// Medicao de tempo
double medirTempo(void (*funcao)(Prontuario[]), Prontuario tabela[]) {
    LARGE_INTEGER inicio, fim, freq;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&inicio);

    funcao(tabela);

    QueryPerformanceCounter(&fim);
    return (double)(fim.QuadPart - inicio.QuadPart) / freq.QuadPart;
}

int main() {
    srand((unsigned int)time(NULL));

    // Aloca tabela
    Prontuario* tabela = (Prontuario*)calloc(TAM, sizeof(Prontuario));
    if (!tabela) {
        fprintf(stderr, "Erro ao alocar memoria para a tabela.\n");
        return 1;
    }

    inicializarTabela(tabela);

    // Prepara e embaralha os indices
    for (int i = 0; i < NUM_TESTES; i++) {
        indices[i] = i;
    }
    embaralharIndices(indices, NUM_TESTES);

    printf("=== Benchmark Automatizado ===\n");
    printf("Configuracao:\n");
    printf("- Tamanho da tabela (TAM): %d\n", TAM);
    printf("- Numero de testes: %d\n", NUM_TESTES);
    printf("- Fator de carga: %.2f%%\n\n", (NUM_TESTES / (float)TAM) * 100);

    // Testes
    printf("1. Teste de Insercao...\n");
    double tempo_insercao = medirTempo(testeInsercao, tabela);

    printf("2. Teste de Busca...\n");
    double tempo_busca = medirTempo(testeBusca, tabela);

    printf("3. Teste de Atualizacao...\n");
    double tempo_atualizacao = medirTempo(testeAtualizacao, tabela);

    printf("4. Teste de Remocao...\n");
    double tempo_remocao = medirTempo(testeRemocao, tabela);

    // Resultados
    printf("\n=== Resultados de %d entradas ===\n", NUM_TESTES);
    printf("Insercao:    %.6f seg\n", tempo_insercao);
    printf("Busca:       %.6f seg\n", tempo_busca);
    printf("Atualizacao: %.6f seg\n", tempo_atualizacao);
    printf("Remocao:     %.6f seg\n", tempo_remocao);
    printf("----------------------------------------\n");
    printf("TOTAL:       %.6f seg\n", tempo_insercao + tempo_busca + tempo_atualizacao + tempo_remocao);

    free(tabela);
    return 0;
}

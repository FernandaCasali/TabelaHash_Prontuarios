#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "prontuario.h"

#define NUM_TESTES 100  // Número de elementos para teste (deve ser menor que TAM)

// Estrutura para armazenar CPFs gerados
int cpfs_testes[NUM_TESTES];

// Funções auxiliares para gerar dados de teste
void gerarNomeSequencial(char* nome, int numero) {
    // Copia "Paciente " no início do nome
    strcpy_s(nome, 50, "Paciente ");

    // Converte o número para string manualmente
    char numeroStr[12]; // Espaço para até 10 dígitos + null
    int i = 0;

    if (numero == 0) {
        numeroStr[i++] = '0';
    }
    else {
        int temp = numero;
        while (temp > 0 && i < 11) {
            numeroStr[i++] = (temp % 10) + '0';
            temp /= 10;
        }
        // Inverter a string número
        for (int j = 0; j < i / 2; j++) {
            char c = numeroStr[j];
            numeroStr[j] = numeroStr[i - 1 - j];
            numeroStr[i - 1 - j] = c;
        }
    }
    numeroStr[i] = '\0';

    // Concatena o número convertido ao final do nome
    strcat_s(nome, 50, numeroStr);
}



void gerarHistoricoAleatorio(char* historico) {
    strcpy_s(historico, 100, "Histórico medico gerado automaticamente.");
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
    p.cpf = 10000 + numero;  // CPFs sequenciais
    cpfs_testes[numero] = p.cpf;  // Armazena para uso posterior
    char nome[50];
    gerarNomeSequencial(p.nome, numero);
    p.dataNas = gerarDataAleatoria();
    gerarHistoricoAleatorio(p.historico);
    return p;
}

// Versão modificada da função inserir para benchmark
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

// Versão modificada da função remover para benchmark
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

// Funções de teste automatizadas
void testeInsercao(Prontuario tabela[]) {
    for (int i = 0; i < NUM_TESTES; i++) {
        Prontuario p = gerarProntuario(i);
        inserir_benchmark(tabela, p);
    }
}

void testeBusca(Prontuario tabela[]) {
    for (int i = 0; i < NUM_TESTES; i++) {
        buscar(tabela, cpfs_testes[i]);
    }
}

void testeAtualizacao(Prontuario tabela[]) {
    for (int i = 0; i < NUM_TESTES; i++) {
        Prontuario* p = buscar(tabela, cpfs_testes[i]);
        if (p != NULL) {
            snprintf(p->historico, 300, "Histórico atualizado no teste %d", i);
        }
    }
}

void testeRemocao(Prontuario tabela[]) {
    for (int i = 0; i < NUM_TESTES; i++) {
        remover_benchmark(tabela, cpfs_testes[i]);
    }
}

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

    // Aloca a tabela no heap
    Prontuario* tabela = (Prontuario*)calloc(TAM, sizeof(Prontuario));
    if (tabela == NULL) {
        fprintf(stderr, "Erro ao alocar memoria para a tabela.\n");
        return 1;
    }

    inicializarTabela(tabela);

    printf("=== Benchmark Automatizado ===\n");
    printf("Configuracao:\n");
    printf("- Tamanho da tabela (TAM): %d\n", TAM);
    printf("- Numero de testes: %d\n", NUM_TESTES);
    printf("- Fator de carga: %.2f%%\n\n", (NUM_TESTES / (float)TAM) * 100);

    // Execução dos testes
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
    printf("Insercao:    %.6f ms\n", tempo_insercao);
    printf("Busca:       %.6f ms\n", tempo_busca);
    printf("Atualizacao: %.6f ms\n", tempo_atualizacao);
    printf("Remocao:     %.6f ms\n", tempo_remocao);
    printf("----------------------------------------\n");
    printf("TOTAL:       %.6f ms\n",
        (tempo_insercao + tempo_busca + tempo_atualizacao + tempo_remocao));


    // Libera a memória alocada no heap
    free(tabela);

    return 0;
}

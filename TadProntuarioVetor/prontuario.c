#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prontuario.h"

// ----------- Funcoes auxiliares -----------

// Imprime a data no formato dd/mm/aaaa
void imprimirData(Data d) {
    printf("%02d/%02d/%04d\n", d.dia, d.mes, d.ano);
}

// Imprime os dados completos de um prontuario
void imprimirProntuario(Prontuario p) {
    printf("\n\tNome: %s", p.nome);
    printf("\tCPF: %d\n", p.cpf);
    printf("\tData de nascimento: ");
    imprimirData(p.dataNas);
    printf("\tHistorico: %s\n", p.historico);
}

// Le uma data a partir de um texto de entrada
Data lerData(char texto[]) {
    Data d;
    printf("%s", texto);
    scanf_s("%d %d %d", &d.dia, &d.mes, &d.ano);
    getchar(); // Limpa o buffer
    return d;
}

// Le os dados de um prontuario a partir do teclado
Prontuario lerProntuario() {
    Prontuario p;
    printf("\nNome: ");
    fgets(p.nome, sizeof(p.nome), stdin);
    printf("CPF: ");
    scanf_s("%d", &p.cpf);
    getchar(); // Limpa o buffer
    p.dataNas = lerData("Digite a data de nascimento (dd mm aaaa): ");
    printf("Historico medico (resumo): ");
    fgets(p.historico, sizeof(p.historico), stdin);
    return p;
}

// ----------- Funcoes da tabela hash -----------

// Funcao de dispersao (hash) baseada no CPF
int funcaoHash(int chave) {
    return chave % TAM;
}

// Inicializa a tabela, marcando todas as posicoes como vazias (cpf = 0)
void inicializarTabela(Prontuario t[]) {
    for (int i = 0; i < TAM; i++) {
        t[i].cpf = 0;
    }
}

// Insere um prontuario na tabela usando sondagem linear e reaproveitamento de posicoes removidas
void inserir(Prontuario t[]) {
    Prontuario p = lerProntuario();
    int id = funcaoHash(p.cpf);  // posicao inicial
    int original = id;
    int posRemovida = -1;

    // Procura uma posicao disponível ou atualiza se ja existir
    while (t[id].cpf != 0) {
        if (t[id].cpf == p.cpf) {
            // CPF ja existe: atualiza o prontuario
            printf("\nCPF ja existente. Atualizando prontuario...\n");
            t[id] = p;
            return;
        }

        // Armazena a primeira posicao removida encontrada
        if (t[id].cpf == -1 && posRemovida == -1) {
            posRemovida = id;
        }

        id = (id + 1) % TAM;

        // Evita loop infinito
        if (id == original) {
            break;
        }
    }

    // Reutiliza posicao removida ou insere na primeira vaga vazia
    if (posRemovida != -1) {
        t[posRemovida] = p;
        printf("\nProntuario inserido na posicao %d (reaproveitada).\n", posRemovida);
    }
    else if (t[id].cpf == 0) {
        t[id] = p;
        printf("\nProntuario inserido na posicao %d.\n", id);
    }
    else {
        printf("\nTabela cheia! Nao foi possivel inserir.\n");
    }
}

// Busca um prontuario na tabela pelo CPF, usando sondagem linear
Prontuario* buscar(Prontuario t[], int cpf) {
    int id = funcaoHash(cpf);
    int original = id;

    while (t[id].cpf != 0) {
        if (t[id].cpf == cpf) {
            return &t[id]; // Ponteiro para o prontuario encontrado
        }
        id = (id + 1) % TAM;
        if (id == original) // Evita loop infinito
            break;
    }
    return NULL; // Nao encontrado
}

// Atualiza um prontuario existente
void atualizar(Prontuario t[]) {
    int cpf;
    printf("\nDigite o CPF para atualizar: ");
    scanf_s("%d", &cpf);
    getchar();

    Prontuario* p = buscar(t, cpf);
    if (p) {
        printf("\nAtualizando prontuario...\n");
        *p = lerProntuario();
    }
    else {
        printf("\nCPF nao encontrado!\n");
    }
}

// Remove um prontuario, marcando a posicao como "removida" (cpf = -1)
void remover(Prontuario t[]) {
    int cpf;
    printf("\nDigite o CPF para remover: ");
    scanf_s("%d", &cpf);
    getchar();

    Prontuario* p = buscar(t, cpf);
    if (p) {
        p->cpf = -1;
        printf("\nProntuario removido com sucesso.\n");
    }
    else {
        printf("\nCPF nao encontrado!\n");
    }
}

// Imprime todos os elementos da tabela
void imprimir(Prontuario t[]) {
    printf("\n\n\tTabela de Prontuarios\n");
    for (int i = 0; i < TAM; i++) {
        printf("\n%d:\n", i);
        if (t[i].cpf == 0)
            printf("\t<vaga vazia>\n");
        else if (t[i].cpf == -1)
            printf("\t<removido>\n");
        else
            imprimirProntuario(t[i]);
        printf("----------------------\n");
    }
}

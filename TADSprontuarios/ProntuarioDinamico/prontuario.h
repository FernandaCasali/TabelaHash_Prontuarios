#ifndef PRONTUARIO_H
#define PRONTUARIO_H

#define TAM 10

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

extern No* tabela[TAM];

void inicializarTabela();
void inserir();
void buscarTodos(int cpf);
void remover(int cpf);
void atualizar(int cpf);
void imprimirTabela();

#endif

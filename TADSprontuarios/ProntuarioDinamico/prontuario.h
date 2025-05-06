#ifndef PRONTUARIO_H
#define PRONTUARIO_H

//Defini tamanho da tabela
#define TAM 10

//-------------- Structs --------------
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

//-------------- Lista  --------------

extern No* tabela[TAM];

//-------------- Funcoes --------------

void inicializarTabela();
void inserir();
void buscarTodos(int cpf);
void remover(int cpf);
void atualizar(int cpf);
void imprimirTabela();

#endif

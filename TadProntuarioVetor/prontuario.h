#ifndef PRONTUARIO_H
#define PRONTUARIO_H

#define TAM 15

typedef struct {
    int dia, mes, ano;
} Data;

typedef struct {
    int cpf;
    char nome[50];
    Data dataNas;
    char historico[300];
} Prontuario;

// -------- Fun��es auxiliares --------
void imprimirData(Data d);
void imprimirProntuario(Prontuario p);
Data lerData(char texto[]);
Prontuario lerProntuario();

// -------- Fun��es da tabela hash --------
void inicializarTabela(Prontuario t[]);
int funcaoHash(int chave);
void inserir(Prontuario t[]);
Prontuario* buscar(Prontuario t[], int cpf);
void atualizar(Prontuario t[]);
void remover(Prontuario t[]);
void imprimir(Prontuario t[]);

#endif

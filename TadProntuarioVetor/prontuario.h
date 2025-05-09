
#define PRONTUARIO_H

#define TAM 149  // Tamanho da tabela hash (primo maior que 2 * numero de entradas)

typedef struct {
    int dia, mes, ano;
} Data;

typedef struct {
    int cpf;
    char nome[50];
    Data dataNas;
    char historico[300];
} Prontuario;

// -------- Funções auxiliares --------
void imprimirData(Data d);
void imprimirProntuario(Prontuario p);
Data lerData(char texto[]);
Prontuario lerProntuario();

// -------- Funções da tabela hash --------
void inicializarTabela(Prontuario t[]);
int funcaoHash(int chave);
void inserir(Prontuario t[]);
Prontuario* buscar(Prontuario t[], int cpf);
void atualizar(Prontuario t[]);
void remover(Prontuario t[]);
void imprimir(Prontuario t[]);



/*
Insercoes	Carga ideal (< 0.7)	Tamanho recomendado (próximo primo > inserções / 0.7)
100	-> 143	(149)
200	-> 286	(293)
500 ->	714	(719)
1.000 ->	1.429	(1439)
5.000 ->	7.143	(7151)
8.000 ->	11.429	(11579)
10.000 ->	14.286	(14347)
100.000 ->	142.857	(143263)
1.000.000 ->	1.428.571	(1432669)

100 entradas -> Dobro de 100 = 200 -> Primeiro primo maior: 211

200 entradas -> Dobro de 200 = 400 -> Primeiro primo maior: 409

500 entradas -> Dobro de 500 = 1000 -> Primeiro primo maior: 1009

1000 entradas -> Dobro de 1000 = 2000 -> Primeiro primo maior: 2003

5000 entradas -> Dobro de 5000 = 10000 -> Primeiro primo maior: 10061

8000 entradas -> Dobro de 8000 = 16000 -> Primeiro primo maior: 16007

10000 entradas -> Dobro de 10000 = 20000 -> Primeiro primo maior: 20011

100000 entradas -> Dobro de 100000 = 200000 -> Primeiro primo maior: 200003

1000000 entradas -> Dobro de 1000000 = 2000000 -> Primeiro primo maior: 2000003
*/
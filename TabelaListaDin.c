#include <stdio.h>
#include <stdlib.h>

//2 * 15 = 31 (primo mais proximo do dobro)
#define TAM 31


//-------------- Struct --------------

typedef struct no
{
	int chave;
	struct no* proximo;
} No;

typedef struct
{
	No* inicio;
	int tam;
} Lista;

//-------------- Funções Lista -------------------

void inicializarLista(Lista* l)
{
	l->inicio = NULL;
	l->tam = 0;
}

void inserirNaLista(Lista* l, int valor)
{
	No* novo = malloc(sizeof(No));

	if (novo)
	{
		novo->chave = valor;
		novo->proximo = l->inicio;
		l->inicio = novo;
		l->tam++;
	}
	else
		printf("\n\tErro ao alocar memoria!\n");
}

int buscarNaLista(Lista* l, int valor) {
	No* aux = l->inicio;
	while (aux && aux->chave != valor)
		aux = aux->proximo;
	if (aux)
		return aux->chave;
	return 0;
}

void imprimirLista(Lista* l) {
	No* aux = l->inicio;
	printf(" Tam: %d: ", l->tam);
	while (aux) {
		printf("%d ", aux->chave);
		aux = aux->proximo;
	}
}
//

// ------------ Funções Tabela Hash ------------------

void inicializarTabela(Lista t[])
{
	int i;
	for (i = 0; i < TAM; i++)
	{
		inicializarLista(&t[i]);
	}
}

int funcaoHash(int chave)
{
	return chave % TAM;
}

void inserir(Lista t[], int valor)
{
	int id = funcaoHash(valor);
	inserirNaLista(&t[id], valor);
}

int busca(Lista t[], int chave)
{
	int id = funcaoHash(chave);
	printf("\nIndice gerado: %d", id);
	return buscarNaLista(&t[id], chave);
}

void imprimir(Lista t[])
{
	int i;
	for (i = 0; i < TAM; i++) {
		printf("%2d = ", i);
		imprimirLista(&t[i]);
		printf("\n");
	}

}
//

// -------------- Main -------------------

int main()
{
	int opc, valor, retorno;
	Lista tabela[TAM];

	inicializarTabela(tabela);

	do
	{
		printf("\n\t0 - Sair\n\t1 - Inserir\n\t2 - Buscar\n\t3 - Imprimir\n");
		if (scanf_s("%d", &opc) != 1) {
			printf("Erro ao ler a opção\n");
			break;
		}

		switch (opc)
		{
		case 1:
			printf("\tQual valor deseja inserir? ");
			if (scanf_s("%d", &valor) != 1) {
				printf("Erro ao ler o valor\n");
				break;
			}
			inserir(tabela, valor);
			break;
		case 2:
			printf("\tQual valor deseja buscar? ");
			if (scanf_s("%d", &valor) != 1) {
				printf("Erro ao ler o valor\n");
				break;
			}
			retorno = busca(tabela, valor);
			if (retorno != 0)
			{
				printf("\tValor encontrado: %d\n", retorno);
			}
			else
			{
				printf("\tValor nao encontrado!\n");
			}
			break;
		case 3:
			imprimir(tabela);
			break;
		default:
			printf("Opcao Invalida!\n");
			break;
		}

	} while (opc != 0);

	return 0;
}

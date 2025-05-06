#include <stdio.h>
#include "prontuario.h"

int main() {
    int opc, valor;
    Prontuario* encontrado;
    Prontuario tabela[TAM];

    inicializarTabela(tabela);

    do {
        printf("\n\t------MENU------");
        printf("\n\t0 - Sair\n\t1 - Inserir prontuario\n\t2 - Buscar por CPF\n\t3 - Imprimir tabela\n\t4 - Atualizar prontuario\n\t5 - Remover prontuario\n");
        printf("\nEscolha uma opcao: ");
        scanf_s("%d", &opc);
        getchar();

        switch (opc) {
        case 1:
            inserir(tabela);
            break;
        case 2:
            printf("\nDigite o CPF a buscar: ");
            scanf_s("%d", &valor);
            getchar();
            encontrado = buscar(tabela, valor);
            if (encontrado) {
                printf("\nProntuario encontrado:\n");
                imprimirProntuario(*encontrado);
            }
            else {
                printf("\nCPF nao encontrado!\n");
            }
            break;
        case 3:
            imprimir(tabela);
            break;
        case 4:
            atualizar(tabela);
            break;
        case 5:
            remover(tabela);
            break;
        case 0:
            printf("Encerrando...\n");
            break;
        default:
            printf("Opcao invalida!\n");
        }
    } while (opc != 0);

    return 0;
}

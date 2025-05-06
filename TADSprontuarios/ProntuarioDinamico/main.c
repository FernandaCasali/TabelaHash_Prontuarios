#include <stdio.h>
#include "prontuario.h"

int main() {
    int opcao, cpf;
    inicializarTabela();

    do {
        printf("\n\n--- MENU ---\n");
        printf("1 - Cadastrar paciente\n");
        printf("2 - Buscar por CPF\n");
        printf("3 - Listar todos os prontuarios\n");
        printf("4 - Remover prontuario por CPF\n");
        printf("5 - Atualizar prontuario por CPF\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf_s("%d", &opcao);
        getchar();

        switch (opcao) {
        case 1:
            inserir();
            break;
        case 2:
            printf("Digite o CPF do paciente: ");
            scanf_s("%d", &cpf);
            getchar();
            buscarTodos(cpf);
            break;
        case 3:
            imprimirTabela();
            break;
        case 4:
            printf("\nDigite o CPF do paciente a ser removido: ");
            scanf_s("%d", &cpf);
            getchar();
            remover(cpf);
            break;
        case 5:
            printf("Digite o CPF do paciente a ser atualizado: ");
            scanf_s("%d", &cpf);
            getchar();
            atualizar(cpf);
            break;
        case 0:
            printf("Encerrando...\n");
            break;
        default:
            printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}

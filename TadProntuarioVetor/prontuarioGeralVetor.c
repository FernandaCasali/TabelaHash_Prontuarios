#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// ---------------- Funcoes auxiliares ----------------

void imprimirData(Data d) {
    printf("%02d/%02d/%04d\n", d.dia, d.mes, d.ano);
}

void imprimirProntuario(Prontuario p) {
    printf("\n\tNome: %s", p.nome);
    printf("\tCPF: %d\n", p.cpf);
    printf("\tData de nascimento: ");
    imprimirData(p.dataNas);
    printf("\tHistorico: %s\n", p.historico);
}

Data lerData(char texto[]) {
    Data d;
    printf("%s", texto);
    scanf_s("%d %d %d", &d.dia, &d.mes, &d.ano);
    getchar();
    return d;
}

Prontuario lerProntuario() {
    Prontuario p;
    printf("\nNome: ");
    fgets(p.nome, sizeof(p.nome), stdin);
    printf("CPF: ");
    scanf_s("%d", &p.cpf);
    getchar();
    p.dataNas = lerData("Digite a data de nascimento (dd mm aaaa): ");
    printf("Historico medico (resumo): ");
    fgets(p.historico, sizeof(p.historico), stdin);
    return p;
}

// ---------------- Hash ----------------

int funcaoHash(int chave) {
    return chave % TAM;
}

void inicializarTabela(Prontuario t[]) {
    for (int i = 0; i < TAM; i++) {
        t[i].cpf = 0; // 0 = vaga vazia
    }
}

void inserir(Prontuario t[]) {
    Prontuario p = lerProntuario();
    int id = funcaoHash(p.cpf);
    int original = id;
    int inserido = 0;

    while (t[id].cpf != 0 && t[id].cpf != -1) {
        if (t[id].cpf == p.cpf) {
            printf("\nCPF ja existente. Atualizando prontuario...\n");
            t[id] = p;
            inserido = 1;
            break;
        }
        id = (id + 1) % TAM;
        if (id == original) {
            printf("\nTabela cheia! Nao foi possivel inserir.\n");
            return;
        }
    }

    if (!inserido) {
        t[id] = p;
        printf("\nProntuario inserido na posicao %d.\n", id);
    }
}

Prontuario* buscar(Prontuario t[], int cpf) {
    int id = funcaoHash(cpf);
    int original = id;

    while (t[id].cpf != 0) {
        if (t[id].cpf == cpf) {
            return &t[id];
        }
        id = (id + 1) % TAM;
        if (id == original)
            break;
    }
    return NULL;
}

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

void remover(Prontuario t[]) {
    int cpf;
    printf("\nDigite o CPF para remover: ");
    scanf_s("%d", &cpf);
    getchar();

    Prontuario* p = buscar(t, cpf);
    if (p) {
        p->cpf = -1; // Marca como removido
        printf("\nProntuario removido com sucesso.\n");
    }
    else {
        printf("\nCPF nao encontrado!\n");
    }
}

void imprimir(Prontuario t[]) {
    for (int i = 0; i < TAM; i++) {
        printf("%d:\n", i);
        if (t[i].cpf == 0)
            printf("\t<vaga vazia>\n");
        else if (t[i].cpf == -1)
            printf("\t<removido>\n");
        else
            imprimirProntuario(t[i]);
        printf("----------------------\n");
    }
}

// ---------------- Main ----------------

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

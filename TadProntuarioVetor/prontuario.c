#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prontuario.h"  // Arquivo de cabecalho com as definicoes de Data e Prontuario

// ----------- Funcoes auxiliares -----------

// Imprime a data no formato dd/mm/aaaa
void imprimirData(Data d) {
    printf("%02d/%02d/%04d\n", d.dia, d.mes, d.ano);
}

// Imprime todos os dados de um prontuario medico
void imprimirProntuario(Prontuario p) {
    printf("\n\tNome: %s", p.nome);             
    printf("\tCPF: %d\n", p.cpf);               
    printf("\tData de nascimento: ");           
    imprimirData(p.dataNas);     // Chama funcao auxiliar para imprimir data
    printf("\tHistorico: %s\n", p.historico);   
}

// Solicita uma data do usuario e retorna preenchida
Data lerData(char texto[]) {
    Data d;
    printf("%s", texto);     // Exibe a mensagem pedida
    scanf_s("%d %d %d", &d.dia, &d.mes, &d.ano); 
    getchar(); 
    return d;
}

// Le os dados de um prontuario pelo teclado e retorna preenchido
Prontuario lerProntuario() {
    Prontuario p;

    printf("\nNome: ");
    fgets(p.nome, sizeof(p.nome), stdin);
    p.nome[strcspn(p.nome, "\n")] = 0;  // Remove o \n do final da string

    printf("\nCPF: ");
    scanf_s("%d", &p.cpf);
    getchar();  // Limpa o buffer

    p.dataNas = lerData("Digite a data de nascimento (dd mm aaaa): "); // Le a data

    printf("Historico medico (resumo): ");
    fgets(p.historico, sizeof(p.historico), stdin);
    p.historico[strcspn(p.historico, "\n")] = 0;  // Também remove o \n do historico

    return p;
}


// ----------- Funcoes da tabela hash -----------

// Funcao de hash simples que calcula a posicao com base no CPF
int funcaoHash(int chave) {
    return chave % TAM;  // Retorna o indice baseado no CPF e tamanho da tabela
}

// Inicializa todas as posicoes da tabela com CPF = 0 (posicao vazia)
void inicializarTabela(Prontuario t[]) {
    for (int i = 0; i < TAM; i++) {
        t[i].cpf = 0;  // Marca a posicao como vazia
    }
}

// Insere um novo prontuario na tabela com tratamento de colisao por sondagem linear
void inserir(Prontuario t[]) {
    Prontuario p = lerProntuario();         // Le o prontuario a ser inserido
    int id = funcaoHash(p.cpf);             // Calcula a posicao inicial usando a funcao hash
    int original = id;                      // Guarda a posicao original para deteccao de loop
    int posRemovida = -1;                   // Armazena a primeira posicao removida encontrada (se houver)

    // Laco para encontrar uma posicao valida ou atualizar
    while (t[id].cpf != 0) {
        if (t[id].cpf == p.cpf) {
            // Caso o CPF ja exista, atualiza o prontuario
            printf("\nCPF ja existente. Atualizando prontuario...\n");
            t[id] = p;
            return;
        }

        // Verifica se encontrou uma posicao marcada como "removida"
        if (t[id].cpf == -1 && posRemovida == -1) {
            posRemovida = id; // Salva a posicao para possivel reutilizacao
        }

        id = (id + 1) % TAM;  // Move para a proxima posicao (sondagem linear)

        if (id == original) {
            break;  // Sai do loop se voltou à posicao inicial (tabela cheia)
        }
    }

    // Se encontrou uma posicao "removida", reutiliza
    if (posRemovida != -1) {
        t[posRemovida] = p;
        printf("\nProntuario inserido na posicao %d (reaproveitada).\n", posRemovida);
    }
    // Se encontrou uma posicao vazia, insere normalmente
    else if (t[id].cpf == 0) {
        t[id] = p;
        printf("\nProntuario inserido na posicao %d.\n", id);
    }
    // Se nenhuma posicao disponivel foi encontrada
    else {
        printf("\nTabela cheia! Nao foi possivel inserir.\n");
    }
}

// Busca um prontuario pelo CPF, retornando um ponteiro para ele se existir
Prontuario* buscar(Prontuario t[], int cpf) {
    int id = funcaoHash(cpf);  // Calcula a posicao inicial
    int original = id;

    // Percorre a tabela procurando pelo CPF com sondagem linear
    while (t[id].cpf != 0) {
        if (t[id].cpf == cpf) {
            return &t[id];  // Retorna o ponteiro para o prontuario encontrado
        }
        id = (id + 1) % TAM;
        if (id == original) // Evita loop infinito
            break;
    }
    return NULL;  // CPF nao encontrado
}

// Atualiza um prontuario existente a partir de uma nova leitura do teclado
void atualizar(Prontuario t[]) {
    int cpf;
    printf("\nDigite o CPF para atualizar: ");
    scanf_s("%d", &cpf);
    getchar();   // Limpa o buffer

    Prontuario* p = buscar(t, cpf);  // Busca o prontuario
    if (p) {
        printf("\nAtualizando prontuario...\n");
        *p = lerProntuario();  // Substitui pelos novos dados
    }
    else {
        printf("\nCPF nao encontrado!\n");
    }
}

// Remove um prontuario logicamente (marca o CPF como -1)
void remover(Prontuario t[]) {
    int cpf;
    printf("\nDigite o CPF para remover: ");
    scanf_s("%d", &cpf);
    getchar();  // Limpa o buffer

    Prontuario* p = buscar(t, cpf);  // Busca o prontuario
    if (p) {
        p->cpf = -1;  // Marca como removido logicamente
        printf("\nProntuario removido com sucesso.\n");
    }
    else {
        printf("\nCPF nao encontrado!\n");
    }
}

// Exibe todos os prontuarios da tabela hash
void imprimir(Prontuario t[]) {
    printf("\n\n\tTabela de Prontuarios\n");
    for (int i = 0; i < TAM; i++) {
        printf("\n%d:\n", i);  // indice da posicao
        if (t[i].cpf == 0)
            printf("\t<vaga vazia>\n");     // Slot nunca utilizado
        else if (t[i].cpf == -1)
            printf("\t<removido>\n");       // Slot com prontuario removido
        else
            imprimirProntuario(t[i]);       // Slot ocupado com prontuario valido
        printf("----------------------\n");
    }
}

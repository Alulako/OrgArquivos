#include "funcoes.h"

int main(){

    int escolha;
    char nomearq1[30], nomearq2[30];

    scanf("%d", &escolha); // armazena a escolha feita

    switch (escolha){

        case 1: // funcionalidade 1

            scanf(" %s", nomearq1); // nome do arquivo csv de entrada
            scanf(" %s", nomearq2); // nome do arquivo binário de saída gerado

            funcao_lerRegistros(nomearq1, nomearq2);
            break;

        case 2: // funcionalidade 2

            scanf(" %s", nomearq1); // nome do arquivo binário de entrada

            funcao_imprimirRegistros(nomearq1);
            break;

        case 3: // funcionalidade 3

            scanf(" %s", nomearq1); // nome do arquivo binário de entrada

            funcao_pesquisarRegistros(nomearq1);
            break;

        default: // caso escreva um número diferente dos 3
            printf("Número inválido\n");
            break;

    }

    return 0;

}
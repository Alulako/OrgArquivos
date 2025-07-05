// Nome: Arthur Domingues Rios, Número USP: 13731751
// Nome: Ana Luíza Lasta Kodama, Número USP: 14651204

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

        case 4: // funcionalidade 4

            scanf(" %s", nomearq1); // nome do arquivo binário de entrada

            funcao_removerRegistros(nomearq1);
            break;

        case 5: // funcionalidade 5

            scanf(" %s", nomearq1); // nome do arquivo binário de entrada

            funcao_inserirRegistros(nomearq1);
            break;

        case 6: // funcionalidade 6

            scanf(" %s", nomearq1); // nome do arquivo binário de entrada

            funcao_atualizarRegistros(nomearq1);
            break;

        case 7: // funcionalidade 7

            scanf(" %s", nomearq1); // nome do arquivo binario de entrada
            scanf(" %s", nomearq2); // nome do arquivo da árvore b

            funcao_criarArvoreB(nomearq1, nomearq2);
            break;

        case 8: // funcionalidade 8

            scanf(" %s", nomearq1); // nome do arquivo binario de entrada
            scanf(" %s", nomearq2); // nome do arquivo da árvore b

            funcao_pesquisarRegistrosArvoreB(nomearq1, nomearq2);
            break;

        case 9: // funcionalidade 9

            scanf(" %s", nomearq1); // nome do arquivo binario de entrada
            scanf(" %s", nomearq2); // nome do arquivo da árvore b

            funcao_removerArvoreB(nomearq1, nomearq2);
            break;

        case 10: // funcionalidade 10

            scanf(" %s", nomearq1); // nome do arquivo binario de entrada
            scanf(" %s", nomearq2); // nome do arquivo da árvore b

            funcao_inserirArvoreB(nomearq1, nomearq2);
            break;

        case 11: // funcionalidade 11

            scanf(" %s", nomearq1); // nome do arquivo binario de entrada
            scanf(" %s", nomearq2); // nome do arquivo da árvore b

            funcao_atualizarArvoreB(nomearq1, nomearq2);
            break;

        default: // caso escreva um número diferente

            printf("Número inválido\n");
            break;

    }

    return 0;

}
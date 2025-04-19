#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registrocabecalho.h"
#include "registrodados.h"
#include "funcoes.h"

int main(){

    int escolha;
    char nomearq1[30], nomearq2[30];

    scanf("%d", &escolha);

    switch (escolha){

        case 1:

            scanf(" %s", nomearq1); // nome do arquivo csv de entrada
            scanf(" %s", nomearq2); // nome do arquivo binário de saída gerado

            funcao_lerRegistros(nomearq1, nomearq2);
            break;

        case 2:

            scanf(" %s", nomearq1); // nome do arquivo binário de entrada

            funcao_imprimirRegistros(nomearq1);
            break;

        case 3:

            scanf(" %s", nomearq1); // nome do arquivo binário de entrada

            funcao_pesquisarRegistros(nomearq1);
            break;

        default:
            printf("Número inválido\n");
            break;

    }

    return 0;

}
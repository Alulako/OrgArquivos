#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registrocabecalho.h"
#include "registrodados.h"
#include "funcoes.h"

int main(){

    FILE *fp;

    fp = fopen("arquivo.bin", "rb+"); // talvez seja melhor abrir e fechar o arquivo em cada função, usar a main apenas para chamar elas

    if(fp == NULL){

        printf("Erro ao abrir o arquivo");
        return -1;

    }

    int escolha; 

    scanf("%d", &escolha);

    switch (escolha){

        case 1:
            funcao_lerRegistros();
            break;

        case 2:
            funcao_imprimirRegistros();
            break;

        case 3:
            funcao_pesquisarRegistros();
            break;

        default:
            printf("Número inválido");
            break;

    }

    fclose(fp);

    return 0;

}
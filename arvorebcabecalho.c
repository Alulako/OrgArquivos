// Nome: Arthur Domingues Rios, Número USP: 13731751
// Nome: Ana Luíza Lasta Kodama, Número USP: 14651204

#include "arvorebcabecalho.h"

struct arvoreb_cabecalho{

    char status;
    int noRaiz;
    int proxRRN;
    int nroNos;

};

arvbcabecalho *criar_cabecalhoarvb(){ // função para criar uma struct de cabeçalho da arvore b

    arvbcabecalho *arvb_cabecalho = malloc(sizeof(arvbcabecalho));

    if(arvb_cabecalho == NULL){

        printf("Falha no processamento do arquivo. ");
        exit(0);

    }

    arvb_cabecalho->status = '1'; // armazena status
    arvb_cabecalho->noRaiz = -1; // armazena noRaiz
    arvb_cabecalho->proxRRN = 0; // armazena proxRRN
    arvb_cabecalho->nroNos = 0; // armazena nroNos

    return arvb_cabecalho;

}

void escrever_cabecalhoarvb(FILE *fileout){ // função para escrever o cabeçalho da arvore b

    arvbcabecalho *arvb_cabecalho = criar_cabecalhoarvb();

    fwrite(&(arvb_cabecalho->status), sizeof(char), 1, fileout); // escreve status
    fwrite(&(arvb_cabecalho->noRaiz), sizeof(int), 1, fileout); // escreve noRaiz
    fwrite(&(arvb_cabecalho->proxRRN), sizeof(int), 1, fileout); // escreve proRRN
    fwrite(&(arvb_cabecalho->nroNos), sizeof(int), 1, fileout); // escreve nroNos

    free(arvb_cabecalho);

}
// Nome: Arthur Domingues Rios, Número USP: 13731751
// Nome: Ana Luíza Lasta Kodama, Número USP: 14651204

#include "arvorebno.h"

struct arvoreb_no{

    int tipoNo;
    int nroChaves;
    int P1;
    int C1;
    long long int PR1;
    int P2;
    int C2;
    long long int PR2;
    int P3;

};

arvbno *criar_no(){ // função para criar uma struct de nó da arvore b

    arvbno *arvb_no = malloc(sizeof(arvbno));

    if(arvb_no == NULL){

        printf("Falha no processamento do arquivo. ");
        exit(0);

    }

    arvb_no->tipoNo = -1; // inicializa tipoNo
    arvb_no->nroChaves = 0; // inicializa nroChaves
    arvb_no->P1 = -1; // inicializa P1
    arvb_no->C1 = -1; // inicializa C1
    arvb_no->PR1 = -1; // inicializa PR1
    arvb_no->P2 = -1; // inicializa P2
    arvb_no->C2 = -1; // inicializa C2
    arvb_no->PR2 = -1; // inicializa PR2
    arvb_no->P3 = -1; // inicializa P3

    return arvb_no;

}

void escrever_no(FILE *fileout){ // função para escrever o nó da arvore b

    arvbno *arvb_no = criar_no();

    fwrite(&(arvb_no->tipoNo), sizeof(int), 1, fileout); // escreve tipoNo
    fwrite(&(arvb_no->nroChaves), sizeof(int), 1, fileout); // escreve nroChaves
    fwrite(&(arvb_no->P1), sizeof(int), 1, fileout); // escreve P1
    fwrite(&(arvb_no->C1), sizeof(int), 1, fileout); // escreve C1
    fwrite(&(arvb_no->PR1), sizeof(long long int), 1, fileout); // escreve PR1
    fwrite(&(arvb_no->P2), sizeof(int), 1, fileout); // escreve P2
    fwrite(&(arvb_no->C2), sizeof(int), 1, fileout); // escreve C2
    fwrite(&(arvb_no->PR2), sizeof(long long int), 1, fileout); // escreve PR2
    fwrite(&(arvb_no->P3), sizeof(int), 1, fileout); // escreve P3

    free(arvb_no);
    
}
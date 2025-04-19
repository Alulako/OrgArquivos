#include "registrocabecalho.h"

#define TAM_DESC_IDENTIFICADOR 23
#define TAM_DESC_YEAR 27
#define TAM_DESC_FINANCIALLOSS 28
#define TAM_DESC_COUNTRY 26
#define TAM_DESC_TYPE 38
#define TAM_DESC_TARGETIND 38
#define TAM_DESC_DEFENSE 67

struct registro_cabecalho{

    char status;                               
    long long topo;                            
    long long proxByteOffset;                  
    int nroRegArq;                         
    int nroRegRem;                           
    char descreveIdentificador[TAM_DESC_IDENTIFICADOR];
    char descreveYear[TAM_DESC_YEAR];
    char descreveFinancialLoss[TAM_DESC_FINANCIALLOSS];
    char codDescreveCountry;
    char descreveCountry[TAM_DESC_COUNTRY];
    char codDescreveType;
    char descreveType[TAM_DESC_TYPE];
    char codDescreveTargetIndustry;
    char descreveTargetIndustry[TAM_DESC_TARGETIND];
    char codDescreveDefense;
    char descreveDefense[TAM_DESC_DEFENSE];

};

void criar_stringTamFixo(FILE *fp, char *campo, int tamanho){

    fread(campo, sizeof(char), tamanho, fp); // armazena o tamanho do campo no campo

    fseek(fp, 1, SEEK_CUR); // pula a ","

}

cabecalho *criar_cabecalho(FILE *filein){

    cabecalho *regcabecalho = malloc(sizeof(cabecalho));

    if(regcabecalho == NULL){

        printf("Falha no processamento do arquivo.");
        exit(1);

    }

    regcabecalho->status = '0';
    regcabecalho->topo = -1;
    regcabecalho->proxByteOffset = 0;
    regcabecalho->nroRegArq = 0;
    regcabecalho->nroRegRem = 0;

    criar_stringTamFixo(filein, regcabecalho->descreveIdentificador, TAM_DESC_IDENTIFICADOR); // ler descreveIdentificador
    criar_stringTamFixo(filein, regcabecalho->descreveYear, TAM_DESC_YEAR); // ler descreveYear
    criar_stringTamFixo(filein, regcabecalho->descreveFinancialLoss, TAM_DESC_FINANCIALLOSS); // ler descreveFinancialLoss

    regcabecalho->codDescreveCountry = '1';
    criar_stringTamFixo(filein, regcabecalho->descreveCountry, TAM_DESC_COUNTRY); // ler descreveCountry

    regcabecalho->codDescreveType = '2';
    criar_stringTamFixo(filein, regcabecalho->descreveType, TAM_DESC_TYPE); // ler descreveType

    regcabecalho->codDescreveTargetIndustry = '3';
    criar_stringTamFixo(filein, regcabecalho->descreveTargetIndustry, TAM_DESC_TARGETIND); // lerDescreveTargetIndustry

    regcabecalho->codDescreveDefense = '4';
    criar_stringTamFixo(filein, regcabecalho->descreveDefense, TAM_DESC_DEFENSE); // ler descreveDefense

    return regcabecalho;

}


void escrever_cabecalho(FILE *filein, FILE *fileout){

    cabecalho *regcabecalho = criar_cabecalho(filein);

    fwrite(&(regcabecalho->status), sizeof(char), 1, fileout); // escrever status
    fwrite(&(regcabecalho->topo), sizeof(long long int), 1, fileout); // escrever topo
    fwrite(&(regcabecalho->proxByteOffset), sizeof(long long int), 1, fileout); // escrever proxByteOffset
    fwrite(&(regcabecalho->nroRegArq), sizeof(int), 1, fileout); // escrever nroRegArq
    fwrite(&(regcabecalho->nroRegRem), sizeof(int), 1, fileout); // escrever nroRegRem

    fwrite(&(regcabecalho->descreveIdentificador), sizeof(char), TAM_DESC_IDENTIFICADOR, fileout); // escrever descreveIdentificador
    fwrite(&(regcabecalho->descreveYear), sizeof(char), TAM_DESC_YEAR, fileout); // escrever descreveYear
    fwrite(&(regcabecalho->descreveFinancialLoss), sizeof(char), TAM_DESC_FINANCIALLOSS, fileout); // escrever descreveFinancialLoss

    fwrite(&(regcabecalho->codDescreveCountry), sizeof(char), 1, fileout); // escrever codDescreveCountry
    fwrite(&(regcabecalho->descreveCountry), sizeof(char), TAM_DESC_COUNTRY, fileout); // escrever descreveCountry

    fwrite(&(regcabecalho->codDescreveType), sizeof(char), 1, fileout); // escrever codDescreveType
    fwrite(&(regcabecalho->descreveType), sizeof(char), TAM_DESC_TYPE, fileout); // escrever descreveType

    fwrite(&(regcabecalho->codDescreveTargetIndustry), sizeof(char), 1, fileout); // escrever codDescreveTargetIndustry
    fwrite(&(regcabecalho->descreveTargetIndustry), sizeof(char), TAM_DESC_TARGETIND, fileout); // escrever descreveTargetIndustry

    fwrite(&(regcabecalho->codDescreveDefense), sizeof(char), 1, fileout); // escrever codDescreveDefense
    fwrite(&(regcabecalho->descreveDefense), sizeof(char), TAM_DESC_DEFENSE, fileout); // escrever descreveDefense

    free(regcabecalho);

}
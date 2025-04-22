// Nome: Arthur Domingues Rios, Número USP: 13731751
// Nome: Ana Luíza Lasta Kodama, Número USP: 14651204

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

void criar_stringTamFixo(FILE *fp, char *campo, int tamanho){ // função feita para criar uma string de cada campo de tamanho fixo

    fread(campo, sizeof(char), tamanho, fp); // armazena o tamanho do campo no campo

    fseek(fp, 1, SEEK_CUR); // pula a ","

}

cabecalho *criar_cabecalho(FILE *filein){ // função para criar uma struct do registro de cabeçalho

    cabecalho *regcabecalho = malloc(sizeof(cabecalho));

    if(regcabecalho == NULL){

        printf("Falha no processamento do arquivo. ");
        exit(0);

    }

    regcabecalho->status = '1'; // armazena status
    regcabecalho->topo = -1; // armazena topo
    regcabecalho->proxByteOffset = 0; // armazena proxByteOffset
    regcabecalho->nroRegArq = 0; // armazena nroRegArq
    regcabecalho->nroRegRem = 0; // armazena nroRegRem

    criar_stringTamFixo(filein, regcabecalho->descreveIdentificador, TAM_DESC_IDENTIFICADOR); // lê descreveIdentificador
    criar_stringTamFixo(filein, regcabecalho->descreveYear, TAM_DESC_YEAR); // lê descreveYear
    criar_stringTamFixo(filein, regcabecalho->descreveFinancialLoss, TAM_DESC_FINANCIALLOSS); // lê descreveFinancialLoss

    regcabecalho->codDescreveCountry = '1'; // armazena codDescreveCountry
    criar_stringTamFixo(filein, regcabecalho->descreveCountry, TAM_DESC_COUNTRY); // lê descreveCountry

    regcabecalho->codDescreveType = '2'; // armazena codDescreveType
    criar_stringTamFixo(filein, regcabecalho->descreveType, TAM_DESC_TYPE); // lê descreveType

    regcabecalho->codDescreveTargetIndustry = '3'; // armazena codDescreveTargetIndustry
    criar_stringTamFixo(filein, regcabecalho->descreveTargetIndustry, TAM_DESC_TARGETIND); // lê DescreveTargetIndustry

    regcabecalho->codDescreveDefense = '4'; // armazena codDescreveDefense
    criar_stringTamFixo(filein, regcabecalho->descreveDefense, TAM_DESC_DEFENSE); // lê descreveDefense

    return regcabecalho;

}


void escrever_cabecalho(FILE *filein, FILE *fileout){

    cabecalho *regcabecalho = criar_cabecalho(filein);

    fwrite(&(regcabecalho->status), sizeof(char), 1, fileout); // escreve status
    fwrite(&(regcabecalho->topo), sizeof(long long int), 1, fileout); // escreve topo
    fwrite(&(regcabecalho->proxByteOffset), sizeof(long long int), 1, fileout); // escreve proxByteOffset
    fwrite(&(regcabecalho->nroRegArq), sizeof(int), 1, fileout); // escreve nroRegArq
    fwrite(&(regcabecalho->nroRegRem), sizeof(int), 1, fileout); // escreve nroRegRem

    fwrite(&(regcabecalho->descreveIdentificador), sizeof(char), TAM_DESC_IDENTIFICADOR, fileout); // escreve descreveIdentificador
    fwrite(&(regcabecalho->descreveYear), sizeof(char), TAM_DESC_YEAR, fileout); // escreve descreveYear
    fwrite(&(regcabecalho->descreveFinancialLoss), sizeof(char), TAM_DESC_FINANCIALLOSS, fileout); // escreve descreveFinancialLoss

    fwrite(&(regcabecalho->codDescreveCountry), sizeof(char), 1, fileout); // escreve codDescreveCountry
    fwrite(&(regcabecalho->descreveCountry), sizeof(char), TAM_DESC_COUNTRY, fileout); // escreve descreveCountry

    fwrite(&(regcabecalho->codDescreveType), sizeof(char), 1, fileout); // escreve codDescreveType
    fwrite(&(regcabecalho->descreveType), sizeof(char), TAM_DESC_TYPE, fileout); // escreve descreveType

    fwrite(&(regcabecalho->codDescreveTargetIndustry), sizeof(char), 1, fileout); // escreve codDescreveTargetIndustry
    fwrite(&(regcabecalho->descreveTargetIndustry), sizeof(char), TAM_DESC_TARGETIND, fileout); // escreve descreveTargetIndustry

    fwrite(&(regcabecalho->codDescreveDefense), sizeof(char), 1, fileout); // escreve codDescreveDefense
    fwrite(&(regcabecalho->descreveDefense), sizeof(char), TAM_DESC_DEFENSE, fileout); // escreve descreveDefense

    free(regcabecalho);

}
#include "registrocabecalho.h"

struct registro_cabecalho{

    char status;
    long long int topo;
    long long int proxByteOffset;
    int nroRegNoRemov;
    int nroRegRemov;
    char descreveIdentificador[23];
    char descreveYear[27];
    char descreveFinancialLoss[28];
    char codDescreveCountry;
    char descreveCountry[26];
    char codDescreveType;
    char descreveType[38];
    char codDescreveTargetIndustry;
    char descreveTargetIndustry[38];
    char codDescreveDefense;
    char descreveDefense[67];

};

bool criar_cabecalho(){

struct registro_cabecalho cabecalho;

cabecalho.status = '0';
cabecalho.topo = -1;
cabecalho.proxByteOffset = 0;
cabecalho.nroRegNoRemov = 0;
cabecalho.nroRegRemov = 0;
strncpy(cabecalho.descreveIdentificador, "IDENTIFICADOR DO ATAQUE", 23);
strncpy(cabecalho.descreveYear, "ANO EM QUE O ATAQUE OCORREU", 27);
strncpy(cabecalho.descreveFinancialLoss, "PREJUIZO CAUSADO PELO ATAQUE", 28);
cabecalho.codDescreveCountry = '1';
strncpy(cabecalho.descreveCountry, "PAIS ONDE OCORREU O ATAQUE", 26);
cabecalho.codDescreveType = '2';
strncpy(cabecalho.descreveType, "TIPO DE AMEACA A SEGURANCA CIBERNETICA", 38);
cabecalho.codDescreveTargetIndustry = '3';
strncpy(cabecalho.descreveTargetIndustry, "SETOR DA INDUSTRIA QUE SOFREU O ATAQUE", 38);
cabecalho.codDescreveDefense = '4';
strncpy(cabecalho.descreveDefense, "ESTRATEGIA DE DEFESA CIBERNETICA EMPREGADA PARA RESOLVER O PROBLEMA", 67);

}

bool escrever_cabecalho();

bool ler_cabecalho();
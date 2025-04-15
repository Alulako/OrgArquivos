#include "registrocabecalho.h"

void inicializarCabecalho(Cabecalho *cab) {
    if (!cab) return;
    cab->status = '0';             // arquivo inconsistente no início
    cab->topo = -1;
    cab->proxByteOffset = 0;
    cab->nroRegNoRemov = 0;
    cab->nroRegRemov = 0;

    // Copia as strings/códigos exigidos, sem colocar '\0' a mais
    // (enunciado: "Não devem ser finalizados com '\0'")
    // Entretanto, se as strings do enunciado têm exatos 23/27/etc chars,
    //  iremos usar strncpy.

    // 23 bytes: "IDENTIFICADOR DO ATAQUE" (21 chars + 2 de espaço extra, mas pode vir sem \0)
    strncpy(cab->descreveIdentificador, "IDENTIFICADOR DO ATAQUE", TAM_DESC_IDENTIFICADOR);

    // 27 bytes: "ANO EM QUE O ATAQUE OCORREU"
    strncpy(cab->descreveYear, "ANO EM QUE O ATAQUE OCORREU", TAM_DESC_YEAR);

    // 28 bytes: "PREJUIZO CAUSADO PELO ATAQUE"
    strncpy(cab->descreveFinancialLoss, "PREJUIZO CAUSADO PELO ATAQUE", TAM_DESC_FINANCIALLOSS);

    // codDescreveCountry = '1'; e 26 bytes: "PAIS ONDE OCORREU O ATAQUE"
    cab->codDescreveCountry = '1';
    strncpy(cab->descreveCountry, "PAIS ONDE OCORREU O ATAQUE", TAM_DESC_COUNTRY);

    // codDescreveType = '2'; e 38 bytes: "TIPO DE AMEACA A SEGURANCA CIBERNETICA"
    cab->codDescreveType = '2';
    strncpy(cab->descreveType, "TIPO DE AMEACA A SEGURANCA CIBERNETICA", TAM_DESC_TYPE);

    // codDescreveTargetIndustry = '3'; e 38 bytes: "SETOR DA INDUSTRIA QUE SOFREU O ATAQUE"
    cab->codDescreveTargetIndustry = '3';
    strncpy(cab->descreveTargetIndustry, "SETOR DA INDUSTRIA QUE SOFREU O ATAQUE", TAM_DESC_TARGETIND);

    // codDescreveDefense = '4'; e 67 bytes:
    // "ESTRATEGIA DE DEFESA CIBERNETICA EMPREGADA PARA RESOLVER O PROBLEMA"
    cab->codDescreveDefense = '4';
    strncpy(cab->descreveDefense,
            "ESTRATEGIA DE DEFESA CIBERNETICA EMPREGADA PARA RESOLVER O PROBLEMA",
            TAM_DESC_DEFENSE);
}

bool escreverCabecalhoBin(FILE *fp, Cabecalho *cab) {
    if (!fp || !cab) return false;

    // Posiciona ponteiro no início:
    fseek(fp, 0, SEEK_SET);

    // Escreve campo a campo, garantindo a ordem e tamanho fixos:
    fwrite(&cab->status, sizeof(char), 1, fp);

    fwrite(&cab->topo, sizeof(long long), 1, fp);
    fwrite(&cab->proxByteOffset, sizeof(long long), 1, fp);
    fwrite(&cab->nroRegNoRemov, sizeof(int), 1, fp);
    fwrite(&cab->nroRegRemov, sizeof(int), 1, fp);

    fwrite(cab->descreveIdentificador, sizeof(char), TAM_DESC_IDENTIFICADOR, fp);
    fwrite(cab->descreveYear, sizeof(char), TAM_DESC_YEAR, fp);
    fwrite(cab->descreveFinancialLoss, sizeof(char), TAM_DESC_FINANCIALLOSS, fp);

    fwrite(&cab->codDescreveCountry, sizeof(char), 1, fp);
    fwrite(cab->descreveCountry, sizeof(char), TAM_DESC_COUNTRY, fp);

    fwrite(&cab->codDescreveType, sizeof(char), 1, fp);
    fwrite(cab->descreveType, sizeof(char), TAM_DESC_TYPE, fp);

    fwrite(&cab->codDescreveTargetIndustry, sizeof(char), 1, fp);
    fwrite(cab->descreveTargetIndustry, sizeof(char), TAM_DESC_TARGETIND, fp);

    fwrite(&cab->codDescreveDefense, sizeof(char), 1, fp);
    fwrite(cab->descreveDefense, sizeof(char), TAM_DESC_DEFENSE, fp);

    return true;
}

bool lerCabecalhoBin(FILE *fp, Cabecalho *cab) {
    if (!fp || !cab) return false;

    // Garante que a leitura começará do byte 0
    fseek(fp, 0, SEEK_SET);

    // Lê campo a campo na mesma ordem
    fread(&cab->status, sizeof(char), 1, fp);

    fread(&cab->topo, sizeof(long long), 1, fp);
    fread(&cab->proxByteOffset, sizeof(long long), 1, fp);
    fread(&cab->nroRegNoRemov, sizeof(int), 1, fp);
    fread(&cab->nroRegRemov, sizeof(int), 1, fp);

    fread(cab->descreveIdentificador, sizeof(char), TAM_DESC_IDENTIFICADOR, fp);
    fread(cab->descreveYear, sizeof(char), TAM_DESC_YEAR, fp);
    fread(cab->descreveFinancialLoss, sizeof(char), TAM_DESC_FINANCIALLOSS, fp);

    fread(&cab->codDescreveCountry, sizeof(char), 1, fp);
    fread(cab->descreveCountry, sizeof(char), TAM_DESC_COUNTRY, fp);

    fread(&cab->codDescreveType, sizeof(char), 1, fp);
    fread(cab->descreveType, sizeof(char), TAM_DESC_TYPE, fp);

    fread(&cab->codDescreveTargetIndustry, sizeof(char), 1, fp);
    fread(cab->descreveTargetIndustry, sizeof(char), TAM_DESC_TARGETIND, fp);

    fread(&cab->codDescreveDefense, sizeof(char), 1, fp);
    fread(cab->descreveDefense, sizeof(char), TAM_DESC_DEFENSE, fp);

    return true;
}

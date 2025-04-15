#ifndef _REGISTROCABECALHO_H
#define _REGISTROCABECALHO_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* Definições de tamanho fixo de cada campo,
   conforme especificado no enunciado (total = 275 bytes). */
#define TAM_DESC_IDENTIFICADOR 23
#define TAM_DESC_YEAR 27
#define TAM_DESC_FINANCIALLOSS 28
#define TAM_DESC_COUNTRY 26
#define TAM_DESC_TYPE 38
#define TAM_DESC_TARGETIND 38
#define TAM_DESC_DEFENSE 67

/* Esta struct de cabeçalho deve ter exatamente 275 bytes:
   - status (1 byte)
   - topo (8 bytes)
   - proxByteOffset (8 bytes)
   - nroRegNoRemov (4 bytes)
   - nroRegRemov (4 bytes)
   - descreveIdentificador (23 bytes)
   - descreveYear (27 bytes)
   - descreveFinancialLoss (28 bytes)
   - codDescreveCountry (1 byte)
   - descreveCountry (26 bytes)
   - codDescreveType (1 byte)
   - descreveType (38 bytes)
   - codDescreveTargetIndustry (1 byte)
   - descreveTargetIndustry (38 bytes)
   - codDescreveDefense (1 byte)
   - descreveDefense (67 bytes)
*/
typedef struct {
    char status;                               // 1 byte
    long long topo;                            // 8 bytes
    long long proxByteOffset;                  // 8 bytes
    int nroRegNoRemov;                         // 4 bytes
    int nroRegRemov;                           // 4 bytes

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
} Cabecalho;

/**
 * @brief Inicializa a struct de cabeçalho com valores padrão
 *  (status = '0', topo = -1, proxByteOffset = 0, nroRegNoRemov = 0, nroRegRemov=0)
 *  e copia as descrições fixas exigidas pelo enunciado.
 *
 * @param cab Ponteiro para a struct Cabecalho
 */
void inicializarCabecalho(Cabecalho *cab);

/**
 * @brief Escreve o cabeçalho no arquivo binário, campo a campo,
 *        na ordem e tamanho exatos (275 bytes).
 *
 * @param fp Arquivo binário aberto em modo escrita/atualização
 * @param cab Ponteiro para a struct Cabecalho já preenchida
 * @return true se escreveu com sucesso
 * @return false em caso de erro (fp null, por ex.)
 */
bool escreverCabecalhoBin(FILE *fp, Cabecalho *cab);

/**
 * @brief Lê o cabeçalho (275 bytes) do arquivo binário, campo a campo,
 *        povoando a struct Cabecalho em memória.
 *
 * @param fp Arquivo binário aberto em modo leitura/atualização
 * @param cab Ponteiro para struct Cabecalho onde serão carregados os dados
 * @return true se leu com sucesso
 * @return false em caso de erro (fp null, EOF inesperado, etc.)
 */
bool lerCabecalhoBin(FILE *fp, Cabecalho *cab);

#endif

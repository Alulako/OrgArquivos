// Nome: Arthur Domingues Rios, Número USP: 13731751
// Nome: Ana Luíza Lasta Kodama, Número USP: 14651204

#ifndef _REG_DADO_H
#define _REG_DADO_H

    #include <stdbool.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    typedef struct registro_dado {
    int idAttack;
    int year;
    float financialLoss;
    char *country;
    char *attackType;
    char *targetIndustry;
    char *defenseMechanism;
    char removido;
    int tamanhoRegistro;
    long long prox;
} dados;

    void escrever_dado(FILE *filein, FILE *fileout);
    dados *ler_regdados(FILE *pos_registro);
    void atualizar_regdados(dados *regdados, char *nomecampo, void *valorcampo);
    int atualizar_tamanho(dados *regdados);
    dados *criar_dado(FILE *filein);
    void escrever_campoTamVar(FILE *fp, char *campo, char keyword);

#endif
// Nome: Arthur Domingues Rios, Número USP: 13731751
// Nome: Ana Luíza Lasta Kodama, Número USP: 14651204

#ifndef _REG_DADO_H
#define _REG_DADO_H

    #include <stdbool.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    struct registro_dado{
    int idAttack;
    int year;
    float financialLoss;
    char *country;
    char *attackType;
    char *targetIndustry;
    char *defenseMechanism;
    char removido;
    int tamanhoRegistro;
    long long int prox;
    };

    typedef struct registro_dado dados;

    dados *ler_regdados(FILE *pos_registro);
    void escrever_dado(FILE *filein, FILE *fileout);
    void atualizar_regdados(dados *regdados, char *nomecampo, void *valorcampo);
    void escrever_campoTamVar(FILE *fp, char *campo, char keyword);
    void liberar_regdados(dados *regdados);
    int atualizar_tamanho(dados *regdados);
    int get_tamanho(dados *regdados);
    int get_idAttack(dados *regdados);
    int get_year(dados *regdados);
    float get_financialLoss(dados *regdados);
    char *get_country(dados *regdados);
    char *get_attackType(dados *regdados);
    char *get_targetIndustry(dados *regdados);
    char *get_defenseMechanism(dados *regdados);


#endif
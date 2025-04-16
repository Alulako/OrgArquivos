#ifndef _REG_DADO_H
#define _REG_DADO_H

    #include <stdbool.h>
    #include <stdio.h>

    typedef struct registro_dado dados;

    dados *criar_dado(FILE *filein);
    bool escrever_dado(FILE *filein, FILE *fileout);
    bool ler_dado();
    // fazer um get para todos os campos do registro (?)


#endif
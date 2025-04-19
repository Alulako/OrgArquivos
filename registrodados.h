#ifndef _REG_DADO_H
#define _REG_DADO_H

    #include <stdbool.h>
    #include <stdio.h>
    #include <string.h>

    typedef struct registro_dado dados;

    void escrever_dado(FILE *filein, FILE *fileout);

#endif
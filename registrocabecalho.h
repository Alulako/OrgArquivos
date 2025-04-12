#ifndef _REG_CABECALHO_H
#define _REG_CABECALHO_H

    #include <stdbool.h>

    typedef struct registro_cabecalho cabecalho;

    bool criar_cabecalho();
    bool escrever_cabecalho();
    bool ler_cabecalho();
    // fazer um get para todos os campos do registro (?)

#endif
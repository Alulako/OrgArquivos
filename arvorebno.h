// Nome: Arthur Domingues Rios, Número USP: 13731751
// Nome: Ana Luíza Lasta Kodama, Número USP: 14651204

#ifndef _ARVOREBNO_H
#define _ARVOREBNO_H

    #include <stdio.h>
    #include <stdlib.h>

    typedef struct arvoreb_no arvbno;

    arvbno *criar_no();
    void escrever_no(FILE *fileout, arvbno *arvb_no);
    int get_tipoNo(arvbno *no);
    void set_tipoNo(arvbno *no, int valor);
    int get_nroChaves(arvbno *no);
    void set_nroChaves(arvbno *no, int valor);
    int get_P1(arvbno *no);
    void set_P1(arvbno *no, int valor);
    int get_C1(arvbno *no);
    void set_C1(arvbno *no, int valor);
    long long int get_PR1(arvbno *no);
    void set_PR1(arvbno *no, long long int valor);
    int get_P2(arvbno *no);
    void set_P2(arvbno *no, int valor);
    int get_C2(arvbno *no);
    void set_C2(arvbno *no, int valor);
    long long int get_PR2(arvbno *no);
    void set_PR2(arvbno *no, long long int valor);
    int get_P3(arvbno *no);
    void set_P3(arvbno *no, int valor);

#endif

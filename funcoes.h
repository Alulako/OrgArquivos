// Nome: Arthur Domingues Rios, Número USP: 13731751
// Nome: Ana Luíza Lasta Kodama, Número USP: 14651204

#ifndef _FUNCOES_H
#define _FUNCOES_H

    #include "registrocabecalho.h"
    #include "registrodados.h"

    void funcao_lerRegistros(char *nomein, char *nomeout);
    void funcao_imprimirRegistros(char *nomeout);
    void funcao_pesquisarRegistros(char *nomein);
    void funcao_removerRegistros(char *nomein);
    void funcao_inserirRegistros(char *nomein);
    void funcao_atualizarRegistros(char *nomein);
    void funcao_criarArvoreB(char *nomein, char *nomearvb);
    void funcao_pesquisarRegistrosArvoreB(char *nomein, char *nomearvb);
    void funcao_removerArvoreB(char *nomein, char *nomearvb);
    void funcao_inserirArvoreB(char *nomein, char *nomearvb);
    void funcao_atualizarArvoreB(char *nomein, char *nomearvb);

#endif

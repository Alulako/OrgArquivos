#include "registrodados.h"

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

bool criar_dado();
bool escrever_dado();
bool ler_dado();
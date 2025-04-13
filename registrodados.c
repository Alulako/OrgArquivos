#include "registrodados.h"

#define CAMPO_INT 0
#define CAMPO_FLOAT 1

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

void criar_campoTamVar(FILE *fp, char *campo){

    char tempchar;
    int tamcampo = 0;

    fread(&tempchar, sizeof(char), 1, fp);

    while(tempchar != "," && tempchar != '\n'){

        tamcampo++;
        fread(&tempchar, sizeof(char), 1, fp);

    }

    if(tamcampo == 0){  // caso o campo seja vazio

        campo = (char *)malloc(sizeof(char));
        strncpy(campo, "0", 1);

    }

    campo = (char *)malloc((tamcampo + 1)*sizeof(char));

    if(campo == NULL){

        printf("Erro ao alocar espaço para o campo");
        exit(1);

    }

    fseek(fp, -(tamcampo + 1), SEEK_CUR);
    fread(campo, sizeof(char), tamcampo, fp);
    campo[tamcampo] = '\0';

    fseek(fp, 1, SEEK_CUR);

}

void criar_campoTamFixo(FILE *fp, void *campo, int tipo){

    char tempchar;

    fread(&tempchar, sizeof(char), 1, fp);

    if(tipo == CAMPO_INT){

        if(tempchar == ","){

            *(int *)campo = -1;
            return;

        }

        fseek(fp, -1, SEEK_CUR);
        fread((int *)campo, sizeof(int), 1, fp);

    }

    if(tipo == CAMPO_FLOAT){

        if(tempchar == ","){

            *(float *)campo = -1.0;
            return;

        }

        fseek(fp, -1, SEEK_CUR);
        fread((float *)campo, sizeof(float), 1, fp);

    }

    fseek(fp, 1, SEEK_CUR);

}

dados *criar_dado(){

    dados *regdados;

    FILE *filein = fopen("nome.csv", "r");

    regdados->removido = "0";
    regdados->prox = -1;

    if(filein == NULL){

        printf("Falha ao abrir o arquivo");
        exit(1);
        
    }

    criar_campoTamFixo(filein, &(regdados->idAttack), CAMPO_INT); // ler idAttack
    criar_campoTamFixo(filein, &(regdados->year), CAMPO_INT); // ler year
    criar_campoTamFixo(filein, &(regdados->financialLoss), CAMPO_FLOAT); // ler financialLoss

    criar_campoTamVar(filein, regdados->country); // ler country
    criar_campoTamVar(filein, regdados->attackType); // ler attackType
    criar_campoTamVar(filein, regdados->targetIndustry); // ler targetIndustry
    criar_campoTamVar(filein, regdados->defenseMechanism); // ler defenseMechanism

    regdados->tamanhoRegistro = strlen(regdados->country) + strlen(regdados->attackType) + strlen(regdados->targetIndustry) 
    + strlen(regdados->defenseMechanism) + sizeof(regdados->idAttack) + sizeof(regdados->year) + sizeof(regdados->financialLoss);

    fclose(filein);

}

bool escrever_dado(){



}

bool ler_dado(){



}
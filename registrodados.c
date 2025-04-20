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

void criar_campoTamVar(FILE *fp, char **campo){

    char tempchar;
    int tamcampo = 0;

    fread(&tempchar, sizeof(char), 1, fp);

    while(tempchar != ',' && tempchar != '\n'){

        tamcampo++;
        fread(&tempchar, sizeof(char), 1, fp);

    }

    if(tamcampo == 0){  // caso o campo seja vazio

        *campo = NULL;
        return;
        
    }

    *campo = (char *)malloc((tamcampo + 1)*sizeof(char));

    if(*campo == NULL){

        printf("Falha no processamento do arquivo.");
        exit(1);

    }

    fseek(fp, -(tamcampo + 1), SEEK_CUR);
    fread(*campo, sizeof(char), tamcampo, fp);
    (*campo)[tamcampo] = '\0';

    fseek(fp, 1, SEEK_CUR);

}

void criar_campoTamFixo(FILE *fp, void *campo, int tipo){

    char tempchar;

    fread(&tempchar, sizeof(char), 1, fp);

    if(tipo == CAMPO_INT){

        if(tempchar == ','){

            *(int *)campo = -1;
            return;

        }

        fseek(fp, -1, SEEK_CUR);
        fread((int *)campo, sizeof(int), 1, fp);

    }

    if(tipo == CAMPO_FLOAT){

        if(tempchar == ','){

            *(float *)campo = -1.0;
            return;

        }

        fseek(fp, -1, SEEK_CUR);
        fread((float *)campo, sizeof(float), 1, fp);

    }

    fseek(fp, 1, SEEK_CUR);

}

dados *criar_dado(FILE *filein){

    dados *regdados = malloc(sizeof(dados));

    if(regdados == NULL){

        printf("Falha no processamento do arquivo.");
        exit(1);

    }

    regdados->removido = '0';
    regdados->prox = -1;

    criar_campoTamFixo(filein, &(regdados->idAttack), CAMPO_INT); // ler idAttack
    criar_campoTamFixo(filein, &(regdados->year), CAMPO_INT); // ler year
    criar_campoTamFixo(filein, &(regdados->financialLoss), CAMPO_FLOAT); // ler financialLoss

    criar_campoTamVar(filein, &(regdados->country)); // ler country
    criar_campoTamVar(filein, &(regdados->attackType)); // ler attackType
    criar_campoTamVar(filein, &(regdados->targetIndustry)); // ler targetIndustry
    criar_campoTamVar(filein, &(regdados->defenseMechanism)); // ler defenseMechanism

    regdados->tamanhoRegistro = sizeof(regdados->prox) + sizeof(regdados->idAttack) + sizeof(regdados->year) + 
    sizeof(regdados->financialLoss);

    if(regdados->country != NULL) // +2 por conta do delimitador e da keyword
        regdados->tamanhoRegistro = regdados->tamanhoRegistro + strlen(regdados->country) + 2;

    if(regdados->attackType != NULL)
        regdados->tamanhoRegistro = regdados->tamanhoRegistro + strlen(regdados->attackType) + 2;

    if(regdados->targetIndustry != NULL)
        regdados->tamanhoRegistro = regdados->tamanhoRegistro + strlen(regdados->targetIndustry) + 2;

    if(regdados->defenseMechanism != NULL)
        regdados->tamanhoRegistro = regdados->tamanhoRegistro + strlen(regdados->defenseMechanism) + 2;

    return regdados;

}

void escrever_campoTamVar(FILE *fp, char *campo, char keyword){

    char delimcampo = '|';

    fwrite(&keyword, sizeof(char), 1, fp); // escrever keyword
    fwrite(campo, sizeof(char), strlen(campo), fp); // escrever campo
    fwrite(&delimcampo, sizeof(char), 1, fp); // escrever delimitador

}

void escrever_dado(FILE *filein, FILE *fileout){

    dados *regdados = criar_dado(filein);

    fwrite(&(regdados->removido), sizeof(char), 1, fileout); // escrever removido
    fwrite(&(regdados->tamanhoRegistro), sizeof(int), 1, fileout); // escrever tamanhoRegistro
    fwrite(&(regdados->prox), sizeof(long long int), 1, fileout); // escrever prox
    fwrite(&(regdados->idAttack), sizeof(int), 1, fileout); // escrever idAttack
    fwrite(&(regdados->year), sizeof(int), 1, fileout); // escrever year
    fwrite(&(regdados->financialLoss), sizeof(float), 1, fileout); // escrever financialLoss

    if(regdados->country != NULL)
        escrever_campoTamVar(fileout, regdados->country, '1'); // escrever country

    if(regdados->attackType != NULL)
        escrever_campoTamVar(fileout, regdados->attackType, '2'); // escrever attackType

    if(regdados->targetIndustry != NULL)
        escrever_campoTamVar(fileout, regdados->targetIndustry, '3'); // escrever targetIndustry

    if(regdados->defenseMechanism != NULL)
        escrever_campoTamVar(fileout, regdados->defenseMechanism, '4'); // escrever defenseMechanism

    free(regdados->country);
    free(regdados->attackType);
    free(regdados->targetIndustry);
    free(regdados->defenseMechanism);

    free(regdados);

}
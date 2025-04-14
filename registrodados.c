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

        *campo = (char *)malloc(2*sizeof(char)); // colocar return aqui e checar se alocação funcionou
        
        if(*campo == NULL){

            printf("Erro ao alocar o espaço para o campo");
            exit(1);

        }

            (*campo)[0] = '$';
            (*campo)[1] = '\0';

            return;
        
    }

    *campo = (char **)malloc((tamcampo + 2)*sizeof(char));

    if(*campo == NULL){

        printf("Erro ao alocar espaço para o campo");
        exit(1); // fechar o arquivo após erro

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

    dados *regdados = malloc(sizeof(dados));

    if(regdados == NULL){

        printf("Falha na alocação");
        exit(1);

    }

    FILE *filein = fopen("nomein.csv", "rb");

    if(filein == NULL){

        printf("Falha ao abrir o arquivo");
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

    regdados->tamanhoRegistro = strlen(regdados->country) + strlen(regdados->attackType) + strlen(regdados->targetIndustry) 
    + strlen(regdados->defenseMechanism) + sizeof(regdados->prox) + sizeof(regdados->idAttack) + sizeof(regdados->year) + 
    sizeof(regdados->financialLoss) + 8;  // +8 é para adiocionar os delimitadores e para as keywords

    if((regdados->country)[0] == '$')
        regdados->tamanhoRegistro--;

    if((regdados->attackType)[0] == '$')
        regdados->tamanhoRegistro--;

    if((regdados->targetIndustry)[0] == '$')
        regdados->tamanhoRegistro--;

    if((regdados->defenseMechanism)[0] == '$')
        regdados->tamanhoRegistro--;

    fclose(filein);

    return regdados;

}

void escrever_campoTamVar(FILE *fp, char *campo, char keyword){

    char delimcampo = '|';

    fwrite(&keyword, sizeof(char), 1, fp); // escrever keyword
    fwrite(&campo, sizeof(char), 1, fp); // escrever campo
    fwrite(&delimcampo, sizeof(char), 1, fp); // escrever delimitador

}

bool escrever_dado(){

    FILE *fileout = fopen("nomeout.csv", "wb");

    if(fileout == NULL){

        printf("Falha ao abrir o arquivo");
        exit(1);  

    }

    dados *regdados = criar_dado();

    // falta tratar os campos vazios

    fwrite(&(regdados->removido), sizeof(char), 1, fileout); // escrever removido
    fwrite(&(regdados->tamanhoRegistro), sizeof(int), 1, fileout); // escrever tamanhoRegistro
    fseek(fileout, 8, SEEK_CUR); // pula prox para escrever depois
    fwrite(&(regdados->idAttack), sizeof(int), 1, fileout); // escrever idAttack
    fwrite(&(regdados->year), sizeof(int), 1, fileout); // escrever year
    fwrite(&(regdados->financialLoss), sizeof(float), 1, fileout); // escrever financialLoss

    escrever_campoTamVar(fileout, regdados->country, '1'); // escrever country
    escrever_campoTamVar(fileout, regdados->attackType, '2'); // escrever attackType
    escrever_campoTamVar(fileout, regdados->targetIndustry, '3'); // escrever targetIndustry
    escrever_campoTamVar(fileout, regdados->defenseMechanism, '4'); // escrever defenseMechanism

    regdados->prox = ftell(fileout);
    fseek(fileout, -(regdados->tamanhoRegistro - 5), SEEK_CUR); // volta para a posição para escrever prox
    fwrite(&(regdados->prox), sizeof(long long int), 1, fileout);

    fseek(fileout, regdados->tamanhoRegistro - 13, SEEK_CUR); // volta para o próximo byte offset

    fclose(fileout);

}

bool ler_dado(){

    FILE *fileout = fopen("nomeout.csv", "rb");

    if(fileout == NULL){

        printf("Falha ao abrir o arquivo");
        exit(1);

    }

    fclose(fileout);

}
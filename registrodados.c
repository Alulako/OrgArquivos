// Nome: Arthur Domingues Rios, Número USP: 13731751
// Nome: Ana Luíza Lasta Kodama, Número USP: 14651204

#include "registrodados.h"

#define CAMPO_INT 0
#define CAMPO_FLOAT 1

void criar_campoTamVar(FILE *fp, char **campo){ // função para ler e armazenar um campo de tamanho variavel

    char tempchar;
    int tamcampo = 0;

    fread(&tempchar, sizeof(char), 1, fp);

    while(tempchar != ',' && tempchar != '\n'){ // ler até encontrar uma virgula ou o final da linha 

        tamcampo++;
        fread(&tempchar, sizeof(char), 1, fp);

    }

    if(tamcampo == 0){  // caso o campo seja vazio, valor do campo é NULL

        *campo = NULL;
        return;
        
    }

    *campo = (char *)malloc((tamcampo + 1)*sizeof(char));

    if(*campo == NULL){

        printf("Falha no processamento do arquivo. ");
        exit(0);

    }

    fseek(fp, -(tamcampo + 1), SEEK_CUR); // retorna para o início do campo
    fread(*campo, sizeof(char), tamcampo, fp);
    (*campo)[tamcampo] = '\0';

    fseek(fp, 1, SEEK_CUR);

}

void criar_campoTamFixo(FILE *fp, void *campo, int tipo){ // função para ler e armazenar um campo de tamanho fixo

    char campotamfixo[30], tempchar;
    int i = 0;

    fread(&tempchar, sizeof(char), 1, fp);
    
    if(tempchar == ',' || tempchar == '\n'){ // caso seja ',' ou \n, o campo é vazio

        if(tipo == CAMPO_INT)
            *(int *)campo = -1;

        if(tipo == CAMPO_FLOAT)
            *(float *)campo = -1.0;

        return;

    }

    campotamfixo[i] = tempchar; // caso não seja vazio, a string começa a ser criada

    i++;

    while(fread(&tempchar, sizeof(char), 1, fp) == 1 && tempchar != ',' && tempchar != '\n'){ // termina de montar a string

        campotamfixo[i] = tempchar;
        i++;

    }

    campotamfixo[i] = '\0'; // adiciona o \0 no final

    if (tipo == CAMPO_INT)
        *(int *)campo = (int)strtol(campotamfixo, NULL, 10); // converte para inteiro

    if (tipo == CAMPO_FLOAT)
        *(float *)campo = strtof(campotamfixo, NULL); // converte para float
    
}


dados *criar_dado(FILE *filein){ // função para criar uma struct do registro de dados

    dados *regdados = malloc(sizeof(dados));

    if(regdados == NULL){

        printf("Falha no processamento do arquivo. ");
        exit(0);

    }

    regdados->removido = '0'; // armazena removido
    regdados->prox = -1; // armazena prox

    criar_campoTamFixo(filein, &(regdados->idAttack), CAMPO_INT); // le idAttack
    criar_campoTamFixo(filein, &(regdados->year), CAMPO_INT); // le year
    criar_campoTamFixo(filein, &(regdados->financialLoss), CAMPO_FLOAT); // le financialLoss

    criar_campoTamVar(filein, &(regdados->country)); // le country
    criar_campoTamVar(filein, &(regdados->attackType)); // le attackType
    criar_campoTamVar(filein, &(regdados->targetIndustry)); // le targetIndustry
    criar_campoTamVar(filein, &(regdados->defenseMechanism)); // le defenseMechanism

    regdados->tamanhoRegistro = sizeof(long long int) // prox
                                + sizeof(int) // idAttack
                                + sizeof(int) // year
                                + sizeof(int); // financialLoss

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

void escrever_campoTamVar(FILE *fp, char *campo, char keyword){ // função para escrever um campo de tamanho variavel no arquivo binario

    char delimcampo = '|';

    fwrite(&keyword, sizeof(char), 1, fp); // escreve keyword
    fwrite(campo, sizeof(char), strlen(campo), fp); // escreve campo
    fwrite(&delimcampo, sizeof(char), 1, fp); // escreve delimitador

}

void escrever_dado(FILE *filein, FILE *fileout){ // função para escrever um registro de dados no arquivo binario

    dados *regdados = criar_dado(filein);

    fwrite(&(regdados->removido), sizeof(char), 1, fileout); // escreve removido
    fwrite(&(regdados->tamanhoRegistro), sizeof(int), 1, fileout); // escreve tamanhoRegistro
    fwrite(&(regdados->prox), sizeof(long long int), 1, fileout); // escreve prox
    fwrite(&(regdados->idAttack), sizeof(int), 1, fileout); // escreve idAttack
    fwrite(&(regdados->year), sizeof(int), 1, fileout); // escreve year
    fwrite(&(regdados->financialLoss), sizeof(float), 1, fileout); // escrevee financialLoss

    if(regdados->country != NULL)
        escrever_campoTamVar(fileout, regdados->country, '1'); // escreve country

    if(regdados->attackType != NULL)
        escrever_campoTamVar(fileout, regdados->attackType, '2'); // escreve attackType

    if(regdados->targetIndustry != NULL)
        escrever_campoTamVar(fileout, regdados->targetIndustry, '3'); // escreve targetIndustry

    if(regdados->defenseMechanism != NULL)
        escrever_campoTamVar(fileout, regdados->defenseMechanism, '4'); // escreve defenseMechanism

    free(regdados->country);
    free(regdados->attackType);
    free(regdados->targetIndustry);
    free(regdados->defenseMechanism);

    free(regdados);

}

dados *ler_regdados(FILE *pos_registro){ // função para ler um registro de dados no arquivo binario (começando do segundo campo)

    dados *regdados = malloc(sizeof(dados)); // cria uma struct para armazenar um registro de dados

    if(regdados == NULL){

        printf("Falha no processamento do arquivo. ");
        exit(0);

    }

    char tempchar, tempstring[30];
    int i = 0;

    fread(&(regdados->tamanhoRegistro), sizeof(int), 1, pos_registro); // armazena o campo tamanhoRegistro
    fread(&(regdados->prox), sizeof(long long int), 1, pos_registro); // armazena o campo prox
    fread(&(regdados->idAttack), sizeof(int), 1, pos_registro); // armazena o campo idAttack
    fread(&(regdados->year), sizeof(int), 1, pos_registro); // armazena o campo year
    fread(&(regdados->financialLoss), sizeof(float), 1, pos_registro); // armazena o campo financialLoss

    fread(&tempchar, sizeof(char), 1, pos_registro);

    if(tempchar != '1'){ 

        (regdados->country) = malloc((sizeof(char))); // aloca o espaço para o ""
        strcpy(regdados->country, ""); // caso não tenha o codigo, o campo é vazio
        fseek(pos_registro, -1, SEEK_CUR);

    }

    else{

        fread(&tempchar, sizeof(char), 1, pos_registro);

        while(tempchar != '|'){ // enquanto não ler o delimitador "|"

            tempstring[i] = tempchar; // armazena os caracteres em uma string temporaria
            
            i++;

            fread(&tempchar, sizeof(char), 1, pos_registro);

        }

        tempstring[i] = '\0';

        (regdados->country) = malloc((i+1)*sizeof(char));

        if((regdados->country) == NULL){

            printf("Falha no processamento do arquivo. ");
            exit(0); 

        }

        strcpy((regdados->country), tempstring); // copia a string temporaria para o campo country

        i = 0;

    }

    fread(&tempchar, sizeof(char), 1, pos_registro);

    if(tempchar != '2'){ 

        (regdados->defenseMechanism) = malloc((sizeof(char))); // aloca o espaço para o ""
        strcpy(regdados->attackType, ""); // caso não tenha o codigo, o campo é vazio
        fseek(pos_registro, -1, SEEK_CUR);

    }

    else{

        fread(&tempchar, sizeof(char), 1, pos_registro);

        while(tempchar != '|'){ // enquanto não ler o delimitador "|"

            tempstring[i] = tempchar; // armazena os caracteres em uma string temporaria
            
            i++;

            fread(&tempchar, sizeof(char), 1, pos_registro);

        }

        tempstring[i] = '\0';

        (regdados->attackType) = malloc((i+1)*sizeof(char));

        if((regdados->attackType) == NULL){

            printf("Falha no processamento do arquivo. ");
            exit(0); 

        }

        strcpy((regdados->attackType), tempstring); // copia a string temporaria para o campo country

        i = 0;

    }

    fread(&tempchar, sizeof(char), 1, pos_registro);

    if(tempchar != '3'){ 
        
        (regdados->defenseMechanism) = malloc((sizeof(char))); // aloca o espaço para o ""
        strcpy(regdados->targetIndustry, ""); // caso não tenha o codigo, o campo é vazio
        fseek(pos_registro, -1, SEEK_CUR);

    }

    else{

        fread(&tempchar, sizeof(char), 1, pos_registro);

        while(tempchar != '|'){ // enquanto não ler o delimitador "|"

            tempstring[i] = tempchar; // armazena os caracteres em uma string temporaria
            
            i++;

            fread(&tempchar, sizeof(char), 1, pos_registro);

        }

        tempstring[i] = '\0';

        (regdados->targetIndustry) = malloc((i+1)*sizeof(char));

        if((regdados->targetIndustry) == NULL){

            printf("Falha no processamento do arquivo. ");
            exit(0); 

        }

        strcpy((regdados->targetIndustry), tempstring); // copia a string temporaria para o campo country

        i = 0;

    }

    fread(&tempchar, sizeof(char), 1, pos_registro);

    if(tempchar != '4'){ 

        (regdados->defenseMechanism) = malloc((sizeof(char))); // aloca o espaço para o ""
        strcpy(regdados->defenseMechanism, ""); // caso não tenha o codigo, o campo é vazio
        fseek(pos_registro, -1, SEEK_CUR);

    }

    else{

        fread(&tempchar, sizeof(char), 1, pos_registro);

        while(tempchar != '|'){ // enquanto não ler o delimitador "|"

            tempstring[i] = tempchar; // armazena os caracteres em uma string temporaria
            
            i++;

            fread(&tempchar, sizeof(char), 1, pos_registro);

        }

        tempstring[i] = '\0';

        (regdados->defenseMechanism) = malloc((i+1)*sizeof(char));

        if((regdados->defenseMechanism) == NULL){

            printf("Falha no processamento do arquivo. ");
            exit(0); 

        }

        strcpy((regdados->defenseMechanism), tempstring); // copia a string temporaria para o campo country

        i = 0;

    }
    
    return(regdados);

}

void atualizar_regdados(dados *regdados, char *nomecampo, void *valorcampo){ // função para atualizar os dados em um registro

    if(strcmp(nomecampo, "idAttack") == 0){

        regdados->idAttack = *((int *)valorcampo);

    }

    else if(strcmp(nomecampo, "year") == 0){

        regdados->year = *((int *)valorcampo);

    }

    else if(strcmp(nomecampo, "financialLoss") == 0){

        regdados->financialLoss = *((float *)valorcampo);

    }

    else if(strcmp(nomecampo, "country") == 0){

        free(regdados->country);
        regdados->country = malloc(strlen((char *)valorcampo) + 1); // aloca novamente com o tamanho do novo campo
        strcpy(regdados->country, (char *)valorcampo);

    }

    else if(strcmp(nomecampo, "attackType") == 0){

        free(regdados->attackType);
        regdados->attackType = malloc(strlen((char *)valorcampo) + 1);
        strcpy(regdados->attackType, (char *)valorcampo);

    }

    else if(strcmp(nomecampo, "targetIndustry") == 0){

        free(regdados->targetIndustry);
        regdados->targetIndustry = malloc(strlen((char *)valorcampo) + 1);
        strcpy(regdados->targetIndustry, (char *)valorcampo);

    }

    else if(strcmp(nomecampo, "defenseMechanism") == 0){

        free(regdados->defenseMechanism);
        regdados->defenseMechanism = malloc(strlen((char *)valorcampo) + 1);
        strcpy(regdados->defenseMechanism, (char *)valorcampo);

    }

    else{

        printf("Falha no processamento do arquivo. ");
        exit(0);  

    }

}

int atualizar_tamanho(dados *regdados){

    regdados->tamanhoRegistro = sizeof(long long int) // prox
                                + sizeof(int) // idAttack
                                + sizeof(int) // year
                                + sizeof(int); // financialLoss

    if((regdados->country)[0]) // adiciona apenas se o campo existir
        regdados->tamanhoRegistro = regdados->tamanhoRegistro + strlen(regdados->country) + 2; // +2 da keyword e do delimitador

    if(regdados->attackType[0])
        regdados->tamanhoRegistro = regdados->tamanhoRegistro + strlen(regdados->attackType) + 2; 

    if(regdados->targetIndustry[0])
        regdados->tamanhoRegistro = regdados->tamanhoRegistro + strlen(regdados->targetIndustry) + 2;

    if(regdados->defenseMechanism[0])
        regdados->tamanhoRegistro = regdados->tamanhoRegistro + strlen(regdados->defenseMechanism) + 2;

    return (regdados->tamanhoRegistro);

}

int get_tamanho(dados *regdados){

    return regdados->tamanhoRegistro;

}

int get_idAttack(dados *regdados){

    return regdados->idAttack;

}

int get_year(dados *regdados){

    return regdados->year;

}

float get_financialLoss(dados *regdados){

    return regdados->financialLoss;

}

char *get_country(dados *regdados){

    return regdados->country;

}

char *get_attackType(dados *regdados){

    return regdados->attackType;

}

char *get_targetIndustry(dados *regdados){

    return regdados->targetIndustry;

}

char *get_defenseMechanism(dados *regdados){

    return regdados->defenseMechanism;

}

void liberar_regdados(dados *regdados){

    if(regdados == NULL)
        return;

    if(regdados->country != NULL)
        free(regdados->country);

    if(regdados->attackType != NULL)
        free(regdados->attackType);

    if(regdados->targetIndustry != NULL)
        free(regdados->targetIndustry);

    if(regdados->defenseMechanism != NULL)
        free(regdados->defenseMechanism);

    free(regdados);

}
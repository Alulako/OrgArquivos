#include "funcoes.h"

#define CAMPO_INT 0
#define CAMPO_FLOAT 1

void modificar_cabecalho(FILE *fp){

    long long int pos_atual = ftell(fp);
    int tempint;

    fseek(fp, 9, SEEK_SET); // retorna ao campo proxByteOffset
    fwrite(&pos_atual, sizeof(long long int), 1, fp);

    fread(&tempint, sizeof(int), 1, fp); // armazena valor de registros não removidos
    tempint++; // incrementa este valor
    fwrite(&tempint, sizeof(int), 1, fp); // escreve por cima do valor anterior

    fseek(fp, pos_atual, SEEK_SET); // volta para o proximo byte offset

}

void funcao_lerRegistros(char *nomein, char *nomeout){

    FILE *filein = fopen(nomein, "r");
    FILE *fileout = fopen(nomeout, "wb");
    char tempchar;
    long long int pos_proxoffset;

    if(filein == NULL || fileout == NULL){

        printf("Falha no processamento do arquivo.");
        exit(1);

    }

    escrever_cabecalho();

    pos_proxoffset = ftell(fileout);

    fseek(fileout, 0, SEEK_SET); // retorna ao início do cabeçalho
    tempchar = '1';
    fwrite(&tempchar, sizeof(char), 1, fileout); // escreve 1 no campo status
    fseek(fileout, 8, SEEK_CUR); // pula para o campo proxByteOffset
    fwrite(&pos_proxoffset, sizeof(long long int), 1, fileout);
    fseek(fileout, 259, SEEK_CUR); // pula para o proximo byte offset

    while(1){

        fread(&tempchar, sizeof(char), 1, filein);

        if(tempchar == '\n') // caso o registro começe com uma linha vazia, quer dizer que chegou no final do csv
            break;

        fseek(filein, -1, SEEK_CUR);

        escrever_dado(filein, fileout);
        modificar_cabecalho(fileout);

    }

    fseek(fileout, 0, SEEK_SET);
    tempchar = '0';
    fwrite(&tempchar, sizeof(char), 1, fileout); // escreve '1' em status

    fclose(filein);
    fclose(fileout);

    //binarioNaTela();

}

void imprimir_campoTamVar(FILE *fp, char keyword){

    char tempchar;

    fread(&tempchar, sizeof(char), 1, fp);
    if(tempchar == keyword){ // checa se campo existe

        fread(&tempchar, sizeof(char), 1, fp);
        while(tempchar != '|'){

            printf("%c", tempchar);
            fread(&tempchar, sizeof(char), 1, fp);

        }

        printf("\n");

    }

    else{

        fseek(fp, -1, SEEK_CUR); // retorna uma posição caso registro não exista
        printf("NADA CONSTA\n");

    }

}

void imprimir_campoTamFixo(FILE *fp, int tipo){

    int tempint;
    float tempfloat;

    if(tipo == CAMPO_INT){

        fread(&tempint, sizeof(int), 1, fp);

        if(tempint != -1){

            printf("%d\n", tempint);
            return;

        }

    }

    if(tipo == CAMPO_FLOAT){

        fread(&tempfloat, sizeof(float), 1, fp);

        if(tempfloat != -1){

            printf("%.2f\n", tempfloat);
            return;

        }

    }

    printf("NADA CONSTA\n");

}

void funcao_imprimirRegistros(char *nomeout){

    FILE *fileout = fopen(nomeout, "rb");

    if(fileout == NULL){

        printf("Falha no processamento do arquivo.");
        exit(1);

    }

    int tempint;
    char tempchar;
    long long int pos_financialLoss, pos_attackType, pos_defenseMechanism;

    fseek(fileout, 0, SEEK_END); // pula para o final do arquivo
    long long int pos_final = ftell(fileout); // armazena o endereço do ultimo byte do arquivo

    fseek(fileout, 17, SEEK_SET); // volta para o inicío do arquivo e pula para o campo que diz o número de registros não removidos
    fread(&tempint, sizeof(int), 1, fileout);

    if(tempint == 0){ // caso não exista nenhum registro não removido

        printf("Registro inexistente.");
        return;

    }

    fseek(fileout, 255, SEEK_CUR); // pula para o primeiro byte do primeiro registro de dado

    while(ftell(fileout) <= pos_final){

    fread(&tempchar, sizeof(char), 1, fileout);
    if(tempchar == '0'){    // apenas irá imprimir o registro se ele não for logicamente removido

        fseek(fileout, 12, SEEK_CUR); // pula para o campo idAttack

        printf("IDENTIFICADOR DO ATAQUE: ");
        imprimir_campoTamFixo(fileout, CAMPO_INT); // lê e printa idAttack 

        printf("ANO EM QUE ATAQUE OCORREU: ");
        imprimir_campoTamFixo(fileout, CAMPO_INT); // lê e printa year

        pos_financialLoss = ftell(fileout);
        fseek(fileout, 4, SEEK_CUR); // pula o financialLoss

        printf("PAIS ONDE OCORREU O ATAQUE: ");
        imprimir_campoTamVar(fileout, '1'); // lê e printa country

        fread(&tempchar, sizeof(char), 1, fileout);
        if(tempchar == '2'){ // checa se attackType existe
            
            pos_attackType = ftell(fileout) - 1;

            fread(&tempchar, sizeof(char), 1, fileout);
            while(tempchar != '|') // loop para chegar no delimitador
                fread(&tempchar, sizeof(char), 1, fileout);

        }

        else{

            pos_attackType = -1; // caso o registro não exista
            fseek(fileout, -1, SEEK_CUR);

        }

        printf("SETOR DA INDUSTRIA QUE SOFREU O ATAQUE: ");
        imprimir_campoTamVar(fileout, '3');

        fread(&tempchar, sizeof(char), 1, fileout);
        if(tempchar == '4') // checa se defenseMechanism existe
            pos_defenseMechanism = ftell(fileout) - 1;

        else{

            pos_defenseMechanism = -1; // caso o registro não exista
            fseek(fileout, -1, SEEK_CUR);

        }

        if(pos_attackType == -1) // caso o campo attackType não exista
            printf("TIPO DE AMEACA A SEGURANCA CIBERNETICA: NADA CONSTA\n");

        else{

            fseek(fileout, -(ftell(fileout) - pos_attackType), SEEK_CUR);
            printf("TIPO DE AMEACA A SEGURANCA CIBERNETICA: ");
            imprimir_campoTamVar(fileout, '2');

        }        

        fseek(fileout, -(ftell(fileout) - pos_financialLoss), SEEK_CUR); // retorna para financialLoss

        printf("PREJUIZO CAUSADO PELO ATAQUE: ");
        imprimir_campoTamFixo(fileout, CAMPO_FLOAT); // lê e printa financialLoss

        if(pos_defenseMechanism == -1){ // caso defenseMechanism não exista

            printf("ESTRATEGIA DE DEFESA CIBERNETICA EMPREGADA PARA RESOLVER O PROBLEMA: NADA CONSTA\n");

            fseek(fileout, -24, SEEK_CUR); // retorna para a posição tamanhoRegistro, voltando 24 bytes
            fread(&tempint, sizeof(int), 1, fileout); // armazena o tamanho do registro
            fseek(fileout, tempint, SEEK_CUR); // avança para o proximo registro

        }

        else{

            printf("ESTRATEGIA DE DEFESA CIBERNETICA EMPREGADA PARA RESOLVER O PROBLEMA: ");
            imprimir_campoTamVar(fileout, '4');

        }

    }
        
    }

    fclose(fileout);

}

bool funcao_pesquisarRegistros(){

    // fazer 'n' pesquisas, e em cada pesquisa, checar nos registros de dados os resultados desta pesquisa

}


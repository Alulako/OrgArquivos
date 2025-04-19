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

    escrever_cabecalho(filein, fileout);

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
    fwrite(&tempchar, sizeof(char), 1, fileout); // escreve '0' em status

    fclose(filein);
    fclose(fileout);

    //binarioNaTela(nomeout);

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

void imprimir_registro(FILE *fp){

    int tempint;
    char tempchar;
    long long int pos_financialLoss, pos_attackType, pos_defenseMechanism;

    fseek(fp, 12, SEEK_CUR); // pula para o campo idAttack

    printf("IDENTIFICADOR DO ATAQUE: ");
    imprimir_campoTamFixo(fp, CAMPO_INT); // lê e printa idAttack 

    printf("ANO EM QUE ATAQUE OCORREU: ");
    imprimir_campoTamFixo(fp, CAMPO_INT); // lê e printa year

    pos_financialLoss = ftell(fp);
    fseek(fp, 4, SEEK_CUR); // pula o financialLoss

    printf("PAIS ONDE OCORREU O ATAQUE: ");
    imprimir_campoTamVar(fp, '1'); // lê e printa country

    fread(&tempchar, sizeof(char), 1, fp);
    if(tempchar == '2'){ // checa se attackType existe
        
        pos_attackType = ftell(fp) - 1;

        fread(&tempchar, sizeof(char), 1, fp);
        while(tempchar != '|') // loop para chegar no delimitador
            fread(&tempchar, sizeof(char), 1, fp);

    }

    else{

        pos_attackType = -1; // caso o registro não exista
        fseek(fp, -1, SEEK_CUR);

    }

    printf("SETOR DA INDUSTRIA QUE SOFREU O ATAQUE: ");
    imprimir_campoTamVar(fp, '3');

    fread(&tempchar, sizeof(char), 1, fp);
    if(tempchar == '4') // checa se defenseMechanism existe
        pos_defenseMechanism = ftell(fp) - 1;

    else{

        pos_defenseMechanism = -1; // caso o registro não exista
        fseek(fp, -1, SEEK_CUR);

    }

    if(pos_attackType == -1) // caso o campo attackType não exista
        printf("TIPO DE AMEACA A SEGURANCA CIBERNETICA: NADA CONSTA\n");

    else{

        fseek(fp, -(ftell(fp) - pos_attackType), SEEK_CUR);
        printf("TIPO DE AMEACA A SEGURANCA CIBERNETICA: ");
        imprimir_campoTamVar(fp, '2');

    }        

    fseek(fp, -(ftell(fp) - pos_financialLoss), SEEK_CUR); // retorna para financialLoss

    printf("PREJUIZO CAUSADO PELO ATAQUE: ");
    imprimir_campoTamFixo(fp, CAMPO_FLOAT); // lê e printa financialLoss

    if(pos_defenseMechanism == -1){ // caso defenseMechanism não exista

        printf("ESTRATEGIA DE DEFESA CIBERNETICA EMPREGADA PARA RESOLVER O PROBLEMA: NADA CONSTA\n");

        fseek(fp, -24, SEEK_CUR); // retorna para a posição tamanhoRegistro, voltando 24 bytes
        fread(&tempint, sizeof(int), 1, fp); // armazena o tamanho do registro
        fseek(fp, tempint, SEEK_CUR); // avança para o proximo registro

    }

    else{

        printf("ESTRATEGIA DE DEFESA CIBERNETICA EMPREGADA PARA RESOLVER O PROBLEMA: ");
        imprimir_campoTamVar(fp, '4');

    }

}

void funcao_imprimirRegistros(char *nomeout){

    FILE *fileout = fopen(nomeout, "rb");

    if(fileout == NULL){

        printf("Falha no processamento do arquivo.");
        exit(1);

    }

    int tempint;
    char tempchar;

    fseek(fileout, 0, SEEK_END); // pula para o final do arquivo
    long long int pos_final = ftell(fileout); // armazena o endereço do ultimo byte do arquivo

    fseek(fileout, 17, SEEK_SET); // volta para o inicío do arquivo e pula para o campo que diz o número de registros não removidos
    fread(&tempint, sizeof(int), 1, fileout);

    if(tempint == 0){ // caso não exista nenhum registro não removido

        printf("Registro inexistente.");
        return;

    }

    fseek(fileout, 255, SEEK_CUR); // pula para o primeiro byte do primeiro registro de dado

    while(ftell(fileout) < pos_final){

        fread(&tempchar, sizeof(char), 1, fileout);
        if(tempchar == '0')    // apenas irá imprimir o registro se ele não for logicamente removido
            imprimir_registro(fileout);

        else{

            fread(&tempint, sizeof(int), 1, fileout); // armazena o tamanho do registro
            fseek(fileout, tempint, SEEK_CUR); // avança para o proximo registro

        }
        
    }

    fclose(fileout);

}

bool filtrar_registroTamVar(FILE *fp, char keyword, char *valor){

    char tempchar, temp[30];
    bool campoexiste = false;

    fseek(fp, 1, SEEK_CUR); // pula para o campo tamanhoRegistro
    int tam;
    fread(&tam, sizeof(int), 1, fp); // armazena o tamanho do registro

    long long int prox = ftell(fp) + tam;

    fseek(fp, 20, SEEK_CUR); // avança para o espaço de campos de tamanho variavel

    while(ftell(fp) < prox){ // enquanto ainda estiver lendo o mesmo registro

        fread(&tempchar, sizeof(char), 1, fp);

        if(campoexiste == true){

            int i = 0;

            while(tempchar != '|'){

                temp[i] = tempchar;
                i++;
                fread(&tempchar, sizeof(char), 1, fp);

            }

            temp[i] = '\0';
            break;

        }

        if(tempchar == keyword) // campo encontrado
            campoexiste = true;

    }

    if(campoexiste == false)
        return false;

    if(strcmp(temp, valor) == 0)
        return true;

    else
        return false;

}

void funcao_pesquisarRegistros(char *nomein){

    FILE *filein = fopen(nomein, "rb");

    if(filein == NULL){

        printf("Falha no processamento do arquivo.");
        exit(1);

    }

    int n, m, tempint;
    char tempchar;
    float tempfloat;
    long long int pos_final;
    bool filtra, registroencontrado;

    fseek(filein, 17, SEEK_SET); // pula para a posição do campo que indica o número de registros não removidos

    fread(&tempint, sizeof(int), 1, filein);
    
    if(tempint == 0){ // caso nenhum registro não removido exista no arquivo

        printf("Registro inexistente.");
        return;

    }

    fseek(filein, 0, SEEK_END);

    pos_final = ftell(filein); // armazena o tamanho do arquivo indo para o final e lendo seu último endereço

    scanf("%d", &n);

    for(int i = 0; i < n; i++){

        scanf("%d", &m);

        char **campos = malloc(m*sizeof(char *));

        if(*campos == NULL){

            printf("Falha no processamento do arquivo.");
            exit(1); 

        }

        void **valores = malloc(m*sizeof(void *));

        if(*valores == NULL){

            printf("Falha no processamento do arquivo.");
            exit(1); 

        }
        
        for(int k = 0; k < m; k++){

            registroencontrado = false; // começa a pesquisa considerando que nenhum registro foi encontrado

            char nomecampo[17]; // maior nome possível é defenseMechanism, que possui 16 letras
            scanf(" %s", nomecampo); // lê o nome do campo
            campos[k] = (char *)malloc(strlen(nomecampo) + 1); // aloca espaço + 1 (por conta do \0)

            if(campos[k] == NULL){

                printf("Falha no processamento do arquivo.");
                exit(1); 
    
            }

            strcpy(campos[k], nomecampo);

            if(strcmp(campos[k], "idAttack") == 0 || strcmp(campos[k], "year") == 0){ // caso o nome seja idAttack ou year

                int *valor = malloc(sizeof(int));

                if(valor == NULL){

                    printf("Falha no processamento do arquivo.");
                    exit(1); 
        
                }

                scanf("%d", valor);
                valores[k] = valor;
                
            }

            else if(strcmp(campos[k], "financialLoss") == 0){ // caso o nome seja financialLoss

                float *valor = malloc(sizeof(int));

                if(valor == NULL){

                    printf("Falha no processamento do arquivo.");
                    exit(1); 
        
                }

                scanf("%f", valor);
                valores[k] = valor;
                
            }

            else if(strcmp(campos[k], "country") == 0 || strcmp(campos[k], "attackType") == 0 || 
            strcmp(campos[k], "targetIndustry") == 0 || strcmp(campos[k], "defenseMechanism") == 0){ // caso o nome seja country, attackType, targetIndustry ou defenseMechanism

                char valor[30];
                scan_quote_string(valor);
                valores[k] = (char *)malloc(strlen(valor) + 1);
            
                if(valores[k] == NULL){
            
                    printf("Falha no processamento do arquivo.");
                    exit(1); 
            
                }
            
                strcpy(valores[k], valor);
                
            }

            else{

                printf("Falha no processamento do arquivo.");
                exit(1);

            }

        }

        fseek(filein, 276, SEEK_SET); // pula o registro de cabeçalho

        while(ftell(filein) < pos_final){

            fread(&tempchar, sizeof(char), 1, filein);
            if(tempchar == '0'){  // apenas verificará o registro caso ele não esteja logicamente removido
                
                fseek(filein, -1, SEEK_CUR);
                long long int inicio_registro = ftell(filein);

                for(int j = 0; j < m; j++){

                    fseek(filein, inicio_registro, SEEK_SET); // retorna para o início do registro

                    if(strcmp(campos[j], "idAttack") == 0){

                        fseek(filein, 13, SEEK_CUR); // pula para o primeiro byte de idAttack
                        fread(&tempint, sizeof(int), 1, filein);

                        if(tempint == *(int *)valores[j])
                            filtra = true;

                        else{

                            filtra = false;
                            break;

                        }

                    }

                    if(strcmp(campos[j], "year") == 0){

                        fseek(filein, 17, SEEK_CUR); // pula para o primeiro byte de year
                        fread(&tempint, sizeof(int), 1, filein);

                        if(tempint == *(int *)valores[j])
                            filtra = true;

                        else{

                            filtra = false;
                            break;

                        }

                    }

                    if(strcmp(campos[j], "financialLoss") == 0){

                        fseek(filein, 13, SEEK_CUR); // pula para o primeiro byte de financialLoss
                        fread(&tempfloat, sizeof(float), 1, filein);

                        if(tempint == *(float *)valores[j])
                            filtra = true;

                        else{

                            filtra = false;
                            break;

                        }

                    }

                    if(strcmp(campos[j], "country") == 0){ 

                        filtra = filtrar_registroTamVar(filein, '1', valores[j]);

                        if(filtra == false)
                            break;

                    }

                    if(strcmp(campos[j], "attackType") == 0){

                        filtra = filtrar_registroTamVar(filein, '2', valores[j]);

                        if(filtra == false)
                            break;

                    }

                    if(strcmp(campos[j], "targetIndustry") == 0){

                        filtra = filtrar_registroTamVar(filein, '3', valores[j]);

                        if(filtra == false)
                            break;

                    }

                    if(strcmp(campos[j], "defenseMechanism") == 0){

                        filtra = filtrar_registroTamVar(filein, '4', valores[j]);

                        if(filtra == false)
                            break;

                    }

                }

                fseek(filein, (inicio_registro + 1), SEEK_SET);

                if(filtra == true){

                    registroencontrado = true; // como a variavel é true, ao menos um registro foi econtrado
                    imprimir_registro(filein); 

                }

                else{

                    fread(&tempint, sizeof(int), 1, filein); // armazena o tamanho do registro
                    fseek(filein, tempint, SEEK_CUR); // avança para o proximo registro

                }

            }
    
            else{ // else para caso o registro tenha sido removido logicamente
    
                fread(&tempint, sizeof(int), 1, filein); // armazena o tamanho do registro
                fseek(filein, tempint, SEEK_CUR); // avança para o proximo registro
    
            }
            
        }

        if(registroencontrado == false)
            printf("Registro inexistente.\n");

        printf("**********\n");

        for(int i = 0; i < m; i++){

            free(valores[i]);
            free(campos[i]);

        }

        free(valores);
        free(campos);

    }

    fclose(filein);

}


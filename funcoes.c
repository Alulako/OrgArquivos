#include "funcoes.h"
#include <ctype.h>

#define CAMPO_INT 0
#define CAMPO_FLOAT 1


void binarioNaTela(char *nomeArquivoBinario) { 

	unsigned long i, cs;
	unsigned char *mb;
	size_t fl;
	FILE *fs;
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}
	printf("%lf\n", (cs / (double) 100));
	free(mb);
	fclose(fs);
}

void scan_quote_string(char *str) {

	/*
	*	Use essa função para ler um campo string delimitado entre aspas (").
	*	Chame ela na hora que for ler tal campo. Por exemplo:
	*
	*	A entrada está da seguinte forma:
	*		nomeDoCampo "MARIA DA SILVA"
	*
	*	Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
	*		scanf("%s", str1); // Vai salvar nomeDoCampo em str1
	*		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
	*
	*/

	char R;

	while((R = getchar()) != EOF && isspace(R)); // ignorar espaços, \r, \n...

	if(R == 'N' || R == 'n') { // campo NULO
		getchar(); getchar(); getchar(); // ignorar o "ULO" de NULO.
		strcpy(str, ""); // copia string vazia
	} else if(R == '\"') {
		if(scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
			strcpy(str, "");
		}
		getchar(); // ignorar aspas fechando
	} else if(R != EOF){ // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então, pois deve ser algum inteiro ou algo assim...
		str[0] = R;
		scanf("%s", &str[1]);
	} else { // EOF
		strcpy(str, "");
	}
}

void modificar_cabecalho(FILE *fp){

    long long int pos_atual = ftell(fp);
    int tempint;

    fseek(fp, 9, SEEK_SET); // retorna ao campo proxByteOffset
    fwrite(&pos_atual, sizeof(long long int), 1, fp);

    fread(&tempint, sizeof(int), 1, fp); // armazena valor de registros não removidos
    tempint++; // incrementa este valor
    fseek(fp, -4, SEEK_CUR);
    fwrite(&tempint, sizeof(int), 1, fp); // escreve por cima do valor anterior

    fseek(fp, pos_atual, SEEK_SET); // volta para o proximo byte offset

}

void funcao_lerRegistros(char *nomein, char *nomeout){

    FILE *filein = fopen(nomein, "r");
    FILE *fileout = fopen(nomeout, "wb+");
    char tempchar;
    long long int pos_proxoffset;

    if(filein == NULL || fileout == NULL){

        printf("Falha no processamento do arquivo. ");
        exit(0);

    }

    escrever_cabecalho(filein, fileout);

    pos_proxoffset = ftell(fileout);

    fseek(fileout, 0, SEEK_SET); // retorna ao início do cabeçalho
    tempchar = '0';
    fwrite(&tempchar, sizeof(char), 1, fileout); // escreve 0 no campo status
    fseek(fileout, 8, SEEK_CUR); // pula para o campo proxByteOffset
    fwrite(&pos_proxoffset, sizeof(long long int), 1, fileout);
    fseek(fileout, 259, SEEK_CUR); // pula para o proximo byte offset

    while(1){

        if(fread(&tempchar, sizeof(char), 1, filein) == 0) // fread retornará zero caso chegue no final do csv
            break;

        fseek(filein, -1, SEEK_CUR);

        escrever_dado(filein, fileout);
        modificar_cabecalho(fileout);

    }

    fseek(fileout, 0, SEEK_SET);
    tempchar = '1';
    fwrite(&tempchar, sizeof(char), 1, fileout); // escreve '1' em status

    fclose(filein);
    fclose(fileout);

    binarioNaTela(nomeout);

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

    int tempint, tamanhoRegistro;
    char tempchar;
    long long int pos_financialLoss, pos_attackType, pos_defenseMechanism, pos_proxReg;

    fread(&tamanhoRegistro, sizeof(int), 1, fp);
    pos_proxReg = ftell(fp) + tamanhoRegistro;

    fseek(fp, 8, SEEK_CUR); // pula para o campo idAttack

    printf("IDENTIFICADOR DO ATAQUE: ");
    imprimir_campoTamFixo(fp, CAMPO_INT); // lê e printa idAttack 

    printf("ANO EM QUE O ATAQUE OCORREU: ");
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

        fseek(fp, pos_attackType, SEEK_SET); // retorna para attackType
        printf("TIPO DE AMEACA A SEGURANCA CIBERNETICA: ");
        imprimir_campoTamVar(fp, '2');

    }        

    fseek(fp, pos_financialLoss, SEEK_SET); // retorna para financialLoss

    printf("PREJUIZO CAUSADO PELO ATAQUE: ");
    imprimir_campoTamFixo(fp, CAMPO_FLOAT); // lê e printa financialLoss

    if(pos_defenseMechanism == -1) // caso defenseMechanism não exista
        printf("ESTRATEGIA DE DEFESA CIBERNETICA EMPREGADA PARA RESOLVER O PROBLEMA: NADA CONSTA\n");

    else{

        printf("ESTRATEGIA DE DEFESA CIBERNETICA EMPREGADA PARA RESOLVER O PROBLEMA: ");
        fseek(fp, pos_defenseMechanism, SEEK_SET);
        imprimir_campoTamVar(fp, '4');

    }

    printf("\n");

    fseek(fp, pos_proxReg, SEEK_SET); // vai para o proximo registro

}

void funcao_imprimirRegistros(char *nomeout){

    FILE *fileout = fopen(nomeout, "rb");

    if(fileout == NULL){

        printf("Falha no processamento do arquivo. ");
        exit(0);

    }

    int tempint;
    char tempchar;

    fseek(fileout, 0, SEEK_END); // pula para o final do arquivo
    long long int pos_final = ftell(fileout); // armazena o endereço do ultimo byte do arquivo

    fseek(fileout, 17, SEEK_SET); // volta para o inicío do arquivo e pula para o campo que diz o número de registros não removidos
    fread(&tempint, sizeof(int), 1, fileout);

    if(tempint == 0){ // caso não exista nenhum registro não removido

        printf("Registro inexistente.\n\n");
        printf("**********\n");
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

        printf("Falha no processamento do arquivo. ");
        exit(0);

    }

    int n, m, tempint;
    char tempchar;
    float tempfloat;
    long long int pos_final;
    bool filtra, registroencontrado;

    fseek(filein, 17, SEEK_SET); // pula para a posição do campo que indica o número de registros não removidos

    fread(&tempint, sizeof(int), 1, filein);
    
    if(tempint == 0){ // caso nenhum registro não removido exista no arquivo

        printf("Registro inexistente.\n\n");
        printf("**********\n");
        return;

    }

    fseek(filein, 0, SEEK_END);

    pos_final = ftell(filein); // armazena o tamanho do arquivo indo para o final e lendo seu último endereço

    scanf("%d", &n);

    for(int i = 0; i < n; i++){

        scanf("%d", &m);

        char **campos = malloc(m*sizeof(char *));

        if(campos == NULL){

            printf("Falha no processamento do arquivo. ");
            exit(0); 

        }

        void **valores = malloc(m*sizeof(void *));

        if(valores == NULL){

            printf("Falha no processamento do arquivo. ");
            exit(0); 

        }
        
        for(int k = 0; k < m; k++){

            registroencontrado = false; // começa a pesquisa considerando que nenhum registro foi encontrado

            char nomecampo[17]; // maior nome possível é defenseMechanism, que possui 16 letras
            scanf(" %s", nomecampo); // lê o nome do campo
            campos[k] = (char *)malloc(strlen(nomecampo) + 1); // aloca espaço + 1 (por conta do \0)

            if(campos[k] == NULL){

                printf("Falha no processamento do arquivo. ");
                exit(0); 
    
            }

            strcpy(campos[k], nomecampo);

            if(strcmp(campos[k], "idAttack") == 0 || strcmp(campos[k], "year") == 0){ // caso o nome seja idAttack ou year

                int *valor = malloc(sizeof(int));

                if(valor == NULL){

                    printf("Falha no processamento do arquivo. ");
                    exit(0); 
        
                }

                scanf("%d", valor);
                valores[k] = valor;
                
            }

            else if(strcmp(campos[k], "financialLoss") == 0){ // caso o nome seja financialLoss

                float *valor = malloc(sizeof(float));

                if(valor == NULL){

                    printf("Falha no processamento do arquivo. ");
                    exit(0); 
        
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
            
                    printf("Falha no processamento do arquivo. ");
                    exit(0); 
            
                }
            
                strcpy(valores[k], valor);
                
            }

            else{

                printf("Falha no processamento do arquivo. ");
                exit(0);

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

                        fseek(filein, 21, SEEK_CUR); // pula para o primeiro byte de financialLoss
                        fread(&tempfloat, sizeof(float), 1, filein);

                        if(tempfloat == *(float *)valores[j])
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
            printf("Registro inexistente.\n\n");

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


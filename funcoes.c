// Nome: Arthur Domingues Rios, Número USP: 13731751
// Nome: Ana Luíza Lasta Kodama, Número USP: 14651204

#include "funcoes.h"
#include <ctype.h> // por conta da função isspace() utilizada em scan_quote_string

#define CAMPO_INT 0
#define CAMPO_FLOAT 1


void binarioNaTela(char *nomeArquivoBinario){ 

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

void scan_quote_string(char *str){

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

void modificar_status(FILE *fp, bool abrindo){ // função para modificar o campo status do cabeçalho quando abrir o arquivo para escrita

    long long int pos_atual = ftell(fp);
    char status;

    fseek(fp, 0, SEEK_SET); // vai para o campo status do registro de cabeçalho

    if(abrindo == true){ // o arquivo está abrindo

        status = '0';
        fwrite(&status, sizeof(char), 1, fp);

    }

    else{ // o arquivo está fechando

        status = '1';
        fwrite(&status, sizeof(char), 1, fp);

    }

    fseek(fp, pos_atual, SEEK_SET); // retorna para a posição que estava

}

void modificar_cabecalho(FILE *fp){ // função para modificar campos do cabeçalho

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

void funcao_lerRegistros(char *nomein, char *nomeout){ // FUNCIONALIDADE 1

    FILE *filein = fopen(nomein, "r");
    FILE *fileout = fopen(nomeout, "wb+");

    char tempchar;
    long long int pos_proxoffset;

    if(filein == NULL || fileout == NULL){

        printf("Falha no processamento do arquivo. ");
        exit(0);

    }

    escrever_cabecalho(filein, fileout); // escreve o cabeçalho

    pos_proxoffset = ftell(fileout); // armazena o proximo byte offset

    modificar_status(fileout, true);

    fseek(fileout, 9, SEEK_SET); // pula para o campo proxByteOffset
    fwrite(&pos_proxoffset, sizeof(long long int), 1, fileout);
    fseek(fileout, 259, SEEK_CUR); // pula para o proximo byte offset

    while(1){ // escreve os registros de dados até chegar no final do csv

        if(fread(&tempchar, sizeof(char), 1, filein) == 0) // fread retornará zero caso chegue no final do csv
            break;

        fseek(filein, -1, SEEK_CUR); // retorna o que foi lido

        escrever_dado(filein, fileout); // escreve o registro de dados
        modificar_cabecalho(fileout); // modifica campos do cabeçalho

    }

    modificar_status(fileout, false);

    fclose(filein);
    fclose(fileout);

    binarioNaTela(nomeout);

}

void imprimir_campoTamVar(FILE *fp, char keyword){ // função feita para imprimir um campo de tamanho variavel

    char tempchar;

    fread(&tempchar, sizeof(char), 1, fp);
    if(tempchar == keyword){ // checa se campo existe

        fread(&tempchar, sizeof(char), 1, fp);
        while(tempchar != '|'){ // imprime na tela até encontrar o delimitador

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

void imprimir_campoTamFixo(FILE *fp, int tipo){ // função feita para imprimir um campo de tamanho fixo

    int tempint;
    float tempfloat;

    if(tipo == CAMPO_INT){ // caso o campo seja um inteiro

        fread(&tempint, sizeof(int), 1, fp);

        if(tempint != -1){ // caso o campo não seja vazio

            printf("%d\n", tempint);
            return;

        }

    }

    if(tipo == CAMPO_FLOAT){ // caso o campo seja um float

        fread(&tempfloat, sizeof(float), 1, fp);

        if(tempfloat != -1){ // caso o campo não seja vazio

            printf("%.2f\n", tempfloat);
            return;

        }

    }

    printf("NADA CONSTA\n");

}

void imprimir_registro(FILE *fp){ // função feita para imprimir um registro, formatado da maneira que foi pedida

    int tempint, tamanhoRegistro;
    char tempchar;
    long long int pos_financialLoss, pos_attackType, pos_defenseMechanism, pos_proxReg;

    fread(&tamanhoRegistro, sizeof(int), 1, fp); // armazena o tamanho do registro
    pos_proxReg = ftell(fp) + tamanhoRegistro; // armazena a posição do proximo registro (posição atual + tamanho do registro)

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
        
        pos_attackType = ftell(fp) - 1; // caso exista, armazena sua posição

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
        pos_defenseMechanism = ftell(fp) - 1; // caso exista, armazena sua posição

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
        fseek(fp, pos_defenseMechanism, SEEK_SET); // retorna defenseMechanism
        imprimir_campoTamVar(fp, '4');

    }

    printf("\n");

    fseek(fp, pos_proxReg, SEEK_SET); // vai para o proximo registro

}

void funcao_imprimirRegistros(char *nomeout){ // FUNCIONALIDADE 2

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

    while(ftell(fileout) < pos_final){ // enquanto a posição do ponteiro estiver antes da posição final do arquivo

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

bool filtrar_registroTamVar(FILE *fp, char keyword, char *valor){ // função para filtrar um campo de tamanho variavel de um registro de dados

    char tempchar, temp[30];
    bool campoexiste = false;

    fseek(fp, 1, SEEK_CUR); // pula para o campo tamanhoRegistro
    int tam;
    fread(&tam, sizeof(int), 1, fp); // armazena o tamanho do registro

    long long int prox = ftell(fp) + tam; // armazena posição do proximo registro

    fseek(fp, 20, SEEK_CUR); // avança para o espaço de campos de tamanho variavel

    while(ftell(fp) < prox){ // enquanto ainda estiver lendo o mesmo registro

        fread(&tempchar, sizeof(char), 1, fp);

        if(campoexiste == true){ // caso o campo exista (checagem feita abaixo)

            int i = 0;

            while(tempchar != '|'){

                temp[i] = tempchar;
                i++;
                fread(&tempchar, sizeof(char), 1, fp);

            }

            temp[i] = '\0';
            break;

        }

        if(tempchar == keyword) // caso encontra a keyword, o campo é encontrado
            campoexiste = true;

    }

    if(campoexiste == false) // caso o campo não exista
        return false;

    if(strcmp(temp, valor) == 0) // caso o valor pesquisado seja o mesmo do campo
        return true;

    else // caso o valor pesquisado seja diferente
        return false;

}

void remover_registro(FILE *fp, long long int inicio_registro){ // função que remove logicamente o registro

    int tempint, tamanho_registro;
    char tempchar;
    long long int pos_atual, prox;

    pos_atual = ftell(fp); // armazena a posição atual

    fseek(fp, 1, SEEK_SET); // vai para o campo topo do registro de cabeçalho
    fread(&prox, sizeof(long long int), 1, fp); // armazena o topo em prox
    fseek(fp, 1, SEEK_SET); // retorna ao início de topo
    fwrite(&inicio_registro, sizeof(long long int), 1, fp); // escreve o byte offset do registro removido

    fseek(fp, 8, SEEK_CUR); // vai para o campo nroRegArq
    fread(&tempint, sizeof(int), 1, fp);
    tempint--;
    fseek(fp, -4, SEEK_CUR); // retorna para o início de nroRegArq
    fwrite(&tempint, sizeof(int), 1, fp); // decrementa 1 no número de registros não removidos
    fread(&tempint, sizeof(int), 1, fp); // armazena o valor de nroRegRem
    tempint++;
    fseek(fp, -4, SEEK_CUR); // retorna para o inicio de nroRegRem
    fwrite(&tempint, sizeof(int), 1, fp); // incrementa 1 no número de registros removidos


    fseek(fp, inicio_registro, SEEK_SET); // retorna para o inicio do registro
    tempchar = '1';
    fwrite(&tempchar, sizeof(char), 1, fp); // marca o registro como logicamente removido
    fread(&tamanho_registro, sizeof(int), 1, fp); // armazena o tamanho do registro
    fwrite(&prox, sizeof(long long int), 1, fp); // escreve o que estava em topo

    fseek(fp, (tamanho_registro - 8), SEEK_CUR); // pula para o proximo registro

}

void inc_nroRegArq(FILE *fp){ // função para incrementar o campo nroRegArq

    long long pos = ftell(fp); // armazena a posição atual
    fseek(fp, 17, SEEK_SET); // vai para o campo nroRegArq do cabeçalho
    int x;  
    fread(&x, 4, 1, fp); // faz a leitura do campo
    x++;
    fseek(fp, -4, SEEK_CUR); // retorna para o inicio do campo
    fwrite(&x, 4, 1, fp); // escreve o valor do campo incrementado 
    fseek(fp, pos, SEEK_SET); // volta para a posição atual

}

void dec_nroRegRem(FILE *fp){ // função para decrementar o campo nroRegRem

    long long pos = ftell(fp); // armazena a posição atual
    fseek(fp, 21, SEEK_SET); // vai para o campo nroRegRem
    int x;  
    fread(&x, 4, 1, fp); // faz a leitura do campo
    x--;
    fseek(fp, -4, SEEK_CUR); // retorna para o inicio do campo
    fwrite(&x, 4, 1, fp); // escreve o valor do campo decrementado
    fseek(fp, pos, SEEK_SET); // volta para a posição atual

}

void inserir_registro(FILE *fp, int idAttack, int year, float financialLoss, 
    char *country, char *attackType, char *targetIndustry, char *defenseMechanism){ // função feita para inserir um registro

    int tamanhoRegistro = sizeof(long long int) + 2*sizeof(int) + sizeof(float); // calculo do tamanho do registro

    if(country[0])
        tamanhoRegistro = tamanhoRegistro + strlen(country) + 2; // + 2 por conta da keyword e do delimitador

    if(attackType[0])
        tamanhoRegistro = tamanhoRegistro + strlen(attackType) + 2;

    if(targetIndustry[0]) 
        tamanhoRegistro = tamanhoRegistro + strlen(targetIndustry) + 2;

    if(defenseMechanism[0])
        tamanhoRegistro = tamanhoRegistro + strlen(defenseMechanism) + 2;   

    fseek(fp, 1, SEEK_SET); // vai para o campo topo do cabeçalho

    long long int topo; 
    fread(&topo, sizeof(long long int), 1, fp); // armazena o campo topo

    long long anterior = -1, atual = topo, encontrado = -1, proxencontrado = -1;

    int tamanhoEncontrado = 0;

    while(atual != -1){ // percorre a lista até o fim

        fseek(fp, atual, SEEK_SET); // vai até o proximo registro

        char remov;
        int tamanhoRemovido; 
        long long int prox;

        fread(&remov, sizeof(char), 1, fp); // armazena o campo removido

        if(remov == '0'){ // caso ocorra algum erro e o registro não esteja logicamente removido

            printf("Falha no processamento do arquivo."); 
            exit(0); 

        }

        fread(&tamanhoRemovido, sizeof(int), 1, fp); // armazena o tamanho do registro removido
        fread(&prox, sizeof(long long int), 1, fp); // armazena o proximo registro da lista

        if(tamanhoRemovido >= tamanhoRegistro){ // caso o novo registro consiga ser inserido no espaço

            encontrado = atual;
            proxencontrado = prox; 
            tamanhoEncontrado = tamanhoRemovido; 

            break;

        }

        anterior = atual; // armazena o valor do atual 
        atual = prox; // armazena o valor do proximo registro

    }

    long long int posicaoEscrita;
    bool espacoEncontrado = (encontrado != -1);

    if(espacoEncontrado){ // caso algum espaço reutilizavel tenha sido encontrado, a lista deve ser atualizada
                                    
        posicaoEscrita = encontrado; // armazena a posição em que vai escrever

        if(anterior == -1){ // caso o espaço encontrado tenha sido o primeiro da lista

            fseek(fp, 1, SEEK_SET); // vai para o campo topo do cabeçalho
            fwrite(&proxencontrado, sizeof(long long int), 1, fp); // escreve o endereço do segundo registro da lista

        }

        else{ // caso contrario

            fseek(fp, anterior + 5, SEEK_SET); // vai para o campo prox do anterior ao encontrado
            fwrite(&proxencontrado, sizeof(long long int), 1, fp); // escreve o endereço do proximo do registro encontrado
        
        }

        dec_nroRegRem(fp);

    } 
    
    else{ // caso nenhum espaço seja reutilizavel para o registro

        fseek(fp, 9, SEEK_SET); // vai para o campo proxByteOffset
        fread(&posicaoEscrita, sizeof(long long int), 1, fp); // armazena o endereço do proximo byte offset do arquivo
        
    }

    fseek(fp, posicaoEscrita, SEEK_SET); // vai para a posição que o registro será inserido
    char removido = '0'; 
    fwrite(&removido, sizeof(char), 1, fp); // escreve o campo removido

    if(espacoEncontrado) // caso algum espaço da lista tenha sido encontrado
        fwrite(&tamanhoEncontrado, 4, 1, fp); // escreve o tamanho do espaço no campo tamanhoRegistro

    else // caso contrario
        fwrite(&tamanhoRegistro, 4, 1, fp); // escreve o tamanho do novo registro

    long long int prox = -1; 
    fwrite(&prox, 8, 1, fp); // escreve o campo prox

    fwrite(&idAttack, 4, 1, fp); // escreve o campo idAttack
    fwrite(&year, 4, 1, fp); // escreve o campo year
    fwrite(&financialLoss, 4, 1, fp); // escreve o campo financialLoss

    if(country[0]) // caso o campo não seja nulo
        escrever_campoTamVar(fp, country, '1'); // escreve o campo country

    if(attackType[0]) 
        escrever_campoTamVar(fp, attackType, '2'); // escreve o campo attackType

    if(targetIndustry[0])  
        escrever_campoTamVar(fp, targetIndustry, '3'); // escreve o campo targetIndustry

    if(defenseMechanism[0])
        escrever_campoTamVar(fp, defenseMechanism, '4'); // escreve o campo defenseMechanism

    if(espacoEncontrado){ // caso algum espaço tenha sido encontrado

        int sobra = tamanhoEncontrado - tamanhoRegistro; // calcula a quantidade de lixo

        for(int i = 0; i < sobra; i++){ 

            char l='$';
            fwrite(&l, sizeof(char), 1, fp); // escreve o lixo

        }

    }

    if(espacoEncontrado) // caso o espaço tenha sido encontrado
        inc_nroRegArq(fp);  // apenas incrementa nroRegArq

    else{ // caso contrário

        long long fim = ftell(fp); // armazena a posição do final do arquivo
        fseek(fp, 9, SEEK_SET);  // vai ate o campo proxByteOffset
        fwrite(&fim, sizeof(long long int), 1, fp); // escreve a posição do final do arquivo
        inc_nroRegArq(fp); // incrementa nroRegArq

    }

}

void atualizar_registro(FILE *fp, long long int inicio_registro, int p, 
    char **camposatt, void **valoresatt){ // função feita para atualizar um registro

    char tempchar;
    int tempint, tamanho_registro, novotamanho;
    float tempfloat;

    dados *regdados = ler_regdados(fp); // cria uma struct de registro de dados lendo o registro no arquivo binario

    tamanho_registro = get_tamanho(regdados); // armazena o tamanho do registro
    
    for(int j = 0; j < p; j++)
        atualizar_regdados(regdados, camposatt[j], valoresatt[j]); // realiza a atualização do registro na struct

    novotamanho = atualizar_tamanho(regdados); // atualiza e armazena o tamanho do registro atualizado

    fseek(fp, inicio_registro, SEEK_SET); // retorna ao inicio do registro no arquivo binario

    if(novotamanho <= tamanho_registro){ // caso o registro atualizado consiga ser escrito no mesmo espaço

        tempchar = '0';
        fwrite(&tempchar, sizeof(char), 1, fp); // escreve removido

        fwrite(&tamanho_registro, sizeof(int), 1, fp); // escreve o tamanho (que neste caso é o tamanho do espaço)

        long long int prox = -1;
        fwrite(&prox, sizeof(long long int), 1, fp); // escreve prox

        tempint = get_idAttack(regdados);
        fwrite(&tempint, sizeof(int), 1, fp); // escreve idAttack

        tempint = get_year(regdados);
        fwrite(&tempint, sizeof(int), 1, fp); // escreve year

        tempfloat = get_financialLoss(regdados);
        fwrite(&tempfloat, sizeof(float), 1, fp); // escreve financialLoss

        char *campovar;

        campovar = get_country(regdados);

        if(campovar[0]) // caso country exista, escreve 
            escrever_campoTamVar(fp, campovar, 1);

        campovar = get_attackType(regdados);

        if(campovar[0]) // caso attackType exista, escreve 
            escrever_campoTamVar(fp, campovar, 2);

        campovar = get_targetIndustry(regdados);

        if(campovar[0]) // caso targetIndustry exista, escreve 
            escrever_campoTamVar(fp, campovar, 3);

        campovar = get_defenseMechanism(regdados);

        if(campovar[0]) // caso defenseMechanism exista, escreve 
            escrever_campoTamVar(fp, campovar, 4);

        int sobra = tamanho_registro - novotamanho; // calcula sobra do lixo

        for(int i = 0; i < sobra; i++){ 

            char l='$';
            fwrite(&l, sizeof(char), 1, fp); // escreve o lixo

        }
        
    }

    else{ // caso não seja possivel escrever o registro no mesmo espaço

        remover_registro(fp, inicio_registro); // remove o registro

        inserir_registro(fp, get_idAttack(regdados), get_year(regdados), get_financialLoss(regdados), 
        get_country(regdados), get_attackType(regdados), get_targetIndustry(regdados), 
        get_defenseMechanism(regdados)); // insere novamente

    }

    liberar_regdados(regdados); // libera o espaço de regdados alocado

}

void pesquisa_generalizada(FILE *fp, int funcionalidade){ /* função feita para realizar uma pesquisa e 
                                                           imprimir, remover ou atualizar um registro */

    // funcionalidade = 3 (imprimir registro)
    // funcionalidade = 4 (remover registro)
    // funcionalidade = 6 (atualizar registro)

    int n, m, p, tempint;
    char tempchar;
    float tempfloat;
    long long int pos_final;
    bool filtra, registroencontrado, idpesquisado;

    char **camposatt = NULL;
    void **valoresatt = NULL;

    idpesquisado = false; // inicializa considerando que o idAttack não será pesquisado

    fseek(fp, 17, SEEK_SET); // pula para a posição do cabeçalho que indica o número de registros não removidos

    fread(&tempint, sizeof(int), 1, fp);
    
    if(tempint == 0){ // caso nenhum registro não removido exista no arquivo

        if(funcionalidade == 3){ // caso esteja realizando uma filtragem nos registros

            printf("Registro inexistente.\n\n");
            printf("**********\n");

        }

        return;

    }

    fseek(fp, 0, SEEK_END); // vai para o final do binario

    pos_final = ftell(fp); // armazena a posição do seu final

    scanf("%d", &n); // lê numero de pesquisas que serão feitas

    for(int i = 0; i < n; i++){

        scanf("%d", &m); // lê quantidade de campos que serão pesquisados

        char **campos = malloc(m*sizeof(char *));

        if(campos == NULL){

            printf("Falha no processamento do arquivo. ");
            exit(0); 

        }

        void **valores = malloc(m*sizeof(void *)); // void, pois podem ser 3 tipos de campos diferentes, int, float e string

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

            strcpy(campos[k], nomecampo); // copia o nome do campo para o espaço alocado

            if(strcmp(campos[k], "idAttack") == 0 || strcmp(campos[k], "year") == 0){ // caso o nome seja idAttack ou year

                int *valor = malloc(sizeof(int));

                if(valor == NULL){

                    printf("Falha no processamento do arquivo. ");
                    exit(0); 
        
                }

                scanf("%d", valor);
                valores[k] = valor; // armazena valor no espaço alocado
                
            }

            else if(strcmp(campos[k], "financialLoss") == 0){ // caso o nome seja financialLoss

                float *valor = malloc(sizeof(float));

                if(valor == NULL){

                    printf("Falha no processamento do arquivo. ");
                    exit(0); 
        
                }

                scanf("%f", valor);
                valores[k] = valor; // armazena valor no espaço alocado
                
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
            
                strcpy(valores[k], valor); // armazena valor em espaço alocado
                
            }

            else{

                printf("Falha no processamento do arquivo. ");
                exit(0);

            }

        }

        if(funcionalidade == 6){ // caso esteja sendo realizada uma atualização

            scanf("%d", &p); // lê quantidade de campos que serão atualizados

            camposatt = malloc(p*sizeof(char *));

            if(camposatt == NULL){

                printf("Falha no processamento do arquivo. ");
                exit(0); 

            }

            valoresatt = malloc(p*sizeof(void *)); // void, pois podem ser 3 tipos de campos diferentes, int, float e string

            if(valoresatt == NULL){

                printf("Falha no processamento do arquivo. ");
                exit(0); 

            }
        
            for(int k = 0; k < p; k++){

                char nomecampo[17]; // maior nome possível é defenseMechanism, que possui 16 letras
                scanf(" %s", nomecampo); // lê o nome do campo
                camposatt[k] = (char *)malloc(strlen(nomecampo) + 1); // aloca espaço + 1 (por conta do \0)

                if(camposatt[k] == NULL){

                    printf("Falha no processamento do arquivo. ");
                    exit(0); 
        
                }

                strcpy(camposatt[k], nomecampo); // copia o nome do campo para o espaço alocado

                if(strcmp(camposatt[k], "idAttack") == 0 || strcmp(camposatt[k], "year") == 0){ // caso o nome seja idAttack ou year

                    char *checkint;
                    int *valor = malloc(sizeof(int));

                    if(valor == NULL){

                        printf("Falha no processamento do arquivo. ");
                        exit(0); 
            
                    }

                    if(!(checkint[0])) // caso o campo seja nulo
                        *valor = -1;

                    else // caso não seja
                        *valor = strtol(checkint, NULL, 10); // converte para inteiro

                    valoresatt[k] = valor; // armazena valor no espaço alocado
                    
                }

                else if(strcmp(camposatt[k], "financialLoss") == 0){ // caso o nome seja financialLoss

                    char *checkfloat;
                    float *valor = malloc(sizeof(float));

                    if(valor == NULL){

                        printf("Falha no processamento do arquivo. ");
                        exit(0); 
            
                    }

                    if(!(checkfloat[0])) // caso o campo seja nulo
                        *valor = -1.0;

                    else // caso não seja
                        *valor = strtof(checkfloat, NULL); // converte para float

                    valoresatt[k] = valor; // armazena valor no espaço alocado
                    
                }

                else if(strcmp(camposatt[k], "country") == 0 || strcmp(camposatt[k], "attackType") == 0 || 
                strcmp(camposatt[k], "targetIndustry") == 0 || strcmp(camposatt[k], "defenseMechanism") == 0){ // caso o nome seja country, attackType, targetIndustry ou defenseMechanism

                    char valor[30];
                    scan_quote_string(valor); 
                    valoresatt[k] = (char *)malloc(strlen(valor) + 1);
                
                    if(valoresatt[k] == NULL){
                
                        printf("Falha no processamento do arquivo. ");
                        exit(0); 
                
                    }
                
                    strcpy(valoresatt[k], valor); // armazena valor em espaço alocado
                    
                }

                else{

                    printf("Falha no processamento do arquivo. ");
                    exit(0);

                }

           }

        }

        fseek(fp, 276, SEEK_SET); // pula o registro de cabeçalho

        while(ftell(fp) < pos_final){ // enquanto a posição do ponteiro for menor que a ultima posição

            fread(&tempchar, sizeof(char), 1, fp);
            if(tempchar == '0'){  // apenas verificará o registro caso ele não esteja logicamente removido
                
                fseek(fp, -1, SEEK_CUR);
                long long int inicio_registro = ftell(fp); // armazena posição do inicio do registro

                for(int j = 0; j < m; j++){

                    fseek(fp, inicio_registro, SEEK_SET); // retorna para o início do registro

                    if(strcmp(campos[j], "idAttack") == 0){

                        fseek(fp, 13, SEEK_CUR); // pula para o primeiro byte de idAttack
                        fread(&tempint, sizeof(int), 1, fp);

                        if(tempint == *(int *)valores[j]){ // caso o valor do campo seja igual ao esperado

                            filtra = true;
                            idpesquisado = true;

                        }

                        else{ 

                            filtra = false;
                            break; // caso não seja o valor esperado, já pula para o proximo registro

                        }

                    }

                    if(strcmp(campos[j], "year") == 0){

                        fseek(fp, 17, SEEK_CUR); // pula para o primeiro byte de year
                        fread(&tempint, sizeof(int), 1, fp);

                        if(tempint == *(int *)valores[j]) // caso o valor do campo seja igual ao esperado
                            filtra = true;

                        else{

                            filtra = false;
                            break; // caso não seja o valor esperado, já pula para o proximo registro

                        }

                    }

                    if(strcmp(campos[j], "financialLoss") == 0){

                        fseek(fp, 21, SEEK_CUR); // pula para o primeiro byte de financialLoss
                        fread(&tempfloat, sizeof(float), 1, fp);

                        if(tempfloat == *(float *)valores[j]) // caso o valor do campo seja igual ao esperado
                            filtra = true;

                        else{

                            filtra = false;
                            break; // caso não seja o valor esperado, já pula para o proximo registro

                        }

                    }

                    if(strcmp(campos[j], "country") == 0){ 

                        filtra = filtrar_registroTamVar(fp, '1', valores[j]); // chama função para checar se o valor é o esperado

                        if(filtra == false)
                            break; // caso não seja o valor esperado, já pula para o proximo registro

                    }

                    if(strcmp(campos[j], "attackType") == 0){

                        filtra = filtrar_registroTamVar(fp, '2', valores[j]); // chama função para checar se o valor é o esperado

                        if(filtra == false)
                            break; // caso não seja o valor esperado, já pula para o proximo registro

                    }

                    if(strcmp(campos[j], "targetIndustry") == 0){

                        filtra = filtrar_registroTamVar(fp, '3', valores[j]); // chama função para checar se o valor é o esperado

                        if(filtra == false)
                            break; // caso não seja o valor esperado, já pula para o proximo registro

                    }

                    if(strcmp(campos[j], "defenseMechanism") == 0){

                        filtra = filtrar_registroTamVar(fp, '4', valores[j]); // chama função para checar se o valor é o esperado

                        if(filtra == false)
                            break; // caso não seja o valor esperado, já pula para o proximo registro

                    }

                }

                fseek(fp, (inicio_registro + 1), SEEK_SET); // retorna ao inicio do registro e pula o campo removido

                if(filtra == true){ // caso a pesquisa tenha passado em todos os casos

                    if(funcionalidade == 3){ // caso esteja realizado uma filtragem

                        registroencontrado = true; // como a variavel é true, ao menos um registro foi encontrado
                        imprimir_registro(fp);

                    }

                    if(funcionalidade == 4){ // caso esteja realizando uma remoção

                        remover_registro(fp, inicio_registro);

                    }

                    if(funcionalidade == 6){ // caso esteja realizando uma atualização

                        atualizar_registro(fp, inicio_registro, p, camposatt, valoresatt);

                    }

                    if(idpesquisado == true){ // caso o idAttack tenha sido utilizado na pesquisa, ele não continua lendo o arquivo

                        idpesquisado = false;
                        break;
                    
                    }
                    
                }

                else{

                    fread(&tempint, sizeof(int), 1, fp); // armazena o tamanho do registro
                    fseek(fp, tempint, SEEK_CUR); // avança para o proximo registro

                }

            }
    
            else{ // else para caso o registro tenha sido removido logicamente
    
                fread(&tempint, sizeof(int), 1, fp); // armazena o tamanho do registro
                fseek(fp, tempint, SEEK_CUR); // avança para o proximo registro
    
            }
            
        }

        if(registroencontrado == false && funcionalidade == 3) // caso nenhum registro tenha sido encontrado na pesquisa da filtragem
            printf("Registro inexistente.\n\n");

        if(funcionalidade == 3) // caso esteja realizando uma filtragem dos registros
            printf("**********\n");

        for(int i = 0; i < m; i++){

            free(valores[i]);
            free(campos[i]);

        }

        free(valores);
        free(campos);

        if(funcionalidade == 6){ // caso esteja realizando uma atualização

            for(int i = 0; i < p; i++){

                free(valoresatt[i]);
                free(camposatt[i]);

            }   


            free(valoresatt);
            free(camposatt);

        }

    }

}

void funcao_pesquisarRegistros(char *nomein){ // FUNCIONALIDADE 3

    FILE *filein = fopen(nomein, "rb");

    if(filein == NULL){

        printf("Falha no processamento do arquivo. ");
        exit(0);

    }

    pesquisa_generalizada(filein, 3);

    fclose(filein);

}

void funcao_removerRegistros(char *nomein){ // FUNCIONALIDADE 4

    FILE *filein = fopen(nomein, "rb+");

    if(filein == NULL){

        printf("Falha no processamento do arquivo. ");
        exit(0);

    }

    modificar_status(filein, true);

    pesquisa_generalizada(filein, 4);

    modificar_status(filein, false);

    fclose(filein);

    binarioNaTela(nomein);

}

void funcao_inserirRegistros(char *nomein){ // FUNCIONALIDADE 5

    FILE *filein = fopen(nomein, "rb+");

    if(filein == NULL){ 

        printf("Falha no processamento do arquivo."); 
        exit(0); 
    
    }

    modificar_status(filein, true); // modifica o campo status no cabeçalho

    int n;  
    
    scanf("%d", &n); // leitura de quantas inserções serão feitas

    for(int k = 0; k < n; k++){

        int idAttack, year;
        float financialLoss;
        char checkidAttack[20], checkyear[10], checkfinancialLoss[20], country[50], attackType[50], targetIndustry[50], defenseMechanism[50];

        scan_quote_string(checkidAttack); // leitura dos campos
        scan_quote_string(checkyear);
        scan_quote_string(checkfinancialLoss);
        scan_quote_string(country);
        scan_quote_string(attackType);
        scan_quote_string(targetIndustry);
        scan_quote_string(defenseMechanism);

        if(!(checkidAttack[0])) // caso o campo seja vazio
            idAttack = -1;

        else // caso não seja
            idAttack = strtol(checkidAttack, NULL, 10); // converte para inteiro

        if(!(checkyear[0])) // caso o campo seja vazio
            year = -1;

        else // caso não seja
            year = strtol(checkyear, NULL, 10); 

        if(!(checkfinancialLoss[0])) // caso o campo seja vazio
            financialLoss = -1.0;

        else // caso não seja
            financialLoss = strtof(checkfinancialLoss, NULL); // converte para float

        inserir_registro(filein, idAttack, year, financialLoss, country, attackType, targetIndustry, defenseMechanism);

    }

    modificar_status(filein, false); // modifica o campo status no cabeçalho

    fclose(filein);

    binarioNaTela(nomein);

}

void funcao_atualizarRegistros(char *nomein){ // FUNCIONALIDADE 6

    FILE *filein = fopen(nomein, "rb+");

    if(filein == NULL){

        printf("Falha no processamento do arquivo. ");
        exit(0);

    }

    modificar_status(filein, true);

    pesquisa_generalizada(filein, 6);

    modificar_status(filein, false);

    fclose(filein);

    binarioNaTela(nomein);

}

void funcao_criarArvoreB(char *nomein, char *nomearvb){ // FUNCIONALIDADE 7



}

void funcao_pesquisarRegistrosArvoreB(char *nomein, char *nomearvb){ // FUNCIONALIDADE 8



}

void funcao_removerArvoreB(char *nomein, char *nomearvb){ // FUNCIONALIDADE 9 (NÃO IMPLEMENTADA)

    printf("FUNCIONALIDADE NÃO IMPLEMENTADA\n");
    return;

}

void funcao_inserirArvoreB(char *nomein, char *nomearvb){ // FUNCIONALIDADE 10



}

void funcao_atualizarArvoreB(char *nomein, char *nomearvb){ // FUNCIONALIDADE 11



}
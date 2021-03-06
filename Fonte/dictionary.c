#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef FMACROS // garante que macros.h não seja reincluída
   #include "macros.h"
#endif
///
#ifndef FTYPES // garante que types.h não seja reincluída
  #include "types.h"
#endif
////
#ifndef FMISC // garante que misc.h não seja reincluída
  #include "misc.h"
#endif

#ifndef FSQLCOMMANDS // garante que sqlcommands.h não seja reincluída
   #include "sqlcommands.h"
#endif

#ifndef FBUFFER // garante que buffer.h não seja reincluída
   #include "buffer.h"
#endif

/* ----------------------------------------------------------------------------------------------
    Objetivo:   Verificação de existência de um arquivo.
    Parametros: Nome do arquivo.
    Retorno:    INT 1 (existe) , 0 (não existe).
   ---------------------------------------------------------------------------------------------*/

int fileExists(const char* filename){
    char directory[LEN_DB_NAME * 2];
    strcpy(directory, connected.db_directory);
    strcat(directory, filename);
    FILE* fptr = fopen(directory, "r");

    if (fptr == NULL) return 0;

    fclose(fptr);
    return 1;
}
/* ----------------------------------------------------------------------------------------------
    Objetivo:   Verifica a existência do atributo antes de adicionar na tabela
    Parametros: Nome da tabela, coluna C.
    Retorno:    INT
                SUCCESS,
                PARAMETER_ERROR_1
   ---------------------------------------------------------------------------------------------*/

int attributeExists(char *nomeTabela, column *c){
    int x, count;
    fs_objects objeto;
    memset(&objeto, 0, sizeof(fs_objects));
    tp_table *tabela;
    tp_buffer *bufferpoll;
    column *aux = NULL;
    column *pagina = NULL;

    if(initAttributes(&objeto, &tabela, &bufferpoll, nomeTabela) != SUCCESS)
        return PARAMETER_ERROR_1;

    for(x = 0; colocaTuplaBuffer(bufferpoll, x, tabela, objeto) == SUCCESS; x++);

    pagina = getPage(bufferpoll, tabela, objeto, 0);
    if(pagina == NULL) pagina = getPage(bufferpoll, tabela, objeto, 1);

    if(pagina != NULL){
        count = 0;
        for(x = 0; x < objeto.qtdCampos; x++){
            if (!pagina[x].nomeCampo) continue;
            for(aux = c; aux != NULL; aux=aux->next) {
                if (!aux->nomeCampo) continue;
                if(objcmp(pagina[x].nomeCampo, aux->nomeCampo) == 0)
                    count++;
            }
        }
        if(count != objeto.qtdCampos){
            free(pagina);
            free(bufferpoll);
            free(tabela);
            return PARAMETER_ERROR_1;
        }
    }

    free(pagina);
    free(bufferpoll);
    free(tabela);
    return SUCCESS;
}
//////
int verifyTableName(char *nomeTabela) {
    FILE *dicionario;
    char tupla[TABLE_NAME_SIZE], directory[LEN_DB_NAME * 2];

    strcpy(directory, connected.db_directory);
    strcat(directory, "fs_object.dat");

    if((dicionario = fopen(directory,"a+b")) == NULL) return ERROR_OPEN_FILE;

    //Percorro o arquivo lendo os nomes das tabelas existentes
    while (fread(tupla, sizeof(char), TABLE_NAME_SIZE, dicionario) > 0){
        if(!strcmp(tupla, nomeTabela)){ // Verifica se o nome dado pelo usuario existe no dicionario de dados.
            fclose(dicionario);
            return 1;
        }
        //Ignoro os outros campos do fs_objects
        fseek(dicionario, sizeof(int) * 2 + sizeof(char) * FILE_NAME_SIZE, SEEK_CUR);
    }

    fclose(dicionario);
    return 0;
}
////
int quantityTable(){

    FILE *dicionario;
    int codTbl = 0;

    char directory[LEN_DB_NAME*2];
    strcpy(directory, connected.db_directory);
    strcat(directory, "fs_object.dat");

    if((dicionario = fopen(directory,"a+b")) == NULL)
        return ERROR_OPEN_FILE;

    while(fgetc (dicionario) != EOF){
        fseek(dicionario, -1, 1);

        codTbl++; // Conta quantas vezes é lido uma tupla no dicionario.

        fseek(dicionario, 48, 1);
    }

    fclose(dicionario);

    return codTbl;
}
////
int returnsSizeValueField(char *nomeCampo, table  *tab) {

    int tam = 0;

    tp_table *temp = tab->esquema;

    while(temp != NULL) {

       if (objcmp(nomeCampo,temp->nome) == 0) {
            tam = temp->tam;

            return tam;
       }

       temp = temp->next;
    }

    return tam;
}
////

char returnsSizeTypeField(char *nomeCampo, table  *tab) {

    char tipo = 0;

    tp_table *temp = tab->esquema;

    while(temp != NULL) {

       if (objcmp(nomeCampo,temp->nome) == 0) {
            tipo = temp->tipo;

            return tipo;
       }

       temp = temp->next;
    }

    return tipo;
}
////
/* ----------------------------------------------------------------------------------------------
    Objetivo:   Retorna vetor de esquemas com todos os atributos chaves (PK, FK e NPK)
    Parametros: Objeto da tabela.
    Retorno:    Vetor de esquemas vetEsqm
   ---------------------------------------------------------------------------------------------*/

tp_table *searchAttributeskeys(fs_objects objeto){
    FILE *schema;
    int cod = 0, chave, i = 0;
    char *tupla = (char *)malloc(sizeof(char) * 109);
    tp_table *esquema = (tp_table *)malloc(sizeof(tp_table)*objeto.qtdCampos);
    tp_table *vetEsqm = (tp_table *)malloc(sizeof(tp_table)*objeto.qtdCampos);
    memset(vetEsqm, 0, sizeof(tp_table)*objeto.qtdCampos);
    memset(esquema, 0, sizeof(tp_table)*objeto.qtdCampos);

    char directory[LEN_DB_NAME*2];
    strcpy(directory, connected.db_directory);
    strcat(directory, "fs_schema.dat");

    if((schema = fopen(directory, "a+b")) == NULL){
        printf("ERROR: could not read schema.\n");
        free(tupla);
		free(esquema);
		free(vetEsqm);
        return ERROR_OPEN_SCHEMA;
    }

    while((fgetc (schema) != EOF) && i < objeto.qtdCampos){ // Varre o arquivo ate encontrar todos os campos com o codigo da tabela.
        fseek(schema, -1, 1);

        if(fread(&cod, sizeof(int), 1, schema)){ // Le o codigo da tabela.
            if(cod == objeto.cod){
                fread(tupla, sizeof(char), FIELD_NAME_SIZE, schema);
                strcpylower(esquema[i].nome,tupla);                  // Copia dados do campo para o esquema.

                fread(&esquema[i].tipo , sizeof(char), 1 , schema);
                fread(&esquema[i].tam  , sizeof(int) , 1 , schema);
                fread(&chave, sizeof(int) , 1 , schema);
                vetEsqm[i].tipo = esquema[i].tipo;
                vetEsqm[i].tam = esquema[i].tam;
                fread(tupla, sizeof(char), TABLE_NAME_SIZE, schema);
                strcpylower(esquema[i].tabelaApt,tupla);

                fread(tupla, sizeof(char), FIELD_NAME_SIZE, schema);
                strcpylower(esquema[i].attApt,tupla);

                strcpylower(vetEsqm[i].tabelaApt, esquema[i].tabelaApt);
                strcpylower(vetEsqm[i].attApt, esquema[i].attApt);
                strcpylower(vetEsqm[i].nome, esquema[i].nome);
                vetEsqm[i].chave = chave;

                i++;
            } else {
                fseek(schema, 109, 1);
            }
        }
    }
    free(tupla);
	free(esquema);

	fclose(schema);

    return vetEsqm;
}
/* ----------------------------------------------------------------------------------------------
    Objetivo:   Copia todas as informações menos a tabela com nome NomeTabela, que será removida.
    Parametros: Nome da tabela que será removida do object.dat.
    Retorno:    INT
                SUCCESS,
                ERROR_OPEN_FILE
   ---------------------------------------------------------------------------------------------*/

int searchObjectFile(char *nomeTabela){
    int teste        = 0,
        cont         = 0,
        achou        = 0,
        tamanhoTotal = sizeof(fs_objects);

    char *table = (char *)malloc(sizeof(char) * tamanhoTotal);
    FILE *dicionario, *fp;

    char directory[LEN_DB_NAME*2];
    strcpy(directory, connected.db_directory);
    strcat(directory, "fs_object.dat");

    if((dicionario = fopen(directory,"a+b")) == NULL) {
        free(table);
        return ERROR_OPEN_FILE;
    }

    strcpy(directory, connected.db_directory);
    strcat(directory, "fs_nobject.dat");

    if((fp = fopen(directory, "a+b")) == NULL) {
        free(table);
        return ERROR_OPEN_FILE;
    }

    fseek(dicionario, 0, SEEK_SET);
    fseek(fp, 0, SEEK_SET);

    while(cont < quantityTable()){
        fread(table, sizeof(char), tamanhoTotal, dicionario);
        teste = exchangeFileObject(nomeTabela, table);

        if(teste == 0){                                         //NÃO É IGUAL
            fseek(fp, 0, SEEK_END);
            fwrite(table, sizeof(char), tamanhoTotal, fp);
        } else if(achou != 1){                                    //É IGUAL E NÃO TINHA SIDO DESCOBERTO.
            achou = 1;
            fread(table, sizeof(char), 0, dicionario);
        }
        cont++;
    }

    fclose(fp);
    fclose(dicionario);

    char directoryex[LEN_DB_NAME*4];
    strcpy(directoryex, connected.db_directory);
    strcat(directoryex, "fs_object.dat");

    remove(directoryex);

    strcpy(directoryex, "mv ");
    strcat(directoryex, connected.db_directory);
    strcat(directoryex, "fs_nobject.dat ");
    strcat(directoryex, connected.db_directory);
    strcat(directoryex, "fs_object.dat");

    system(directoryex);

    free(table);
    return SUCCESS;
}
//
fs_objects readObject(char *nTabela) {
    int cod;
    fs_objects objeto;
    char directory[LEN_DB_NAME * 2];
    char *tupla = (char *) malloc(sizeof (char) * TABLE_NAME_SIZE);
    memset(tupla, '\0', TABLE_NAME_SIZE);
    strcpy(directory, connected.db_directory);
    strcat(directory, "fs_object.dat");
    FILE *dicionario = fopen(directory, "a+b"); // Abre o dicionario de dados

    if (!verifyTableName(nTabela)) {
        printf("ERROR: relation \"%s\" was not found.\n", nTabela);
        if (dicionario) fclose(dicionario);
        free(tupla);
        return objeto;
    }

    if (dicionario == NULL) {
        printf("ERROR: data dictionary was not found.\n\n");
        free(tupla);
        return objeto;
    }

    while (fgetc(dicionario) != EOF) {
        fseek(dicionario, -1, SEEK_CUR);
        fread(tupla, sizeof(char), TABLE_NAME_SIZE , dicionario); //Lê somente o nome da tabela
        if (strcmp(tupla, nTabela) == 0) { // Verifica se o nome dado pelo usuario existe no dicionario de dados.
            strcpy(objeto.nome, tupla);
            fread(&cod, sizeof (int), 1, dicionario);   // Copia valores referentes a tabela pesquisada para a estrutura.
            objeto.cod = cod;
            fread(tupla, sizeof (char), TABLE_NAME_SIZE, dicionario);
            strcpylower(objeto.nArquivo, tupla);
            fread(&cod, sizeof (int), 1, dicionario);
            objeto.qtdCampos = cod;

            free(tupla);
            fclose(dicionario);
            return objeto;
        }
        fseek(dicionario, 28, 1); // Pula a quantidade de caracteres para a proxima verificacao(4B do codigo, 20B do nome do arquivo e 4B da quantidade de campos).
    }
    free(tupla);
    fclose(dicionario);

    return objeto;
}
//
// LEITURA DE DICIONARIO E ESQUEMA
tp_table *readSchema (fs_objects objeto){
    FILE *schema;
    int i = 0, cod = 0;
    char *tupla = (char *)malloc(sizeof(char)*FIELD_NAME_SIZE);
    memset(tupla, 0, FIELD_NAME_SIZE);
    char *tuplaT = (char *)malloc(sizeof(char)*TABLE_NAME_SIZE+1);
    memset(tuplaT, 0, TABLE_NAME_SIZE+1);

    tp_table *esquema = (tp_table *)malloc(sizeof(tp_table)*(objeto.qtdCampos+1)); // Aloca esquema com a quantidade de campos necessarios.
    memset(esquema, 0, (objeto.qtdCampos+1)*sizeof(tp_table));
    for (i = 0; i < objeto.qtdCampos+1; i++) {
        esquema[i].next = NULL;
    }


    i = 0;
    if(esquema == NULL){
        free(tupla);
        free(tuplaT);
        return ALLOCATION_ERROR;
    }

    char directory[LEN_DB_NAME*2];
    strcpy(directory, connected.db_directory);
    strcat(directory, "fs_schema.dat");

    schema = fopen(directory, "a+b"); // Abre o arquivo de esquemas de tabelas.

    if (schema == NULL){
        free(tupla);
        free(tuplaT);
        free(esquema);
        return ERROR_OPEN_SCHEMA;
    }

    while((fgetc (schema) != EOF) && (i < objeto.qtdCampos)){ // Varre o arquivo ate encontrar todos os campos com o codigo da tabela.
        fseek(schema, -1, 1);

        if(fread(&cod, sizeof(int), 1, schema)){ // Le o codigo da tabela.
            if(cod == objeto.cod){ // Verifica se o campo a ser copiado e da tabela que esta na estrutura fs_objects.

                fread(tupla, sizeof(char), FIELD_NAME_SIZE, schema);
                strcpylower(esquema[i].nome,tupla);                  // Copia dados do campo para o esquema.

                fread(&esquema[i].tipo, sizeof(char),1,schema);
                fread(&esquema[i].tam, sizeof(int),1,schema);
                fread(&esquema[i].chave, sizeof(int),1,schema);

                fread(tuplaT, sizeof(char), TABLE_NAME_SIZE, schema);
                strcpylower(esquema[i].tabelaApt,tuplaT);

                fread(tupla, sizeof(char), FIELD_NAME_SIZE, schema);
                strcpylower(esquema[i].attApt,tupla);

                if (i > 0)
                    esquema[i-1].next = &esquema[i];

                i++;
            } else {
                fseek(schema, 109, 1); // Pula a quantidade de caracteres para a proxima verificacao (40B do nome, 1B do tipo e 4B do tamanho,4B da chave, 20B do nome da Tabela Apontada e 40B do atributo apontado).
            }
        }
    }
    esquema[i].next = NULL;

    free(tupla);
    free(tuplaT);
    fclose(schema);

    return esquema;
}
////
int tupleSize(tp_table *esquema, fs_objects objeto) {// Retorna o tamanho total da tupla da tabela.

    int qtdCampos = objeto.qtdCampos, i, tamanhoGeral = 0;

    if(qtdCampos){ // Lê o primeiro inteiro que representa a quantidade de campos da tabela.
        for(i = 0; i < qtdCampos; i++)
            tamanhoGeral += esquema[i].tam; // Soma os tamanhos de cada campo da tabela.
    }

    return tamanhoGeral;
}
////


// CRIA TABELA
table *initTable(char *nome){
    if(verifyTableName(nome)){   // Se o nome já existir no dicionario, retorna erro.
        return INVALID_TABLE_NAME;
    }

    table *t = (table *)malloc(sizeof(table)*1);
    memset(t,0,sizeof(table));
    strcpylower(t->nome,nome); // Inicia a estrutura de tabela com o nome da tabela.
    t->esquema = NULL; // Inicia o esquema da tabela com NULL.
    return t; // Retorna estrutura para criação de uma tabela.
}
////
////
int addField(table *t,char *nomeCampo, char tipoCampo, int tamanhoCampo, int tChave, char *tabelaApt, char *attApt){
    tp_table *e = NULL;

    // Se a estrutura passada for nula, retorna erro.
    if(t == NULL) return ERROR_STRUCTURE_TABLE_NULL;

    tp_table *aux;
    if(t->esquema == NULL){ // Se o campo for o primeiro a ser adicionado, adiciona campo no esquema.
        e = (tp_table *)malloc(sizeof(tp_table));

        memset(e, 0, sizeof(tp_table));
        if (e == NULL) return MALLOC_FAILED;

        e->next = NULL;

        int n = strlen(nomeCampo);

        //Considerar o /0
        if (n >= FIELD_NAME_SIZE) n = FIELD_NAME_SIZE - 1;

        strncpylower(e->nome, nomeCampo,n); // Copia nome do campo passado para o esquema
        e->tipo = tipoCampo; // Copia tipo do campo passado para o esquema
        e->tam = tamanhoCampo; // Copia tamanho do campo passado para o esquema
        e->chave = tChave; // Copia tipo de chave passado para o esquema

        if(strlen(tabelaApt) >= 1)
            strcpylower(e->tabelaApt, tabelaApt); //Copia a Tabela Refenciada da FK de chave passado para o esquema;

        if(strlen(attApt) >= 1)
            strcpylower(e->attApt, attApt); //Copia o Atributo Refenciado da FK de chave passado para o esquema

        t->esquema = e;

        // return t; // Retorna a estrutura
        return SUCCESS;
    } else {
        for(aux = t->esquema; aux != NULL; aux = aux->next){ // Anda até o final da estrutura de campos.
            if(aux->next == NULL){ // Adiciona um campo no final.
                e = (tp_table *)malloc(sizeof(tp_table));
                memset(e, 0, sizeof(*e));

                if (e == NULL) return MALLOC_FAILED;

                e->next = NULL;

                int n = strlen(nomeCampo)+1;

                if (n > FIELD_NAME_SIZE) {
                    n = FIELD_NAME_SIZE;
                }

                strncpylower(e->nome, nomeCampo,n); // Copia nome do campo passado para o esquema
                e->tipo = tipoCampo; // Copia tipo do campo passado para o esquema
                e->tam = tamanhoCampo; // Copia tamanho do campo passado para o esquema
                e->chave = tChave; // Copia tipo de chave passado para o esquema

                strcpylower(e->tabelaApt, tabelaApt); //Copia a Tabela Refenciada da FK de chave passado para o esquema;

                strcpylower(e->attApt, attApt); //Copia o Atributo Refenciado da FK de chave passado para o esquema

                aux->next = e; // Faz o campo anterior apontar para o campo inserido.
                // return t;
                return SUCCESS;
            }
        }
    }

    // return t; //Retorna estrutura atualizada.
    return SUCCESS;
}

///
int finishTable(table *t){
    if(t == NULL) return PARAMETER_ERROR_1;

    tp_table *aux;
    FILE *esquema, *dicionario, *arquivo;
    int codTbl = quantityTable() + 1, qtdCampos = 0; // Conta a quantidade de tabelas já no dicionario e soma 1 no codigo dessa nova tabela.
    char nomeArquivo[FILE_NAME_SIZE];
    memset(nomeArquivo, 0, FILE_NAME_SIZE);

    char directory[LEN_DB_NAME * 2];
    strcpy(directory, connected.db_directory);
    strcat(directory, "fs_schema.dat");

    if((esquema = fopen(directory,"a+b")) == NULL)
        return ERROR_OPEN_FILE;

    for(aux = t->esquema; aux != NULL; aux = aux->next){ // Salva novos campos no esquema da tabela, fs_schema.dat

        fwrite(&codTbl         ,sizeof(codTbl)         ,1,esquema);  //Código Tabela
        fwrite(&aux->nome      ,sizeof(aux->nome)      ,1,esquema);  //Nome campo
        fwrite(&aux->tipo      ,sizeof(aux->tipo)      ,1,esquema);  //Tipo campo
        fwrite(&aux->tam       ,sizeof(aux->tam)       ,1,esquema);  //Tamanho campo
        fwrite(&aux->chave     ,sizeof(aux->chave)     ,1,esquema);  //Chave do campo
        fwrite(&aux->tabelaApt ,sizeof(aux->tabelaApt) ,1,esquema);  //Tabela Apontada
        fwrite(&aux->attApt    ,sizeof(aux->attApt)    ,1,esquema);  //Atributo apontado.

        qtdCampos++; // Soma quantidade total de campos inseridos.
    }

    fclose(esquema);

    strcpy(directory, connected.db_directory);
    strcat(directory, "fs_object.dat");

    if((dicionario = fopen(directory,"a+b")) == NULL)
        return ERROR_OPEN_FILE;

    strcpylower(nomeArquivo, t->nome);
    strcat(nomeArquivo, ".dat\0");
    strcat(t->nome, "\0");
    // Salva dados sobre a tabela no dicionario.
    fwrite(&t->nome,sizeof(t->nome),1,dicionario);
    fwrite(&codTbl,sizeof(codTbl),1,dicionario);
    fwrite(&nomeArquivo,sizeof(nomeArquivo),1,dicionario);
    fwrite(&qtdCampos,sizeof(qtdCampos),1,dicionario);

    //Cria arquivo da tabela
    strcpy(directory, connected.db_directory);
    strcat(directory, nomeArquivo);
    if((arquivo = fopen(directory, "a")) == NULL) return ERROR_OPEN_FILE;
    fclose(arquivo);
    fclose(dicionario);
    return SUCCESS;
}
////
// INSERE NA TABELA
column *insertValue(table  *tab, column *c, char *nomeCampo, char *valorCampo) {
    int i;

    column *aux;
    column *e = NULL;

    if(c == NULL){ // Se o valor a ser inserido é o primeiro, adiciona primeiro campo.

        e = (column *)malloc(sizeof(column));

        if (e == NULL)    {
            return ALLOCATION_ERROR;
        }

        memset(e, 0, sizeof(column));

        int tam = returnsSizeValueField(nomeCampo, tab);
        char tipo = returnsSizeTypeField(nomeCampo,tab);

        int nTam = strlen(valorCampo);

        if (tipo == 'S') {
            nTam = tam;
        }

        e->valorCampo = (char *)malloc(sizeof(char) * (nTam+1));

        if (e->valorCampo == NULL) {
            free(e);
            return ALLOCATION_ERROR;
        }

        int n = strlen(nomeCampo)+1;

        /**
         * Verifica se o nome ultrapassa o limite, se sim trunca
         */
        if (n > FIELD_NAME_SIZE) {
           n = FIELD_NAME_SIZE;
           printf("WARNING: field name exceeded the size limit and was truncated.\n");
        }


        strncpylower(e->nomeCampo, nomeCampo, n-1);

        n = strlen(valorCampo);

        if (n > tam && tipo == 'S') {
            n = tam;
            printf("WARNING: value of column \"%s\" exceeded the size limit and was truncated.\n", nomeCampo);
        }

        for(i=0; i < n; i++) e->valorCampo[i] = valorCampo[i];
            e->valorCampo[i] = '\0';

        //strncpy(e->valorCampo, valorCampo,n);

        e->next = NULL;
        c = e;
        return c;
    } else {
        for(aux = c; aux != NULL; aux = aux->next) { // Anda até o final da lista de valores a serem inseridos e adiciona um novo valor.
            if(aux->next == NULL){

                e = (column *)malloc(sizeof(column));

                if (e == NULL) {
                    return ALLOCATION_ERROR;
                }

                memset(e, 0, sizeof(column));

                int tam = returnsSizeValueField(nomeCampo, tab);
                char tipo = returnsSizeTypeField(nomeCampo,tab);

                int nTam = strlen(valorCampo);

                if (tipo == 'S') {
                    nTam = tam;
                }

                e->valorCampo = (char *) malloc (sizeof(char) * (nTam+1));

                if (e->valorCampo == NULL) {
                    free(e);
                    return ALLOCATION_ERROR;
                }

                e->next = NULL;

                int n = strlen(nomeCampo)+1;

                /**
                 * Verifica se o nome do campo ultrapassa o limite, se sim trunca
                 */
                if (n > FIELD_NAME_SIZE) {
                   n = FIELD_NAME_SIZE;
                   printf("WARNING: field name exceeded the size limit and was truncated.\n");
                }

                strncpylower(e->nomeCampo, nomeCampo, n-1);

                //strncpy(e->nomeCampo, nomeCampo,n);

                n = strlen(valorCampo);

                if (n > tam && tipo == 'S') {
                    n = tam;
                    printf("WARNING: value of column \"%s\"exceeded the size limit and was truncated.\n", nomeCampo);
                }

                for(i=0; i < n; i++) e->valorCampo[i] = valorCampo[i];
                e->valorCampo[i] = '\0';

                //strncpy(e->valorCampo, valorCampo,n);
                aux->next = e;
                return c;
            }
        }
    }

    if (e) free(e);
    return INSERT_ERROR;
}
//////
/*------------------------------------------------------------------------------------------
Objetivo: Mostrar as tabelas do banco de dados ou, em específico, os atributos de uma tabela
------------------------------------------------------------------------------------------*/

void printTable(char *tbl){
	if(tbl == NULL){     //mostra todas as tabelas do banco
		FILE *dicionario;
		printf("		List of Relations\n");
		char *tupla = (char *)malloc(sizeof(char)*TABLE_NAME_SIZE);

		char directory[LEN_DB_NAME*2];
    	strcpy(directory, connected.db_directory);
    	strcat(directory, "fs_object.dat");

		if((dicionario = fopen(directory,"a+b")) == NULL){
			free(tupla);
			printf("ERROR: cannot open file\n");
			return;
		}

		printf(" %-10s | %-15s | %-10s | %-10s\n", "Schema", "Name", "Type", "Owner");
		printf("------------+-----------------+------------+-------\n");
		int i=0;
		while(fgetc (dicionario) != EOF){
			fseek(dicionario, -1, 1);
			fread(tupla, sizeof(char), TABLE_NAME_SIZE, dicionario);
			printf(" %-10s | %-15s | %-10s | %-10s\n", "public", tupla, "tuple", "ibetres");
			fseek(dicionario, 28, 1);
			i++;
		}
		fclose(dicionario);
		free(tupla);
		printf("(%d %s)\n\n", i, (i<=1)? "row": "rows");
	} else{               //mostra todos atributos da tabela *tbl

		if(!verifyTableName(tbl)) {
			printf("Did not find any relation named \"%s\".\n", tbl);
			return;
		}

		printf("	  Table \"public.%s\"\n", tbl);
		printf(" %-18s | %-12s | %-10s\n", "Column", "Type", "Modifiers");
		printf("--------------------+--------------+-----------\n");

		fs_objects objeto1;
		tp_table *esquema1;

		openTable(tbl, &objeto1, &esquema1);

		tp_table *tab3 = (tp_table *)malloc(sizeof(tp_table));
		tab3 = searchAttributeskeys(objeto1); //retorna tp_table
		int l, ipk=0, ifk=0;

		char **pk 			= (char**)malloc(objeto1.qtdCampos*sizeof(char**));
		char **fkTable		= (char**)malloc(objeto1.qtdCampos*sizeof(char**));
		char **fkColumn 	= (char**)malloc(objeto1.qtdCampos*sizeof(char**));
		char **refColumn 	= (char**)malloc(objeto1.qtdCampos*sizeof(char**));

		memset(pk 		, 0, objeto1.qtdCampos);
		memset(fkTable 	, 0, objeto1.qtdCampos);
		memset(fkColumn , 0, objeto1.qtdCampos);
		memset(refColumn, 0, objeto1.qtdCampos);

		int i;
		for(i=0; i<objeto1.qtdCampos; i++) {
			pk[i] 			= (char*)malloc(FIELD_NAME_SIZE*sizeof(char));
			fkTable[i] 		= (char*)malloc(FIELD_NAME_SIZE*sizeof(char));
			fkColumn[i] 	= (char*)malloc(FIELD_NAME_SIZE*sizeof(char));
			refColumn[i] 	= (char*)malloc(FIELD_NAME_SIZE*sizeof(char));

			memset(pk[i] 		, '\0', FIELD_NAME_SIZE);
			memset(fkTable[i] 	, '\0', FIELD_NAME_SIZE);
			memset(fkColumn[i]  , '\0', FIELD_NAME_SIZE);
			memset(refColumn[i] , '\0', FIELD_NAME_SIZE);

		}

		for(l=0; l<objeto1.qtdCampos; l++) {

			if(tab3[l].chave == PK){
				strcpylower(pk[ipk++], tab3[l].nome);
			}
			else if(tab3[l].chave == FK){
				strcpylower(fkTable[ifk]	, tab3[l].tabelaApt);
				strcpylower(fkColumn[ifk]	, tab3[l].attApt);
				strcpylower(refColumn[ifk++], tab3[l].nome);
			}

			printf("  %-17s |", tab3[l].nome);

			if(tab3[l].tipo == 'S')
				printf(" %-8s(%d) |", " varchar", tab3[l].tam);
			else if(tab3[l].tipo == 'I')
				printf(" %-10s   |", " integer");
			else if(tab3[l].tipo == 'C')
				printf(" %-10s   |", " char");
			else if(tab3[l].tipo == 'D')
				printf(" %-10s   |", " double");

			printf(" %-10s ", (tab3[l].chave == PK || tab3[l].chave == FK)? "not null": "null");

			printf("\n");
		}
		if(ipk){	//printf PK's
			printf("Indexes:\n");
			for(l = 0; l < ipk; l++){
				printf("\t\"%s_pkey\" PRIMARY KEY (%s)\n", tbl, pk[l]);
			}
		}
		if(ifk){	//printf FK's
			printf("Foreign-key constrains:\n");
			for(l = 0; l < ifk; l++){
				printf("\t\"%s_%s_fkey\" FOREIGN KEY (%s) REFERENCES %s(%s)\n",tbl, refColumn[l], refColumn[l], fkTable[l], fkColumn[l]);
			}
		}

		free(pk);
		free(fkTable);
		free(fkColumn);
		free(refColumn);
		free(tab3);
		printf("\n");
	}
}
/* NÃO IMPLEMENTADO: a ideia era dar o free de maneira correta nas listas alocadas ao longo
 * da execução do programa, já que os grupos anteriores não se preocuparam com isso. No entanto,
 * não tivemos tempo ágil para implementar uma solução livre de erros e preferimos deixar
 * dessa maneira.
 */

void freeTp_table(tp_table **tabela, int n) {
	free(tabela);
}


void freeTable(table *tabela) {
	if (tabela != NULL) {
		free(tabela->esquema);
		free(tabela);
	}
}


void freeColumn(column *colunas) {
	if (colunas != NULL) {
		free(colunas);
	}
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shuntingyard.h"
////
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

#ifndef FBUFFER // garante que buffer.h não seja reincluída
  #include "buffer.h"
#endif

#ifndef FDICTIONARY // the same
  #include "dictionary.h"
#endif

/* ----------------------------------------------------------------------------------------------
    Objetivo:   Recebe o nome de uma tabela e engloba as funções readObject() e readSchema().
    Parametros: Nome da Tabela, Objeto da Tabela e tabela.
    Retorno:    tp_table
   ---------------------------------------------------------------------------------------------*/
tp_table *openTable(char *nomeTabela, struct fs_objects *objeto, tp_table **tabela) {
    *objeto     = readObject(nomeTabela);
    *tabela     = readSchema(*objeto);

    return *tabela;
}
// Se foram especificadas colunas no *s_insert, verifica se elas existem no esquema.
int allColumnsExists(rc_insert *s_insert, table *tabela) {
	int i;
	if (!s_insert->columnName) return 0;

	for (i = 0; i < s_insert->N; i++)
		if (returnsSizeTypeField(s_insert->columnName[i], tabela) == 0) {
			printf("ERROR: column \"%s\" of relation \"%s\" does not exist.\n", s_insert->columnName[i], tabela->nome);
			return 0;
		}

	return 1;
}
////
int typesCompatible(char table_type, char insert_type) {
	return (table_type == 'D' && insert_type == 'I')
		|| (table_type == 'D' && insert_type == 'D')
		|| (table_type == 'I' && insert_type == 'I')
		|| (table_type == 'S' && insert_type == 'S')
		|| (table_type == 'S' && insert_type == 'C')
		|| (table_type == 'C' && insert_type == 'C')
		|| (table_type == 'C' && insert_type == 'S');
}
////
// Busca o tipo do valor na inserção *s_insert do valor que irá para *columnName
// Se não existe em *s_insert, assume o tipo do esquema já que não receberá nada.
char getInsertedType(rc_insert *s_insert, char *columnName, table *tabela) {
	int i;
	char noValue;
	for (i = 0; i < s_insert->N; i++) {
		if (objcmp(s_insert->columnName[i], columnName) == 0) {
			return s_insert->type[i];
		}
	}

	noValue = returnsSizeTypeField(columnName, tabela);

	return noValue;
}
// Busca o valor na inserção *s_insert designado à *columnName.
// Se não existe, retorna 0, 0.0 ou \0
char *getInsertedValue(rc_insert *s_insert, char *columnName, table *tabela) {
	int i;
	char tipo, *noValue;

	for (i = 0; i < s_insert->N; i++) {
		if (objcmp(s_insert->columnName[i], columnName) == 0) {
			return s_insert->values[i];
		}
	}

	tipo = returnsSizeTypeField(columnName, tabela);
	noValue = (char *)malloc(sizeof(char)*3);

	if (tipo == 'I') {
		noValue = "0";
	} else if (tipo == 'D') {
		noValue = "0.0";
	} else
		noValue[0] = '\0';

	return noValue;
}
/* ----------------------------------------------------------------------------------------------
    Objetivo:   Inicializa os atributos necessários para a verificação de FK e PK.
    Parametros: Objeto da tabela, Tabela, Buffer e nome da tabela.
    Retorno:    INT
                SUCCESS,
                PARAMETER_ERROR_1,
   ---------------------------------------------------------------------------------------------*/

int initAttributes(struct fs_objects *objeto, tp_table **tabela, tp_buffer **bufferpool, char *nomeT){


    *objeto     = readObject(nomeT);
    *tabela     = readSchema(*objeto);
    *bufferpool = initbuffer();

    if(*tabela == ERROR_OPEN_SCHEMA) {
        return PARAMETER_ERROR_1;
    }

    if(*bufferpool == ALLOCATION_ERROR)
        return PARAMETER_ERROR_1;

    return SUCCESS;
}
////
int verifyFK(char *tableName, char *column){
    if(verifyTableName(tableName) == 1){
        struct fs_objects objeto = readObject(tableName);
        tp_table *esquema = readSchema(objeto);
		if(esquema == ERROR_OPEN_SCHEMA){
            printf("ERROR: cannot create schema.\n");
            return 0;
        }

        for(; esquema != NULL; esquema = esquema->next){
            if(objcmp(esquema->nome, column) == 0){
                if(esquema->chave == PK){
                    return 1;
                }
            }
        }
    }
	return 0;
}

////////
/* ----------------------------------------------------------------------------------------------
    Objetivo:   Gera as verificações em relação a chave FK.
    Parametros: Nome da Tabela, Coluna C, Nome do Campo, Valor do Campo, Tabela Apontada e Atributo Apontado.
    Retorno:    INT
                SUCCESS,
                PARAMETER_ERROR_1,
                ERROR_FOREIGN_KEY
   ---------------------------------------------------------------------------------------------*/

int verifyForeignKey(char *nomeTabela,column *c, char *nomeCampo, char *valorCampo, char *tabelaApt, char *attApt){
    int x,j, erro, page;
    char str[20];
    char dat[5] = ".dat";
    struct fs_objects objeto;
    tp_table *tabela;
    tp_buffer *bufferpoll;
    column *pagina = NULL;

    strcpylower(str, tabelaApt);
    strcat(str, dat);              //Concatena e junta o nome com .dat

    erro = attributeExists(nomeTabela, c);
    /*if(erro != SUCCESS )
        return PARAMETER_ERROR_1;*/

    //if(attributeExists(tabelaApt, c))
        //return ERROR_FOREIGN_KEY;

    if(initAttributes(&objeto, &tabela, &bufferpoll, tabelaApt) != SUCCESS) {
        free(bufferpoll);
        free(tabela);
        return PARAMETER_ERROR_1;
    }


    erro = SUCCESS;
    for(x = 0; erro == SUCCESS; x++)
        erro = colocaTuplaBuffer(bufferpoll, x, tabela, objeto);

    for (page = 0; page < PAGES; page++) {
        if (pagina) free(pagina);
        pagina = getPage(bufferpoll, tabela, objeto, page);
        if (!pagina) break;

        for(j = 0; j < objeto.qtdCampos * bufferpoll[page].nrec; j++){
            if (pagina[j].nomeCampo) {
                if(objcmp(pagina[j].nomeCampo, attApt) == 0){

                    if(pagina[j].tipoCampo == 'S'){
                        if(objcmp(pagina[j].valorCampo, valorCampo) == 0){
                            free(pagina);
                            free(bufferpoll);
                            free(tabela);
                            return SUCCESS;
                        }
                    } else if(pagina[j].tipoCampo == 'I'){
                        int *n = (int *)&pagina[j].valorCampo[0];
                        if(*n == atoi(valorCampo)){
                            free(pagina);
                            free(bufferpoll);
                            free(tabela);
                            return SUCCESS;
                        }
                    } else if(pagina[j].tipoCampo == 'D'){
                        double *nn = (double *)&pagina[j].valorCampo[0];

                        if(*nn == atof(valorCampo)){
                            free(pagina);
                            free(bufferpoll);
                            free(tabela);
                            return SUCCESS;
                        }
                    } else if(pagina[j].tipoCampo == 'C'){
                        if(pagina[j].valorCampo == valorCampo){
                            free(pagina);
                            free(bufferpoll);
                            free(tabela);
                            return SUCCESS;
                        }
                    } else {
                        free(pagina);
                        free(bufferpoll);
                        free(tabela);
                        return ERROR_FOREIGN_KEY;
                    }
                }
            }
        }
    }

    if (pagina) free(pagina);
    free(bufferpoll);
    free(tabela);
    return ERROR_FOREIGN_KEY;
}
/* ----------------------------------------------------------------------------------------------
    Objetivo:   Gera as verificações em relação a chave PK.
    Parametros: Nome da Tabela, Coluna C, Nome do Campo, Valor do Campo
    Retorno:    INT
                SUCCESS,
                PARAMETER_ERROR_1,
                ERROR_PRIMARY_KEY
   ---------------------------------------------------------------------------------------------*/

int verifyPrimaryKey(char *nomeTabela, column *c, char *nomeCampo, char *valorCampo) {
    int j, x, erro, page;
    column *pagina = NULL;

    struct fs_objects objeto;
    tp_table *tabela;
    tp_buffer *bufferpoll;

    erro = attributeExists(nomeTabela, c);
    if (erro != SUCCESS ) {
        free(bufferpoll);
        return PARAMETER_ERROR_1;
    }


    if (initAttributes(&objeto, &tabela, &bufferpoll, nomeTabela) != SUCCESS) {
        free(bufferpoll);
        free(tabela);
        return PARAMETER_ERROR_1;
    }

    erro = SUCCESS;
    for(x = 0; erro == SUCCESS; x++)
        erro = colocaTuplaBuffer(bufferpoll, x, tabela, objeto);

    page = 0;
    for (page = 0; page < PAGES; page++) {
        if (pagina) free(pagina);
        pagina = getPage(bufferpoll, tabela, objeto, page);
        if (!pagina) break;

        for(j = 0; j < objeto.qtdCampos * bufferpoll[page].nrec; j++){
            if (pagina[j].nomeCampo) {
                if (objcmp(pagina[j].nomeCampo, nomeCampo) == 0) {
                    if (pagina[j].tipoCampo == 'S') {
                        if (objcmp(pagina[j].valorCampo, valorCampo) == 0){
                            free(pagina);
                            free(bufferpoll);
                            free(tabela);
                            return ERROR_PRIMARY_KEY;
                        }
                    } else if (pagina[j].tipoCampo == 'I') {
                        int *n = (int *)&pagina[j].valorCampo[0];

                        if (*n == atoi(valorCampo)) {
                            free(pagina);
                            free(bufferpoll);
                            free(tabela);
                            return ERROR_PRIMARY_KEY;
                        }
                    } else if (pagina[j].tipoCampo == 'D'){
                        double *nn = (double *)&pagina[j].valorCampo[0];

                        if (*nn == atof(valorCampo)){
                            free(pagina);
                            free(bufferpoll);
                            free(tabela);
                            return ERROR_PRIMARY_KEY;
                        }
                    } else if (pagina[j].tipoCampo == 'C'){
                        if (pagina[j].valorCampo == valorCampo){
                            free(pagina);
                            free(bufferpoll);
                            free(tabela);
                            return ERROR_PRIMARY_KEY;
                        }
                    }
                }
            }
        }
    }

    if (pagina) free(pagina);
    free(bufferpoll);
    free(tabela);
    return SUCCESS;
}

/////
int finishInsert(char *nome, column *c){
    column *auxC, *temp;
    int i = 0, x = 0, t, erro, j = 0;
    FILE *dados;

    struct fs_objects objeto,dicio; // Le dicionario
    tp_table *auxT ; // Le esquema
    auxT = openTable(nome, &dicio, &auxT);

    table *tab     = (table *)malloc(sizeof(table));
    tp_table *tab2 = (tp_table *)malloc(sizeof(struct tp_table));
    memset(tab2, 0, sizeof(tp_table));

    tab->esquema = openTable(nome, &objeto, &tab->esquema);
    tab2 = searchAttributeskeys(objeto);

    for(j = 0, temp = c; j < objeto.qtdCampos && temp != NULL; j++, temp = temp->next){
        switch(tab2[j].chave){
            case NPK:
                erro = SUCCESS;
                break;

            case PK:
                erro = verifyPrimaryKey(nome, temp , temp->nomeCampo, temp->valorCampo);
                if (erro == ERROR_PRIMARY_KEY){
                    printf("ERROR: duplicate key value violates unique constraint \"%s_pkey\"\nDETAIL:  Key (%s)=(%s) already exists.\n", nome, temp->nomeCampo, temp->valorCampo);

					free(auxT); // Libera a memoria da estrutura.
					//free(temp); // Libera a memoria da estrutura.
					free(tab); // Libera a memoria da estrutura.
					free(tab2); // Libera a memoria da estrutura.
                    return ERROR_PRIMARY_KEY;
                }

                break;

            case FK:
                if (tab2[j].chave == 2 && strlen(tab2[j].attApt) != 0 && strlen(tab2[j].tabelaApt) != 0){

                    erro = verifyForeignKey(nome, temp, tab2[j].nome, temp->valorCampo, tab2[j].tabelaApt, tab2[j].attApt);

                    if (erro != SUCCESS){
                        printf("ERROR: invalid reference to \"%s.%s\". The value \"%s\" does not exist.\n", tab2[j].tabelaApt,tab2[j].attApt,temp->valorCampo);

						free(auxT); // Libera a memoria da estrutura.
						free(temp); // Libera a memoria da estrutura.
                        free(tab); // Libera a memoria da estrutura.
						free(tab2); // Libera a memoria da estrutura.
                        return ERROR_FOREIGN_KEY;
                    }
                }

                break;
        }
    }


    if (erro == ERROR_FOREIGN_KEY){
        printf("ERROR: unknown foreign key error.\n");

		free(auxT); // Libera a memoria da estrutura.
		free(temp); // Libera a memoria da estrutura.
        free(tab); // Libera a memoria da estrutura.
		free(tab2); // Libera a memoria da estrutura.
        return ERROR_FOREIGN_KEY;
    }

    if (erro == ERROR_PRIMARY_KEY){
        printf("ERROR: unknown primary key error.\n");

		free(auxT); // Libera a memoria da estrutura.
		free(temp); // Libera a memoria da estrutura.
        free(tab); // Libera a memoria da estrutura.
		free(tab2); // Libera a memoria da estrutura.
        return ERROR_PRIMARY_KEY;
    }
    if (erro == PARAMETER_ERROR_1) {
        printf("ERROR: invalid parameter.\n");
		free(auxT); // Libera a memoria da estrutura.
		free(temp); // Libera a memoria da estrutura.
        free(tab); // Libera a memoria da estrutura.
		free(tab2); // Libera a memoria da estrutura.
        return PARAMETER_ERROR_1;
    }

    char directory[LEN_DB_NAME*2];
    strcpy(directory, connected.db_directory);
    strcat(directory, dicio.nArquivo);

    if((dados = fopen(directory,"a+b")) == NULL){
        printf("ERROR: cannot open file.\n");
		free(auxT); // Libera a memoria da estrutura.
		free(temp); // Libera a memoria da estrutura.
        free(tab); // Libera a memoria da estrutura.
		free(tab2); // Libera a memoria da estrutura.
        return ERROR_OPEN_FILE;

	}

    for(auxC = c, t = 0; auxC != NULL; auxC = auxC->next, t++){
        if (t >= dicio.qtdCampos)
            t = 0;

        if (auxT[t].tipo == 'S'){ // Grava um dado do tipo string.

            if (sizeof(auxC->valorCampo) > auxT[t].tam && sizeof(auxC->valorCampo) != 8){
                printf("ERROR: invalid string length.\n");
				free(tab); // Libera a memoria da estrutura.
				free(tab2); // Libera a memoria da estrutura.
				free(auxT); // Libera a memoria da estrutura.
				free(temp); // Libera a memoria da estrutura.
				fclose(dados);
                return STRING_LENGTH_ERROR;
            }

            if (objcmp(auxC->nomeCampo, auxT[t].nome) != 0){
                printf("ERROR: column name \"%s\" is not valid.\n", auxC->nomeCampo);
				free(tab); // Libera a memoria da estrutura.
				free(tab2); // Libera a memoria da estrutura.
				free(auxT); // Libera a memoria da estrutura.
				free(temp); // Libera a memoria da estrutura.
				fclose(dados);
                return FIELD_NAME_ERROR;
            }

            char valorCampo[auxT[t].tam];
            strncpy(valorCampo, auxC->valorCampo, auxT[t].tam);
            strcat(valorCampo, "\0");
            fwrite(&valorCampo,sizeof(valorCampo),1,dados);

        } else if (auxT[t].tipo == 'I'){ // Grava um dado do tipo inteiro.
            i = 0;
            while (i < strlen(auxC->valorCampo)){
                if (auxC->valorCampo[i] < 48 || auxC->valorCampo[i] > 57){
                    printf("ERROR: column \"%s\" expectet integer.\n", auxC->nomeCampo);
					free(tab); // Libera a memoria da estrutura.
					free(tab2); // Libera a memoria da estrutura.
					free(auxT); // Libera a memoria da estrutura.
					//free(temp); // Libera a memoria da estrutura.
					fclose(dados);
                    return ERROR_EXPECTED_INTEGER;
                }
                i++;
            }

            int valorInteiro = 0;

            sscanf(auxC->valorCampo,"%d",&valorInteiro);
            fwrite(&valorInteiro,sizeof(valorInteiro),1,dados);

        } else if (auxT[t].tipo == 'D'){ // Grava um dado do tipo double.
            x = 0;
            while (x < strlen(auxC->valorCampo)){
                if((auxC->valorCampo[x] < 48 || auxC->valorCampo[x] > 57) && (auxC->valorCampo[x] != 46)){
                    printf("ERROR: column \"%s\" expect double.\n", auxC->nomeCampo);
					free(tab); // Libera a memoria da estrutura.
					free(tab2); // Libera a memoria da estrutura.
					free(auxT); // Libera a memoria da estrutura.
					free(temp); // Libera a memoria da estrutura.
					fclose(dados);
                    return ERROR_EXPECTED_DOUBLE;
                }
                x++;
            }

            double valorDouble = convertD(auxC->valorCampo);
            fwrite(&valorDouble,sizeof(valorDouble),1,dados);
        }
        else if (auxT[t].tipo == 'C'){ // Grava um dado do tipo char.

            if (strlen(auxC->valorCampo) > (sizeof(char))) {
                printf("ERROR: column \"%s\" expect char.\n", auxC->nomeCampo);
				free(tab); // Libera a memoria da estrutura.
				free(tab2); // Libera a memoria da estrutura.
				free(auxT); // Libera a memoria da estrutura.
				free(temp); // Libera a memoria da estrutura.
				fclose(dados);
                return ERROR_EXPECTED_CHAR;
            }
            char valorChar = auxC->valorCampo[0];
            fwrite(&valorChar,sizeof(valorChar),1,dados);
        }

    }

    fclose(dados);
    free(tab); // Libera a memoria da estrutura.
	free(tab2); // Libera a memoria da estrutura.
    free(auxT); // Libera a memoria da estrutura.
    free(temp); // Libera a memoria da estrutura.
    return SUCCESS;
}

/* insert: Recebe uma estrutura rc_insert e valida os tokens encontrados pela interface().
 *         Se os valores forem válidos, insere um novo valor.
 */
void insert(rc_insert *s_insert) {
	int i;
    char flag = 0;
    column *colunas = NULL;
    tp_table *esquema = NULL;
	struct fs_objects objeto;
	memset(&objeto, 0, sizeof (struct fs_objects));
    table *tabela = (table *) malloc(sizeof (table));
	tabela -> esquema = NULL;
	memset(tabela, 0, sizeof (table));

	openTable(s_insert -> objName, &objeto, &tabela -> esquema); //retorna o esquema para a insere valor
	// strcpylower(tabela->nome, s_insert->objName);
    strcpy(tabela -> nome, s_insert -> objName);

	if (s_insert -> columnName != NULL) {
		if (allColumnsExists(s_insert, tabela)) {
			for (esquema = tabela -> esquema; esquema != NULL; esquema = esquema -> next) {
				if (typesCompatible(esquema -> tipo, getInsertedType(s_insert, esquema -> nome, tabela))) {
					colunas = insertValue(tabela, colunas, esquema -> nome, getInsertedValue(s_insert, esquema -> nome, tabela));
				}
                else {
					printf("ERROR: data type invalid to column '%s' of relation '%s' (expected: %c, received: %c).\n", esquema -> nome, tabela -> nome, esquema -> tipo, getInsertedType(s_insert, esquema -> nome, tabela));
					flag = 1;
				}
			}
		}
        else {
			flag = 1;
		}
	}
    else {
		if (s_insert -> N == objeto.qtdCampos) {
			for (i = 0; i < objeto.qtdCampos; i++) {

                // bug da inserção de string no tipo char
				if (s_insert -> type[i] == 'S' && tabela -> esquema[i].tipo == 'C' && s_insert -> values[i][1] == '\0') {
					// s_insert -> values[i][1] = '\0';
					s_insert -> type[i] = 'C';
				}

                // precisa disso? '0' não é considerado Double, caso tente inserir?
				if (s_insert -> type[i] == 'I' && tabela -> esquema[i].tipo == 'D') {
					s_insert -> type[i] = 'D';
				}

				if (s_insert -> type[i] == tabela -> esquema[i].tipo) {
					colunas = insertValue(tabela, colunas, tabela -> esquema[i].nome, s_insert -> values[i]);
                }
				else {
					printf("ERROR: data type invalid to column '%s' of relation '%s' (expected: %c, received: %c).\n", tabela -> esquema[i].nome, tabela -> nome, tabela -> esquema[i].tipo, s_insert -> type[i]);
					flag = 1;
				}
			}
		}
        else {
			printf("ERROR: INSERT has more expressions than target columns.\n");
			flag = 1;
		}
	}

	if (!flag)
		if (finishInsert(s_insert -> objName, colunas) == SUCCESS)
			printf("INSERT 0 1\n");

	//freeTp_table(&esquema, objeto.qtdCampos);
	free(esquema);
	freeColumn(colunas);
	freeTable(tabela);
}

///////////////
void printing(rc_insert *select) {
    struct fs_objects objeto;
    int j, erro, x, p, cont = 0, novaTupla = 1;
    char *nomeTabela = select -> objName;
    if (!verifyTableName(nomeTabela)) {
        printf("\nERROR: relation \"%s\" was not found.\n\n\n", nomeTabela);
        return;
    }

    char **tokens = shuntingYard(select -> condition, select -> ncond);
    for (j = 0; j < select -> ncond; j++)
      printf("- token[%d] = %s\n", j, tokens[j]);
    printf("N: %d   ncond: %d\n", select -> N, select -> ncond);
    printf("objName: %s\n", select -> objName);
    for (j = 0; j < select -> N; j++)
      printf("columnName: %s\n", select -> columnName[j]);
    for (j = 0; j < select -> ncond; j++)
      printf("condition: %s\n", select -> condition[j]);
    objeto = readObject(nomeTabela);
    tp_table *esquema = readSchema(objeto);
    if (esquema == ERROR_OPEN_SCHEMA){
        printf("ERROR: schema cannot be created.\n");
        free(esquema);
        return;
    }

    tp_buffer *bufferpoll = initbuffer();
    // printf("--------------------\n");
    if (bufferpoll == ALLOCATION_ERROR){
        free(bufferpoll);
        free(esquema);
        printf("ERROR: no memory available to allocate buffer.\n");
        return;
    }

    erro = SUCCESS;
    for (x = 0; erro == SUCCESS; x++)
      erro = colocaTuplaBuffer(bufferpoll, x, esquema, objeto);
	  p = 0;
    int ntuples = --x, flag, i, aux = 0; //Flag cuida quais colunas serão printadas
    //Queria usar esses colunas printadas para não ter que repetir o código feio =(
    char *colunasPrintada = malloc(objeto.qtdCampos * sizeof(char));
    memset(colunasPrintada, 0, objeto.qtdCampos * sizeof(char));
	  while (x) {
	    column *pagina = getPage(bufferpoll, esquema, objeto, p);
	    if (pagina == PARAMETER_ERROR_2){
        printf("ERROR: could not open the table.\n");
        free(bufferpoll); free(esquema); return;
	    }
	    if (!cont) {
	      for (aux = j = 0; j < objeto.qtdCampos; j++) {
          // Código feio para fazer a projeção(??)
          for (flag = i = 0; i < select -> N; i++) {
            //printf("%s\n", select -> columnName[i]);
            flag |= !strcmp(select -> columnName[i], pagina[j].nomeCampo);
          }
          if (!flag && select -> N > 0) continue;
          colunasPrintada[j] = 1; aux++;
          //Fim do código feio
	        if (pagina[j].tipoCampo == 'S') printf(" %-20s ", pagina[j].nomeCampo);
	        else printf(" %-10s ", pagina[j].nomeCampo);
	        if (aux < select -> N) printf("|");
	      }
	      printf("\n");
	      for (aux = j = 0; j < objeto.qtdCampos; j++){
          if (!colunasPrintada[j]) continue;
	        printf("%s",(pagina[j].tipoCampo == 'S')? "----------------------": "------------");
          aux++;
	        if (aux < select -> N) printf("+");
	      }
	      printf("\n");
	    }
	    cont++; novaTupla = 1;
		  for(aux = j = 0; j < objeto.qtdCampos * bufferpoll[p].nrec; j++) {
        // Código feio para fazer a projeção(??)
        //Fim do código feio
        for (flag = i = 0; i < select -> N; i++) {
        //  printf(">>%s    %s<<",select -> columnName[i], select -> columnName[i]);
          flag |= !strcmp(select -> columnName[i], pagina[j].nomeCampo);
        }
        if (!flag && select -> N > 0) continue;
        aux++;
        // if (novaTupla && testwhere(pagina + j, tokens, select -> ncond, bufferpoll[p].nrec, objeto)) {
        //   //??
        // }
          //???x -= bufferpoll[p++].nrec; continue; }
        novaTupla = 0;
        if(pagina[j].tipoCampo == 'S')
          printf(" %-20s ", pagina[j].valorCampo);
        else if(pagina[j].tipoCampo == 'I') {
          int *n = (int *)&pagina[j].valorCampo[0];
          printf(" %-10d ", *n);
        } else if(pagina[j].tipoCampo == 'C') {
          printf(" %-10c ", pagina[j].valorCampo[0]);
        } else if(pagina[j].tipoCampo == 'D') {
          double *n = (double *)&pagina[j].valorCampo[0];
    	    printf(" %-10f ", *n);
        }
        if(j >= 0 && aux % select -> N == 0) { novaTupla = 1; printf("\n"); }
        else printf("|");
    	}
    	x -= bufferpoll[p++].nrec;
    }
    printf("\n(%d %s)\n\n", ntuples, ntuples == 1 ? "row" : "rows");

    free(bufferpoll);
    free(esquema);
}
/* ----------------------------------------------------------------------------------------------
    Objetivo:   Copia todas as informações menos a tabela do objeto, que será removida.
    Parametros: Objeto que será removido do schema.
    Retorno:    INT
                SUCCESS,
                DELETE_SCHEMA_FILE_ERROR
   ---------------------------------------------------------------------------------------------*/

int searchSchemaFile(struct fs_objects objeto){
    FILE *schema, *newSchema;
    int cod = 0;
    char *tupla = (char *)malloc(sizeof(char) * 109);
    memset(tupla, '\0', 109);

    tp_table *esquema = (tp_table *)malloc(sizeof(tp_table)*objeto.qtdCampos);
    memset(esquema, 0, sizeof(tp_table)*objeto.qtdCampos);

    char directory[LEN_DB_NAME*2];
    memset(&directory, '\0', LEN_DB_NAME*2);

    strcpy(directory, connected.db_directory);
    strcat(directory, "fs_schema.dat");

    if((schema = fopen(directory, "a+b")) == NULL) {
        free(tupla);
        return DELETE_SCHEMA_FILE_ERROR;
    }

    strcpy(directory, connected.db_directory);
    strcat(directory, "fs_nschema.dat");

    if((newSchema = fopen(directory, "a+b")) == NULL) {
        free(tupla);
        return DELETE_SCHEMA_FILE_ERROR;
    }

    fseek(newSchema, 0, SEEK_SET);

    while((fgetc (schema) != EOF)){ // Varre o arquivo ate encontrar todos os campos com o codigo da tabela.
        fseek(schema, -1, 1);
        fseek(newSchema, 0, SEEK_END);

        if(fread(&cod, sizeof(int), 1, schema)){ // Le o codigo da tabela.
            if(cod != objeto.cod){
                fwrite(&cod, sizeof(int), 1, newSchema);

                fread(tupla, sizeof(char), FIELD_NAME_SIZE, schema);
                strcpylower(esquema[0].nome,tupla);                  // Copia dados do campo para o esquema.
                fwrite(tupla, sizeof(char), FIELD_NAME_SIZE, newSchema);

                fread(&esquema[0].tipo , sizeof(char), 1 , schema);
                fread(&esquema[0].tam  , sizeof(int) , 1 , schema);
                fread(&esquema[0].chave, sizeof(int) , 1 , schema);

                fwrite(&esquema[0].tipo , sizeof(char), 1, newSchema);
                fwrite(&esquema[0].tam  , sizeof(int) , 1, newSchema);
                fwrite(&esquema[0].chave, sizeof(int) , 1, newSchema);

                fread(tupla, sizeof(char), TABLE_NAME_SIZE, schema);
                strcpylower(esquema[0].tabelaApt,tupla);
                fwrite(&esquema[0].tabelaApt, sizeof(char), TABLE_NAME_SIZE, newSchema);

                fread(tupla, sizeof(char), FIELD_NAME_SIZE, schema);
                strcpylower(esquema[0].attApt,tupla);
                fwrite(&esquema[0].attApt, sizeof(char), FIELD_NAME_SIZE, newSchema);

            } else {
                fseek(schema, 109, 1);
            }
        }
    }

    fclose(newSchema);
    fclose(schema);

    char directoryex[LEN_DB_NAME*4];
    memset(&directoryex, '\0', LEN_DB_NAME*4);
    strcpy(directoryex, connected.db_directory);
    strcat(directoryex, "fs_schema.dat");

    remove(directoryex);

    strcpy(directoryex, "mv ");
    strcat(directoryex, connected.db_directory);
    strcat(directoryex, "fs_nschema.dat ");
    strcat(directoryex, connected.db_directory);
    strcat(directoryex, "fs_schema.dat");

    system(directoryex);

    free(tupla);
    return SUCCESS;
}

/* ----------------------------------------------------------------------------------------------
    Objetivo:   Função para exclusão de tabelas.
    Parametros: Nome da tabela (char).
    Retorno:    INT
                SUCCESS,
                DELETE_OBJECT_FILE_ERROR,
                DELETE_SCHEMA_FILE_ERROR,
                READING_DATA_ERROR.
   ---------------------------------------------------------------------------------------------*/

int deleteTable(char *nomeTabela) {
    struct fs_objects objeto, objeto1;
    tp_table *esquema, *esquema1;
    int x, erro, i, j, k, l, qtTable;
    char str[TABLE_NAME_SIZE + 4]; //.dat
    char dat[5] = ".dat";
    memset(str, '\0', TABLE_NAME_SIZE + 4);

    if (!verifyTableName(nomeTabela)) {
        printf("ERROR: table \"%s\" does not exist.\n", nomeTabela);
        return TABLE_NAME_ERROR;
    }

    strncpy(str, nomeTabela, TABLE_NAME_SIZE - 1);
    strcat(str, dat);              //Concatena e junta o nome com .dat

    openTable(nomeTabela, &objeto, &esquema);
    qtTable = quantityTable();

    char **tupla = (char **)malloc(sizeof(char **)*qtTable);
    memset(tupla, 0, qtTable);

    for (i = 0; i < qtTable; i++) {
        tupla[i] = (char *)malloc(sizeof(char)*TABLE_NAME_SIZE);
        memset(tupla[i], '\0', TABLE_NAME_SIZE);
    }

    tp_table *tab2 = (tp_table *)malloc(sizeof(struct tp_table));
    tab2 = searchAttributeskeys(objeto);   //retorna o tipo de chave que e cada campo

    FILE *dicionario;

    char directory[LEN_DB_NAME*2];
    memset(directory, '\0', LEN_DB_NAME*2);

    strcpy(directory, connected.db_directory);
    strcat(directory, "fs_object.dat");

    if((dicionario = fopen(directory,"a+b")) == NULL)
        return ERROR_OPEN_FILE;

    k=0;
    while(fgetc (dicionario) != EOF){
        fseek(dicionario, -1, 1);

        //coloca o nome de todas as tabelas em tupla
        fread(tupla[k], sizeof(char), TABLE_NAME_SIZE , dicionario);
        k++;

        fseek(dicionario, 28, 1);
    }

    fclose(dicionario);

    for(i = 0; i < objeto.qtdCampos; i++){
        if(tab2[i].chave == PK){
            for(j=0; j<qtTable; j++) {                      //se tiver chave primaria verifica se ela e chave
                if(objcmp(tupla[j], nomeTabela) != 0) {     //estrangeira em outra tabela

                    openTable(tupla[j], &objeto1, &esquema1);

                    tp_table *tab3 = (tp_table *)malloc(sizeof(struct tp_table));
                    tab3 = searchAttributeskeys(objeto1);

                    for(l=0; l<objeto1.qtdCampos; l++) {
                        if(tab3[l].chave == FK) { //verifica se a outra tabela possui chave estrangeira. se sim, verifica se e da tabela anterior.
                            if(objcmp(nomeTabela, tab3[l].tabelaApt) == 0) {
                                printf("ERROR: cannot drop table \"%s\" because other objects depend on it.\n", nomeTabela);
                                return ERROR_FOREIGN_KEY;
                            }
                        }
                    }
                    free(tab3);
                }
            }
        }
    }

    free(tab2);

    tp_buffer *bufferpoll = initbuffer();

    if(bufferpoll == ALLOCATION_ERROR){
        printf("ERROR: no memory available to allocate buffer.\n");
        return READING_DATA_ERROR;
    }

    erro = SUCCESS;
    for(x = 0; erro == SUCCESS; x++)
        erro = colocaTuplaBuffer(bufferpoll, x, esquema, objeto);

    if(searchSchemaFile(objeto) != 0) {
        free(bufferpoll);
        return DELETE_SCHEMA_FILE_ERROR;
    }

    if(searchObjectFile(nomeTabela) != 0) {
        free(bufferpoll);
        return DELETE_OBJECT_FILE_ERROR;
    }

   	strcpy(directory, connected.db_directory);
    strcat(directory, str);

    remove(directory);

    free(bufferpoll);

    printf("DROP TABLE\n");

    return SUCCESS;
}

//N = numero de colunas da tabela
//**fieldName = Vetor dos nomes das colunas da tabela
//Verifica se as colunas da tabela não tem nome repetido
int verifyFieldName(char **fieldName, int N){
    int i, j;
    if(N <= 1) return 1; //Meeeu deus, notem como essa poda realmente faz alguma diferença
    for(i = 0; i < N; i++){
        for(j = i + 1; j < N; j++){
            if(objcmp(fieldName[i], fieldName[j]) == 0){
                printf("ERROR: column \"%s\" specified more than once\n", fieldName[i]);
                return 0;
            }
        }
    }
    return 1;
}

//////
void createTable(rc_insert *t) {
	int size, i, error;

    //+ 4 por causa do .dat
    char tableName[TABLE_NAME_SIZE + 4], fkTable[TABLE_NAME_SIZE], fkColumn[FIELD_NAME_SIZE];

    strncpy(tableName, t->objName, TABLE_NAME_SIZE); //Faz o truncamento do nome da tabela
    strcat(tableName, ".dat\0");                  //tableName.dat

    if(fileExists(tableName)){ printf("ERROR: table already exist\n"); return; }
    table *tab = NULL;
    tab = initTable(t->objName);    //cria a tabela

    if(!verifyFieldName(t->columnName, t->N)){ freeTable(tab); return; }

    for(i = 0; i < t->N; i++){
    	if (t->type[i] == 'S') //String
    		size = atoi(t->values[i]); //t->values contem tamanho das strings na criação, atoi converte esse tamanho(string) para int
    	else if (t->type[i] == 'I') //Inteiro
    		size = sizeof(int);
    	else if (t->type[i] == 'D') //Double
    		size = sizeof(double);
        else if (t->type[i] == 'C') //Char
    		size = sizeof(char);
        //???
    	if (t->attribute[i] == FK) {
    		strncpy(fkTable, t->fkTable[i], TABLE_NAME_SIZE);
    		strncpy(fkColumn, t->fkColumn[i], FIELD_NAME_SIZE);
    	} else {
    		strcpy(fkTable, "");
    		strcpy(fkColumn, "");
    	}

        if((error = addField(tab, t->columnName[i], t->type[i], size, t->attribute[i], fkTable, fkColumn) != SUCCESS)){
            printf("ERROR: %s\n", error == MALLOC_FAILED ? "malloc failed" : "could not initialize the table");
            return; //Por favor, mudem o printf. Não sei escrever!
        }

        //objcmp é a stcmp deles, basicamente transformam tudo para minusculo antes
        if((objcmp(fkTable, "") != 0 || objcmp(fkColumn, "") != 0) && verifyFK(fkTable, fkColumn) == 0){
			printf("ERROR: attribute FK cannot be referenced\n");
            freeTable(tab);
            return;
        }
    }

    printf("%s\n",(finishTable(tab) == SUCCESS) ? "CREATE TABLE" : "ERROR: could not create data file"); //Por favor, mudem o printf. Não sei escrever!
    if (tab != NULL) freeTable(tab);
}

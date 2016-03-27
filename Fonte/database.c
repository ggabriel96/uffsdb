#include <stdio.h>
#include <stdlib.h>
#include <string.h>
////
// #ifndef FMACROS // garante que macros.h não seja reincluída
   #include "macros.h"
// #endif
///
#ifndef FTYPES // garante que types.h não seja reincluída
  #include "types.h"
#endif
////
#ifndef FMISC // garante que misc.h não seja reincluída
  #include "misc.h"
#endif

char connectDB(char *db_name) {
    int i = 0;
	FILE *DB = NULL;
	char name[LEN_DB_NAME], dir[LEN_DB_DIR], valid = 0;

    if ((DB = fopen("data/DB.dat", "rb")) == NULL) {
        return ERRO_ABRIR_ARQUIVO;
    }

    for (i = 0; fread(&valid, sizeof (char), 1, DB) > 0; i++) {
        fread(name, sizeof (char), LEN_DB_NAME, DB);
        fread(dir, sizeof (char), LEN_DB_DIR, DB);
        if (strcmp(name, db_name) == 0 && valid) {
            // atualiza o diretorio do banco que está conectado
            fclose(DB);
            strcpy(connected.db_directory, "data/");
            strcat(connected.db_directory, dir);
            // esse for dá segfault >_>
            // for (i = 0; connected.db_directory[i] != '/'; i++);
        	// strcat(connected.db_directory + i + 1, dir);
        	return SUCCESS;
        }
    }
    fclose(DB);
    return DB_NOT_EXISTS;
}

void createDB(char *dbname) { //Se dbname é NULL, vai ser criado o banco padrão
    FILE *DB;
    int qtdb = 0;
    char name[LEN_DB_NAME], valid, mkdir[LEN_DB_NAME + 27] = "mkdir data/", first = 0;
    if (dbname == NULL) { first = 1; dbname = DEFAULT_DB; }

    // Forço sempre a criação da pasta data
    system("mkdir data > /dev/null 2>&1");

    if ((DB = fopen("data/DB.dat", "a+b")) == NULL) {
        printf("ERROR: could not open data/DB.dat.\n");
        return;
    }

    // Verifica o tamanho do nome e trunca, quando necessário
    if (strlen(dbname) >= LEN_DB_NAME) {
    	printf("WARNING: database name is too long, it will be truncated to %d chars.\n", LEN_DB_NAME - 1);
    	dbname[LEN_DB_NAME - 1] = '\0';
    }

    // Percorre a lista de bancos para ver se o banco que está sendo criado já não existe
    for (qtdb = 0; fread(&valid, sizeof (char), 1, DB) > 0; fseek(DB, LEN_DB_DIR, SEEK_CUR)) {
        fread(name, sizeof (char), LEN_DB_NAME, DB);
        if (valid) qtdb++;
        if (strcasecmp(name, dbname) == 0 && valid) {
            if(!first) printf("ERROR: database already exists\n");
            fclose(DB); return;
        }
    }

    // Se não existe o banco ainda, mas a quantidade de bancos for maior ou igual ao limite, o banco não é criado
    // O first está ali para permitir a criação do banco padrão, mesmo ultrapassando o limite
    if (qtdb >= QTD_DB) {
        printf("ERROR: the limit of %d databases has been reached.\n", QTD_DB);
        fclose(DB); return;
    }

    data_base *SGBD = (data_base *) malloc(sizeof (data_base));;
    SGBD -> valid = 1;
    strcpy(SGBD -> db_name, dbname);
	strcpy(SGBD -> db_directory, dbname);
	strcat(SGBD -> db_directory, "/");
    strcat(mkdir, SGBD -> db_name); strcat(mkdir, "> /dev/null 2>&1");

    //Cria a pasta do banco e só escreve o banco no arquivo se foi possível criar a pasta
    if (system(mkdir) == -1) printf("ERROR: failed to create database %s.\n", SGBD -> db_name);
    else {
        fwrite(SGBD, sizeof (data_base), 1, DB);
        if (!first) printf("CREATE DATABASE\n");
    }
    fclose(DB); free(SGBD); SGBD = NULL;
}

void dropDatabase(char *db_name) {
    int i = 0;
	FILE *DB = NULL;
	char name[LEN_DB_NAME], dir[LEN_DB_DIR], valid = 0;

	if (!strcmp(db_name, connected.db_name)) {
		printf("ERROR: You can not delete the database that you are connected.\n");
		return;
	}

    if ((DB = fopen("data/DB.dat", "r+b")) == NULL) {
        printf("ERROR: could not open 'data/DB.dat' file.\n");
        return;
    }

    for (i = 0; fread(&valid, sizeof (char), 1, DB) > 0; i++) {
        fread(name, sizeof (char), LEN_DB_NAME, DB);
        fread(dir, sizeof (char), LEN_DB_DIR, DB);

        if (!strcmp(name, db_name) && valid) {
        	char directory[LEN_DB_NAME * 2] = "rm -Rf data/";
        	strcat(directory, dir);
        	if (system(directory) != -1) {
                valid = 0;
                fseek(DB, -(LEN_DB_DIR + LEN_DB_NAME + 1), SEEK_CUR);
            	fwrite(&valid, sizeof (char), 1, DB); // do banco e apaga ele
                printf("DROP DATABASE\n");
            }
            else printf("ERROR: could not delete database folder '%s'.", directory);
        	fclose(DB);
        	return;
        }
    }
    fclose(DB);
    printf("ERROR: database does not exist.\n");
}

void showDB() {
	FILE *DB = NULL;
	int i = 0, qtty = 0;
	char name[LEN_DB_NAME], valid = 0;

    if ((DB = fopen("data/DB.dat", "r+b")) == NULL) {
        printf("ERROR: could not open 'data/DB.dat' file.\n");
        return;
    }

    printf("\n List of databases\n");
    for (i = 0; i < LEN_DB_NAME + 1; i++) printf("-");
    printf("\n");
    for (i = 0; fread(&valid, sizeof (char), 1, DB) > 0; i++) {
        fread(name, sizeof (char), LEN_DB_NAME, DB);
        fseek(DB, LEN_DB_DIR, SEEK_CUR);
        if (valid) {
            printf(" %s\n", name);
            qtty++;
        }
    }
    printf("(%d %s)\n\n", qtty, qtty > 1 ? "rows": "row");
    fclose(DB);
}

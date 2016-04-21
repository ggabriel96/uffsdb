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

int seekdb(FILE *DB, char *dbname, char *dbdir) {
    if (DB == NULL) return SEEKDB_NULL_DB;
    if (dbname == NULL) return SEEKDB_NULL_DBNAME;
    int qtty = 0;
    long oldpos = ftell(DB);
    char name[LEN_DB_NAME], dir[LEN_DB_DIR], valid = 0;
    while (fread(&valid, sizeof (char), 1, DB) > 0) {
        fread(name, sizeof (char), LEN_DB_NAME, DB);
        fread(dir, sizeof (char), LEN_DB_DIR, DB);
        if (valid) {
            qtty++;
            if (!strcmp(dbname, name)) {
                fseek(DB, -(LEN_DB_DIR + LEN_DB_NAME + 1), SEEK_CUR);
                if (dbdir == NULL) return SEEKDB_FOUND_NULL;
                else { strcpy(dbdir, dir); return SEEKDB_FOUND; }
            }
        }
    }
    fseek(DB, oldpos, SEEK_SET);
    return qtty;
}

int connectDB(char *dbname) {
    int status = 0;
    char *dbdir = NULL;
    FILE *DB = fopen("data/DB.dat", "rb");
    if (DB == NULL) return ERROR_OPEN_FILE;
    dbdir = (char *) malloc(sizeof (char) * LEN_DB_DIR);
    status = seekdb(DB, dbname, dbdir);
    fclose(DB);
    if (status == SEEKDB_FOUND) {
        strcpy(connected.db_directory, "data/");
        strcat(connected.db_directory, dbdir);
        status = SUCCESS;
    }
    else if (status == SEEKDB_FOUND_NULL) status = MALLOC_FAILED;
    else status = DB_NOT_EXISTS;
    free(dbdir); dbdir = NULL;
    return status;
}

void createDB(char *dbname) { //Se dbname é NULL, vai ser criado o banco padrão
    int qtdb = 0;
    FILE *DB = NULL;
    data_base *SGBD = NULL;
    // 27 = |mkdir data/| (11) +  |> /dev/null 2>&1|(16)
    char mkdir[LEN_DB_NAME + 27] = "mkdir data/", first = 0;
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

    qtdb = seekdb(DB, dbname, NULL);
    if (qtdb == SEEKDB_FOUND_NULL && !first) printf("ERROR: database '%s' already exists.\n", dbname);
    // Se não existe o banco ainda, mas a quantidade de bancos
    // for maior ou igual ao limite, o banco não é criado
    // O first está ali para permitir a criação do banco padrão,
    // mesmo ultrapassando o limite
    else if (qtdb >= DATABASE_AMOUNT_LIMIT && !first) printf("ERROR: the limit of %d databases has been reached.\n", DATABASE_AMOUNT_LIMIT);
    else if (qtdb >= 0) {
        SGBD = (data_base *) malloc(sizeof (data_base));;
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
    }
    else if(!first) printf("ERROR: failed to create database \"%s\" (error code: %d).\n", dbname, qtdb);
    fclose(DB);
    free(SGBD); SGBD = NULL;
}

void dropDatabase(char *dbname) {
    int status = 0;
    char valid = 0;
	FILE *DB = NULL;
    char *dbdir = NULL;

	if (!strcmp(dbname, connected.db_name)) {
		printf("ERROR: You can not delete the database that you are connected.\n");
		return;
	}

    if ((DB = fopen("data/DB.dat", "r+b")) == NULL) {
        printf("ERROR: could not open 'data/DB.dat' file.\n");
        return;
    }

    dbdir = (char *) malloc(sizeof (char) * LEN_DB_DIR);
    status = seekdb(DB, dbname, dbdir);
    if (status == SEEKDB_FOUND) {
        char directory[LEN_DB_NAME * 2] = "rm -Rf data/";
        strcat(directory, dbdir);
        if (system(directory) != -1) {
            valid = 0;
            fwrite(&valid, sizeof (char), 1, DB); // do banco e apaga ele
            printf("DROP DATABASE\n");
        }
        else printf("ERROR: could not delete database folder '%s'.", directory);
    }
    else if (status == -2) printf("ERROR: could not allocate memory for database directory path.\n");
    else if (status > 0) printf("ERROR: database '%s' does not exist.\n", dbname);
    else printf("ERROR: could not delete database '%s' (error code: %d).\n", dbname, status);
    fclose(DB);
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
    while (fread(&valid, sizeof (char), 1, DB) > 0) {
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

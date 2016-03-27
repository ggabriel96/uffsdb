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
    int i;
	FILE *DB;
	char vec_name[QTD_DB][LEN_DB_NAME], vec_directory[QTD_DB][LEN_DB_NAME], valid, directory[LEN_DB_NAME * 2] = "data/";

    if ((DB = fopen("data/DB.dat", "rb")) == NULL) {
        return ERRO_ABRIR_ARQUIVO;
    }

    for (i = 0; fgetc(DB) != EOF; i++) {
        // o fgetc na condição do for anda um char no arquivo
        // então tem que fazer ele voltar esse char, caso não seja EOF
    	fseek(DB, -1, SEEK_CUR);
    	fread(&valid, sizeof (char), 1, DB);
        fread(vec_name[i], sizeof (char), LEN_DB_NAME, DB);
        fread(vec_directory[i], sizeof (char), LEN_DB_NAME, DB);
        // verifica se encontrou o banco
        if (strcasecmp(vec_name[i], db_name) == 0 && valid) {
            // atualiza o diretorio do banco que está conectado
    		strcat(directory, vec_directory[i]);
        	strcpylower(connected.db_directory, directory);
        	fclose(DB);
        	return SUCCESS;
        }
    }
    fclose(DB);
    return DB_NOT_EXISTS;
}

void createDB(char *db_name) {
    int i;
	FILE *DB;
	char vec_name[QTD_DB][LEN_DB_NAME], vec_directory[QTD_DB][LEN_DB_DIR], create[LEN_DB_NAME + 12] = "mkdir data/", valid;

    if ((DB = fopen("data/DB.dat", "a+b")) == NULL) {
        printf("ERROR: could not open data/DB.dat.\n");
        return;
    }

    if (strlen(db_name) > LEN_DB_NAME) {
    	printf("WARNING: database name is too long, it will be truncated to %d chars.\n", LEN_DB_NAME);
    	db_name[LEN_DB_NAME] = '\0';
    }

    for (i = 0; fgetc(DB) != EOF; i++) {
        // o fgetc na condição do for anda um char no arquivo
        // então tem que fazer ele voltar esse char, caso não seja EOF
    	fseek(DB, -1, SEEK_CUR);
    	fread(&valid, sizeof (char), 1, DB);
        fread(vec_name[i], sizeof (char), LEN_DB_NAME, DB);
        fread(vec_directory[i], sizeof (char), LEN_DB_DIR, DB);
        if (strcasecmp(vec_name[i], db_name) == 0 && valid) {
        	fclose(DB);
			if (strcasecmp(db_name, "uffsdb") != 0)
        		printf("ERROR: database already exists.\n");
            return;
        }
    }

    // >= porque o 'i' vai parar uma posição
    // após a quantidade de bancos existentes
    if (i >= QTD_DB) {
    	fclose(DB);
        printf("ERROR: the limit of %d databases has been reached.\n", QTD_DB);
        return;
    }

    data_base *SGBD = (data_base *) malloc(sizeof (data_base));;
    SGBD -> valid = 1;
    strcpylower(SGBD -> db_name, db_name);
	strcpylower(SGBD -> db_directory, db_name);
	strcat(SGBD -> db_directory, "/");
	fwrite(SGBD, sizeof (data_base), 1, DB);

    strcat(create, SGBD -> db_name);
    // verifica se foi possivel criar o diretório
	if (system(create) == -1) {
		printf("ERROR: failed to create database %s.\n", SGBD -> db_name);
		fclose(DB); free(SGBD); SGBD = NULL;
		return;
	}

    fclose(DB);
    free(SGBD);
    SGBD = NULL;

    if (strcasecmp(db_name, "uffsdb") != 0) printf("CREATE DATABASE\n");
}

void dropDatabase(char *db_name) {
    int i;
	FILE *DB;
	char vec_name[QTD_DB][LEN_DB_NAME], vec_directory[QTD_DB][LEN_DB_NAME], valid;

	if(!strcmp(db_name, connected.db_name)) {
		printf("ERROR: You can not delete the database that you are connected.\n");
		return;
	}

    if((DB = fopen("data/DB.dat","r+b")) == NULL) {
       	printf("ERROR: cannot open file\n");
		return;
    }

    for(i = 0; fgetc(DB) != EOF; i++) {
    	fseek(DB, -1, 1);
    	fread(&valid, sizeof(char), 1, DB);
        fread(vec_name[i], sizeof(char), LEN_DB_NAME, DB);
        fread(vec_directory[i], sizeof(char), LEN_DB_DIR, DB);

        if(!objcmp(vec_name[i], db_name) && valid) {
        	valid = 0;
        	// fseek(DB, ((LEN_DB_NAME*2+1)*i), SEEK_SET); 	// posiciona o cabecote sobre o byte de validade -> FOrmula antiga
        	fseek(DB, (LEN_DB_NAME + LEN_DB_DIR + 1) * i, SEEK_SET); 	// posiciona o cabecote sobre o byte de validade
        	fwrite(&valid, sizeof(char), 1, DB);			   // do banco e apaga ele

        	char directory[LEN_DB_NAME * 2] = "rm data/";
        	strcat(directory, vec_directory[i]);
        	strcat(directory, " -R");

        	system(directory);

        	fclose(DB);
        	printf("DROP DATABASE\n");
        	return;
        }
    }
    fclose(DB);
    printf("ERROR: database does not exist\n");
}

void showDB() {
	FILE *DB;
	int i, qtdDB=0;
	char vec_name[QTD_DB][LEN_DB_NAME], vec_directory[QTD_DB][LEN_DB_DIR], valid;

    if((DB = fopen("data/DB.dat","r+b")) == NULL) {
       	printf("ERROR: cannot open file\n");
		return;
    }

    printf(" List of databases:\n");
    for (i = 0; i < LEN_DB_NAME; i++) printf("-");
    printf("\n");
    for (i = 0; fgetc(DB) != EOF; i++) {
    	fseek(DB, -1, 1);
    	fread(&valid, sizeof(char), 1, DB);
        fread(vec_name[i], sizeof(char), LEN_DB_NAME, DB);
        fread(vec_directory[i], sizeof(char), LEN_DB_DIR, DB);

        if (valid) {
            printf("%s\n", vec_name[i]);
            qtdDB++;
        }
    }

    printf("(%d %s)\n\n", qtdDB, qtdDB > 1 ? "rows": "row");
    fclose(DB);
}

void dbInit(char *db) {
	char *name;
    // here in the system command, 0, 1 and 2 stand for
    // STDIN, STDOUT and STDERR, respectively.
    // "> /dev/null 2>&1" is redirecting "mkdir data" STDOUT
    // to /dev/null and STDERR to STDOUT
	if (system("mkdir data > /dev/null 2>&1") == -1)
		printf("ERROR: It was not possible to initialize uffsdb.\n");
    else if (db == NULL)
        name = "uffsdb";
    else name = db;
	createDB(name);
}

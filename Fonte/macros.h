#include <math.h>

#define FMACROS 0 // flag para identificar se macros.h já foi incluída

// ERRORS
#define DB_EXISTS 1
#define DB_NOT_EXISTS 24
#define ERROR_OPEN_FILE -6
// #define ERROR_OPEN_DICTIONARY -11   //---Não usado
#define ERROR_OPEN_SCHEMA NULL
#define BUFFER_ALREADY_FULL -10
#define ERROR_FOREIGN_KEY -15
#define ERROR_PRIMARY_KEY -14
// #define ERROR_CREATE_TABLE -16  //---Não usado
#define ALLOCATION_ERROR NULL
#define READING_ERROR NULL
#define PARAMETER_ERROR_1 -5
#define ERROR_STRUCTURE_TABLE_NULL 74
#define PRINT_ERROR -8
#define INSERT_ERROR NULL
#define READING_DATA_ERROR -9
#define STRING_LENGTH_ERROR -2
#define ERROR_EXPECTED_CHAR -12
#define ERROR_EXPECTED_DOUBLE -13
#define ERROR_EXPECTED_INTEGER -1
#define	FIELD_NAME_ERROR -3
#define TABLE_NAME_ERROR -4
#define INVALID_TABLE_NAME NULL
#define INVALID_PAGE NULL
#define PARAMETER_ERROR_2 NULL
// #define DELETE_FILE_ERROR -17   //---Não usado
#define DELETE_OBJECT_FILE_ERROR -18
#define DELETE_SCHEMA_FILE_ERROR -19
// #define EMPTY_ERROR -7   //---Não usado
#define MALLOC_FAILED 73
#define SUCCESS 0
#define ERROR -1

// seekdb
#define SEEKDB_FOUND -1
#define SEEKDB_FOUND_NULL -2
#define SEEKDB_NULL_DB -3
#define SEEKDB_NULL_DBNAME -4

#define FK 2
#define NPK 0
#define PK 1

#define DEFAULT_DB "uffsdb"
#define LEN_DB_NAME 21
#define LEN_DB_DIR LEN_DB_NAME + 1
#define PAGES 1024          // Número de páginas.
#define DATABASE_AMOUNT_LIMIT 100
#define SIZE 512            // Tamanho da página.
#define FIELD_NAME_SIZE 40  // Tamanho do nome dos campos de uma tabela.
#define TABLE_NAME_SIZE 20  // Tamanho do nome da tabela.
#define FILE_NAME_SIZE 20   // Tamanho do nome do arquivo.

#define LE 0
#define LT 1
#define EQ 2
#define NE 3
#define GE 4
#define GT 5
#define AND 6
#define OR 7

#define STRING 8
#define COLUMN 9
#define NUM 10

#define EPS 1e-9
#define igualDouble(x,y) (fabs ((x) - (y)) < EPS)
#define menorIgualDouble(x,y) ((x) < (y) || igualDouble(x,y))
#define maiorIgualDouble(x,y) ((x) > (y) || igualDouble(x,y))

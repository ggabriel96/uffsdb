#define FSQLCOMMANDS 1   // flag para garantir a não reinclusão

#ifndef FMACROS // garante que macros.h não seja reincluída
   #include "macros.h"
#endif
///
#ifndef FTYPES // garante que types.h não seja reincluída
  #include "types.h"
#endif
/* insert: Recebe uma estrutura rc_insert e valida os tokens encontrados pela interface().
 *         Se os valores forem válidos, insere um novo valor.
 */
void insert(rc_insert *);
int allColumnsExists(rc_insert *, table *);
char getInsertedType(rc_insert *, char *c, table *);
char *getInsertedValue(rc_insert *, char *, table *);
int typesCompatible(char , char);
/*
    Esta função finaliza a inserção de valores em uma tabela. Assume que o usuário entrou com todos
    os campos de uma tupla completa.
    Retorna:
        -> ERROR_OPEN_FILE, quando ocorreu um erro ao abrir o arquivo fs_object.dat ou fs_schema.dat;
        -> STRING_LENGTH_ERROR, quando ocorreu um erro no tamanho da string inserida;
        -> FIELD_NAME_ERROR, quando o nome do campo passado na estrutura;
        -> ERROR_EXPECTED_INTEGER, quando o valor passado não é um inteiro;
        -> ERROR_EXPECTED_DOUBLE, quando o valor passado não é um double;
        -> ERROR_EXPECTED_CHAR, quando o valor passado é maior que 1byte;
        -> SUCCESS, quando a função teve sucesso em sua operação de inserção na tabela.
    *nome - Nome da tabela que vai ser inserido os valores da estrutura *c.
    *c - Estrutura com o valores que vão ser inseridos na tabela *nome.
*/
int finishInsert(char *, column *);
/* ----------------------------------------------------------------------------------------------
    Objetivo:   Utilizada para impressão de tabelas.
    Parametros: Nome da tabela (char).
    Retorno:    void.
   ---------------------------------------------------------------------------------------------*/
void printing(char [] );
/* ----------------------------------------------------------------------------------------------
    Objetivo:   Função para exclusão de tabelas.
    Parametros: Nome da tabela (char).
    Retorno:    INT
                SUCCESS,
                DELETE_OBJECT_FILE_ERROR,
                DELETE_SCHEMA_FILE_ERROR,
                READING_DATA_ERROR.
   ---------------------------------------------------------------------------------------------*/
int deleteTable(char *);
/* ----------------------------------------------------------------------------------------------
    Objetivo:   Copia todas as informações menos a tabela do objeto, que será removida.
    Parametros: Objeto que será removido do schema.
    Retorno:    INT
                SUCCESS,
                DELETE_SCHEMA_FILE_ERROR
   ---------------------------------------------------------------------------------------------*/
int searchSchemaFile(struct fs_objects);
/* ----------------------------------------------------------------------------------------------
    Objetivo:   Gera as verificações em relação a chave FK.
    Parametros: Nome da Tabela, Coluna C, Nome do Campo, Valor do Campo, Tabela Apontada e Atributo Apontado.
    Retorno:    INT
                SUCCESS,
                PARAMETER_ERROR_1,
                ERROR_FOREIGN_KEY
   ---------------------------------------------------------------------------------------------*/
int verifyForeignKey(char *, column *, char *, char *, char *, char *);

/* ----------------------------------------------------------------------------------------------
    Objetivo:   Gera as verificações em relação a chave pK.
    Parametros: Nome da Tabela, Coluna C, Nome do Campo, Valor do Campo
    Retorno:    INT
                SUCCESS,
                PARAMETER_ERROR_1,
                ERROR_PRIMARY_KEY
   ---------------------------------------------------------------------------------------------*/
int verifyPrimaryKey(char *, column *, char *, char *);
/* ----------------------------------------------------------------------------------------------
    Objetivo:   Inicializa os atributos necessários para a verificação de FK e PK.
    Parametros: Objeto da tabela, Tabela, Buffer e nome da tabela.
    Retorno:    INT
                SUCCESS,
                PARAMETER_ERROR_1,
   ---------------------------------------------------------------------------------------------*/
int initAttributes(struct fs_objects *, tp_table **, tp_buffer **, char *);
/* ----------------------------------------------------------------------------------------------
    Objetivo:   Recebe o nome de uma tabela e engloba as funções readObject() e readSchema().
    Parametros: Nome da Tabela, Objeto da Tabela e tabela.
    Retorno:    tp_table
   ---------------------------------------------------------------------------------------------*/
tp_table * openTable(char *, struct fs_objects *, tp_table **);
/* createTable: Recebe uma estrutura contendo os parâmetros do CREATE TABLE
 *              e cria uma tabela no banco corrente
 */
void createTable(rc_insert *);
////
int verifyFieldName(char **, int );
////
int verifyFK(char *, char *);

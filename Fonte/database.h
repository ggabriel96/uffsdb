// Procura pelo banco de nome 'dbname' no arquivo 'DB'
// e coloca no ponteiro 'dbdir' o diretório do banco, caso
// o tenha encontrado. 'DB' deve ter passado previamente
// por fopen e 'dbdir' deve ter sido previamente alocado
// Retorna:
// - SEEKDB_NULL_DB, caso DB seja NULL;
// - SEEKDB_NULL_DBNAME, caso dbname seja NULL;
// - SEEKDB_FOUND caso tenha encontrado o banco ou
// SEEKDB_FOUND_NULL caso tenha encontrado e dbdir é NULL
// (voltando à posição inicial do registro encontrado, ou seja, &valid);
// - a quantidade total de bancos válidos, caso não tenha
// encontrado (retornando à posição original do arquivo DB).
int seekdb(FILE *, char *, char *);

/* createDB: Recebe um nome para criar um banco de dados
 *           cria o banco, caso ainda não exista
 */
void createDB(char *);

/* connectDB: Recebe o nome de um banco de dadados
 *            conecta-se com o banco, caso exista
 */
int connectDB(char *);

/* dbInit: cria o banco de dados padrão
 *         com o nome "ibetres", caso não exista
 */
void dbInit();

/* dropDatabase: Recebe o nome de um banco de dados
 *               deleta o banco caso o banco exista
 *               e o usuário esteja conectado em outro banco
 */
void dropDatabase(char *);

/* showDB: lista todos os bancos do SGBD "\l"
 *
 */
void showDB();
/* interface: Parte gerada pelo yacc, presente no arquivo yacc.y
 *            É responsável por fazer a conexão ao banco e chamar yyparse()
 *            para fazer a validação dos comandos enviados pelo usuário.
 */
int interface();

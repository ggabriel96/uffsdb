// Procura pelo banco de nome 'dbname' no arquivo 'DB'
// e coloca no ponteiro 'dbdir' o diretório do banco, caso
// o tenha encontrado. 'DB' deve ter passado previamente
// por fopen e 'dbdir' deve ter sido previamente alocado
// retorna:
// -1, caso DB seja NULL
// -2, caso dbname seja NULL
// -3, caso dbdir seja NULL
//  0, caso não tenha entrado o banco (e retorna à posição original do arquivo DB)
//  1, caso tenha encontrado (voltando à posição inicial do registro encontrado, &valid)
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

# uffsdb
A simple teaching DBMS

# How to compile
    // on Ubuntu and derivatives:
    sudo apt-get install byacc flex
    cd path/to/uffsdb/Fonte
    make

# How to run
    path/to/uffsdb/Fonte/uffsdb

# The compiler
uffsdb commands are interpreted using `yacc` and `lex`.
In the `interface` folder, type `make` to compile both.
You can edit the following files: `parser.h`, `parser.c`, `lex.l`, and `yacc.y`.

	
# ERROS IDENTIFICADOS E NÃO CONSERTADOS

1) São exibidas mensagens de erros repetidas, em várias linhas. Acontece ao digitar uma sintaxe errada. Como por exemplo:

    uffsdb=# create databasea baba;
    ERROR: syntax error.
    uffsdb=# ERROR: syntax error. 

2) Não reconhece símbolos digitados que não as letras ou sentenças sql, como por exemplo  '[' e ']' e printa algumas coisas que não deveria quando isso acontece.

    uffsdb=# ;;;;;;;
    uffsdb=# uffsdb=# uffsdb=# uffsdb=# uffsdb=# uffsdb=# uffsdb=#
    uffsdb=# [[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
    [[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[uffsdb=#

3) Erros que não existem são mostrados, como por exemplo ao inserir tuplas em uma tabela que não existe mais: não é necessário dizer que a linha do insert está errada, basta dizer que a tabela não existe.

    uffsdb=# drop table tbl;
    uffsdb=# insert into tbl values (1);
    ERROR: relation "tbl" was not found.
    ERROR: INSERT has more expressions than target columns.

4) Ao ser criada uma tabela com um campo do tipo varchar maior ou igual a 1000, a tabulação não funciona direito, empurrando os demais campos para a direita. Como no exemplo:

    uffsdb=# create table animal (string1 varchar(1001), string2 varchar(3) );
    CREATE TABLE
    uffsdb=# \t animal
    	  Table "public.animal"
     Column             | Type         | Modifiers
    --------------------+--------------+-----------
      string1           |  varchar(1001) | null
      string2           |  varchar(3) | null 

5) Ao se digitar várias aspas simples faz com que o banco pare de funcionar corretamente, algumas vezes esperando que você digite um número par de aspas simples, as vezes não fazendo mais nada que é requisitado.
	

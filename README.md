Students: 1411100001 - Vladimir Belinski
          1411100003 - Gabriel Batista Galli
          1411100031 - Acácia dos Campos da Terra
          1411100044 - Harold Cristien Santos Becker
          1411100046 - João Pedro Winckler Bernardi

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


# Erros identificados e não corrigidos

1) São exibidas mensagens de erros repetidas, em várias linhas. Acontece ao se digitar uma sintaxe errada, como por exemplo:

    uffsdb=# create databasea baba;
    ERROR: syntax error.
    uffsdb=# ERROR: syntax error.

2) Não reconhece símbolos digitados que não as letras ou sentenças sql, como por exemplo  '[' e ']', imprimindo na tela caracteres indesejados quando isso acontece.

    uffsdb=# ;;;;;;;
    uffsdb=# uffsdb=# uffsdb=# uffsdb=# uffsdb=# uffsdb=# uffsdb=#
    uffsdb=# [[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
    [[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[uffsdb=#

3) Erros que não existem são mostrados, por exemplo, ao se inserir tuplas em uma tabela que não existe mais: não é necessário dizer que a linha do insert está errada, basta dizer que a tabela não existe.

    uffsdb=# drop table tbl;
    uffsdb=# insert into tbl values (1);
    ERROR: relation "tbl" was not found.
    ERROR: INSERT has more expressions than target columns.

4) Ao ser criada uma tabela com um campo do tipo varchar maior ou igual a 1000 a tabulação não funciona corretamente, deslocando os demais campos para a direita. Como no exemplo:

    uffsdb=# create table animal (string1 varchar(1001), string2 varchar(3) );
    CREATE TABLE
    uffsdb=# \t animal
    	  Table "public.animal"
     Column             | Type         | Modifiers
    --------------------+--------------+-----------
      string1           |  varchar(1001) | null
      string2           |  varchar(3) | null

5) Digitar várias aspas simples faz com que o banco pare de funcionar corretamente; algumas vezes espera que seja digitado um número par de aspas simples, em outras não faz mais nada do que é requisitado.

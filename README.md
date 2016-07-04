# Students
    1411100001 - Vladimir Belinski
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

# Alterações

Primeiramente, cabe destacar que foram reescritas as funções presentes no arquivo "database.c", pois apresentavam muitas falhas.

Também foi realizada alteração de forma com que agora é necessário somente realizar um make na pasta 'Fonte' para compilar o projeto inteiro.

As funcionalidades principais implementadas foram: (a) implementação da cláusula WHERE (com a possibilidade de se utilizar os operadores relacionais =, <, >, !=, >= e <=; e os operadores lógicos AND e OR) do SELECT; (b) projeção.

Em relação à seleção, foi utilizada a seguinte notação: SELECT * | expression FROM table [ WHERE condition ].
Onde: 'table' corresponde ao nome de uma tabela;
			'expression' corresponde a uma ou mais colunas da tabela 'table' separadas por vírgula;
			'condition' corresponde a uma ou mais condições que podem conter os operadores relacionais =,<,>,!=,>= e <= e os operadores lógicos AND e   OR, sendo que os operandos dos operadores lógicos devem ser resultados de expressões que utilizaram operadores relacionais.

Para resolver linearmente a expressão da cláusula WHERE foi implementada a função shuntingYard(char **, int n), que recebe como parâmetros uma lista de tokens montada pelo lex e yacc e a quantidade de tokens e retorna um vetor com os tokens organizados em notação polonesa reversa. É importante destacar que caso não existam tuplas a seleção retorna uma mensagem ao usuário.

Por sua vez, em relação a projeção foram realizadas alterações na função printing() do arquivo "sqlcomands.c", as quais implicam em funcionalidades como:
	- A projeção será exibida na tela de acordo com a ordem que foi solicitada pelo usuário, ou seja, se foi feito o comando "select d, b, a from table;" o resultado será exibido com as colunas na ordem: d, b e a.

O processo planejado para projeção consiste em: receber como parâmetros os tokens separados pelo lex, chamar a função shuntingYard(), que por sua vez retornará um vetor contendo os tokens em notação polonesa reversa. Após isto, será verificada toda tupla contida na tabela, para saber se a condição do select é respeitada por aquela tupla. Em caso positivo, a tupla será exibida na tela de acordo com a ordem solicitada.

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

6) Há erro na inserção de tuplas nas tabelas, causando falha de segmentação. Um exemplo de inserção incorreta é:

create table migs (ql integer, mr double, nm varchar(30) primary key);
insert into migs values (1, 43.98, 'None');
insert into migs values (2, 549.10, 'Rel');
insert into migs values (2442, 345654.877, 'Harold');

A primeira inserção ocorre, a segunda já causa falha de segmentação ou double free or corruption error.

7) Há erro no momento de criar as tabelas. É criada com nome de coluna incorreto, ou nem é criada a coluna desejada. Um exemplo de criação incorreta de tabela é:
  - Causar o erro acima citado (6);
  - Conectar novamente ao banco;
  - Criar a tabela Funcionarios, a qual se encontra no arquivo "PopulaçãoVolumosa.txt" e preencher a tabela com as tuplas encontradas no arquivo;
  - Apagar a tabela migs com o comando "drop table migs;";
  - Recriar a tabela migs;
  - Verificar a estrutura da tabela com o comando "\t migs";
Será verificado que em sua estrutura consta os campos da tabela Funcionarios, ao invés dos campos definidos no create table.

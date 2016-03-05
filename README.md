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

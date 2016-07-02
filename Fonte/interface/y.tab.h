/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_INTERFACE_Y_TAB_H_INCLUDED
# define YY_YY_INTERFACE_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INSERT = 258,
    INTO = 259,
    VALUES = 260,
    SELECT = 261,
    FROM = 262,
    WHERE = 263,
    LOP = 264,
    COP = 265,
    CREATE = 266,
    TABLE = 267,
    INTEGER = 268,
    VARCHAR = 269,
    DOUBLE = 270,
    CHAR = 271,
    PRIMARY = 272,
    KEY = 273,
    REFERENCES = 274,
    DATABASE = 275,
    DROP = 276,
    OBJECT = 277,
    NUMBER = 278,
    VALUE = 279,
    QUIT = 280,
    LIST_TABLES = 281,
    LIST_TABLE = 282,
    ALPHANUM = 283,
    CONNECT = 284,
    HELP = 285,
    LIST_DBASES = 286,
    CLEAR = 287,
    CONTR = 288
  };
#endif
/* Tokens.  */
#define INSERT 258
#define INTO 259
#define VALUES 260
#define SELECT 261
#define FROM 262
#define WHERE 263
#define LOP 264
#define COP 265
#define CREATE 266
#define TABLE 267
#define INTEGER 268
#define VARCHAR 269
#define DOUBLE 270
#define CHAR 271
#define PRIMARY 272
#define KEY 273
#define REFERENCES 274
#define DATABASE 275
#define DROP 276
#define OBJECT 277
#define NUMBER 278
#define VALUE 279
#define QUIT 280
#define LIST_TABLES 281
#define LIST_TABLE 282
#define ALPHANUM 283
#define CONNECT 284
#define HELP 285
#define LIST_DBASES 286
#define CLEAR 287
#define CONTR 288

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 38 "interface/yacc.y" /* yacc.c:1909  */

    int intval;
    double floatval;
    int subtok;
    char *strval;

#line 127 "interface/y.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_INTERFACE_Y_TAB_H_INCLUDED  */

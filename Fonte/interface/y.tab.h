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
    LE = 264,
    LT = 265,
    EQ = 266,
    NE = 267,
    GE = 268,
    GT = 269,
    AND = 270,
    OR = 271,
    CREATE = 272,
    TABLE = 273,
    INTEGER = 274,
    VARCHAR = 275,
    DOUBLE = 276,
    CHAR = 277,
    PRIMARY = 278,
    KEY = 279,
    REFERENCES = 280,
    DATABASE = 281,
    DROP = 282,
    OBJECT = 283,
    NUMBER = 284,
    VALUE = 285,
    QUIT = 286,
    LIST_TABLES = 287,
    LIST_TABLE = 288,
    ALPHANUM = 289,
    CONNECT = 290,
    HELP = 291,
    LIST_DBASES = 292,
    CLEAR = 293,
    CONTR = 294
  };
#endif
/* Tokens.  */
#define INSERT 258
#define INTO 259
#define VALUES 260
#define SELECT 261
#define FROM 262
#define WHERE 263
#define LE 264
#define LT 265
#define EQ 266
#define NE 267
#define GE 268
#define GT 269
#define AND 270
#define OR 271
#define CREATE 272
#define TABLE 273
#define INTEGER 274
#define VARCHAR 275
#define DOUBLE 276
#define CHAR 277
#define PRIMARY 278
#define KEY 279
#define REFERENCES 280
#define DATABASE 281
#define DROP 282
#define OBJECT 283
#define NUMBER 284
#define VALUE 285
#define QUIT 286
#define LIST_TABLES 287
#define LIST_TABLE 288
#define ALPHANUM 289
#define CONNECT 290
#define HELP 291
#define LIST_DBASES 292
#define CLEAR 293
#define CONTR 294

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

#line 139 "interface/y.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_INTERFACE_Y_TAB_H_INCLUDED  */

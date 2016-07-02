#ifndef lint
static const char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20140101

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)

#define YYPREFIX "yy"

#define YYPURE 0

#line 2 "interface/yacc.y"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#ifndef FMACROS
   #include "../macros.h"
#endif
#ifndef FTYPES
   #include "../types.h"
#endif
#ifndef FMISC
   #include "../misc.h"
#endif
#ifndef FDICTIONARY
   #include "../dictionary.h"
#endif
#ifndef FSQLCOMMANDS
   #include "../sqlcommands.h"
#endif
#ifndef FDATABASE
   #include "../database.h"
#endif
#ifndef FPARSER
   #include "parser.h"
#endif

extern char* yytext[];
extern FILE * yyin;
extern FILE* outFile_p;

int yywrap() {
    return 1;
}

#line 38 "interface/yacc.y"
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union {
    int intval;
    double floatval;
    int subtok;
    char *strval;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
#line 68 "interface/y.tab.c"

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

#define INSERT 257
#define INTO 258
#define VALUES 259
#define SELECT 260
#define FROM 261
#define WHERE 262
#define LOP 263
#define COP 264
#define CREATE 265
#define TABLE 266
#define INTEGER 267
#define VARCHAR 268
#define DOUBLE 269
#define CHAR 270
#define PRIMARY 271
#define KEY 272
#define REFERENCES 273
#define DATABASE 274
#define DROP 275
#define OBJECT 276
#define NUMBER 277
#define VALUE 278
#define QUIT 279
#define LIST_TABLES 280
#define LIST_TABLE 281
#define ALPHANUM 282
#define CONNECT 283
#define HELP 284
#define LIST_DBASES 285
#define CLEAR 286
#define CONTR 287
#define YYERRCODE 256
static const short yylhs[] = {                           -1,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    9,
   18,   10,   16,   12,   13,    7,    8,   15,   14,   17,
   20,    1,   11,   19,   21,   21,   23,   23,   24,   22,
   22,   25,   25,   25,   26,    3,   27,   27,   29,   31,
   29,   29,   29,   28,   30,   30,   30,   32,   33,   34,
   35,    5,   36,   37,    4,   38,   39,    6,   41,    2,
   43,   43,   40,   40,   45,   45,   45,   45,   46,   47,
   42,   44,   44,   44,   44,   44,   44,   44,   44,   44,
   48,   48,
};
static const short yylen[] = {                            2,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    0,    2,
    1,    1,    1,    1,    1,    2,    1,    1,    1,    1,
    0,   10,    1,    1,    0,    3,    1,    3,    1,    1,
    3,    1,    1,    1,    0,    8,    3,    5,    1,    0,
    5,    1,    1,    1,    0,    2,    5,    1,    1,    0,
    0,    6,    0,    0,    6,    0,    0,    6,    0,    7,
    0,    2,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    3,    3,    3,    3,    3,    3,    3,    3,    3,
    3,    3,
};
static const short yydefred[] = {                         0,
    0,   69,    0,    0,   21,   22,   27,    0,    0,   29,
   28,   23,   30,   24,   25,   33,    0,    1,    2,    3,
    4,    5,    6,    7,    8,    9,   10,   11,   12,   13,
   14,   15,   16,   17,   18,   31,    0,   45,   63,   60,
   66,   26,   20,    0,   39,   74,   73,    0,    0,    0,
    0,    0,    0,   34,    0,    0,    0,    0,   64,   61,
   67,    0,    0,   38,   81,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   54,    0,    0,   65,   62,   68,
   36,    0,   75,   78,   77,   76,    0,   72,    0,    0,
   70,    0,   49,   50,   52,   53,    0,   43,   42,   44,
    0,    0,    0,   80,   79,    0,    0,    0,    0,   46,
    0,    0,    0,    0,    0,    0,   82,   85,    0,   86,
    0,   89,    0,   90,    0,    0,   56,   58,    0,    0,
   32,   41,    0,    0,   48,   51,   59,    0,   57,
};
static const short yydgoto[] = {                         17,
   18,   19,   20,   21,   22,   23,   24,   25,   26,   27,
   28,   87,   30,   31,   32,   33,   34,   35,   55,   44,
   63,  101,   47,   48,  102,   50,   76,   77,   97,  114,
  111,  129,  138,   52,   69,   51,   68,   53,   70,   49,
   37,   66,   74,   88,   89,  106,  107,   90,
};
static const short yysindex[] = {                       -40,
 -216,    0, -244, -230,    0,    0,    0, -221, -220,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  -39,    0,    0,    0,
    0,    0,    0, -218,    0,    0,    0,    9, -202, -218,
 -215, -214, -213,    0,   24, -211, -210,   24,    0,    0,
    0, -211, -191,    0,    0, -193, -206,   12,   12,   12,
   31,   24,  -28,   12,    0,   31, -222,    0,    0,    0,
    0, -239,    0,    0,    0,    0,  -28,    0, -223, -223,
    0,   12,    0,    0,    0,    0, -250,    0,    0,    0,
   31,   29,   31,    0,    0,  -28,  -28,  -28,  -28,    0,
   24, -197, -200,   33,   12, -239,    0,    0, -223,    0,
 -223,    0, -223,    0, -223, -199,    0,    0,   39, -206,
    0,    0,   31, -196,    0,    0,    0,   40,    0,
};
static const short yyrindex[] = {                        82,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  -37,    0,    0,
    0,    0,    0,    0, -176,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   25,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  -24,    0,    0,    0,
    0,   44,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   45,    0,    0,    0,    0,  -30,    0,
  -27,    0,  -26,    0,  -25,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,
};
static const short yygindex[] = {                         0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  -61,    2,  -66,    0,    0,    0,    0,    0,   37,    0,
    0,  -23,  -38,    0,    0,    0,  -42,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  -81,  -57,   -1,    1,    0,
};
#define YYTABLESIZE 254
static const short yytable[] = {                         14,
   15,   29,   46,   37,   81,  103,   78,   79,   80,   92,
   83,   14,   91,   84,   87,   88,   55,   64,   16,   55,
  112,   38,  113,   71,  118,  120,  122,  124,   83,   39,
  110,   84,   87,   88,  115,   40,  117,   98,   99,  104,
  105,   36,  100,   41,   93,   94,   95,   96,  119,  121,
  123,  125,   56,  131,   42,   43,   62,   54,   57,   67,
   59,   60,   61,   14,   45,   65,  136,   72,   73,   75,
   16,   15,  116,   82,  127,  128,  130,  133,  134,  137,
  139,   19,   35,   71,   40,   47,   58,  135,  108,    0,
  109,    0,  132,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  126,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    1,    0,    0,    2,
    0,    0,    0,   37,    3,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    4,    5,   45,    0,    6,    7,
    8,    0,    9,   10,   11,   12,   13,   83,   84,   85,
    0,    0,    0,   86,
};
static const short yycheck[] = {                         40,
   41,    0,   42,   41,   71,   87,   68,   69,   70,   76,
   41,   40,   74,   41,   41,   41,   41,   56,   59,   44,
  271,  266,  273,   62,  106,  107,  108,  109,   59,  274,
   92,   59,   59,   59,  101,  266,  103,  277,  278,  263,
  264,  258,  282,  274,  267,  268,  269,  270,  106,  107,
  108,  109,   44,  115,  276,  276,   55,  276,  261,   58,
  276,  276,  276,   40,  276,  276,  133,  259,  262,  276,
   59,   41,   44,   72,  272,  276,   44,  277,   40,  276,
   41,    0,  259,   59,   41,   41,   50,  130,   90,   -1,
   90,   -1,  116,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  111,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  257,   -1,   -1,  260,
   -1,   -1,   -1,  261,  265,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  275,  276,  276,   -1,  279,  280,
  281,   -1,  283,  284,  285,  286,  287,  276,  277,  278,
   -1,   -1,   -1,  282,
};
#define YYFINAL 17
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 287
#define YYTRANSLATE(a) ((a) > YYMAXTOKEN ? (YYMAXTOKEN + 1) : (a))
#if YYDEBUG
static const char *yyname[] = {

"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,"'('","')'","'*'",0,"','",0,0,0,0,0,0,0,0,0,0,0,0,0,0,"';'",0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"INSERT",
"INTO","VALUES","SELECT","FROM","WHERE","LOP","COP","CREATE","TABLE","INTEGER",
"VARCHAR","DOUBLE","CHAR","PRIMARY","KEY","REFERENCES","DATABASE","DROP",
"OBJECT","NUMBER","VALUE","QUIT","LIST_TABLES","LIST_TABLE","ALPHANUM",
"CONNECT","HELP","LIST_DBASES","CLEAR","CONTR","illegal-symbol",
};
static const char *yyrule[] = {
"$accept : start",
"start : insert",
"start : select",
"start : create_table",
"start : create_database",
"start : drop_table",
"start : drop_database",
"start : table_attr",
"start : list_tables",
"start : connection",
"start : exit_program",
"start : semicolon",
"start : parentesis_open",
"start : parentesis_close",
"start : help_pls",
"start : list_databases",
"start : clear",
"start : contributors",
"start : qualquer_coisa",
"start :",
"connection : CONNECT OBJECT",
"qualquer_coisa : OBJECT",
"exit_program : QUIT",
"clear : CLEAR",
"parentesis_open : '('",
"parentesis_close : ')'",
"table_attr : LIST_TABLE OBJECT",
"list_tables : LIST_TABLES",
"list_databases : LIST_DBASES",
"help_pls : HELP",
"contributors : CONTR",
"$$1 :",
"insert : INSERT INTO $$1 table opt_column_list VALUES parentesis_open value_list parentesis_close semicolon",
"semicolon : ';'",
"table : OBJECT",
"opt_column_list :",
"opt_column_list : parentesis_open column_list parentesis_close",
"column_list : column",
"column_list : column ',' column_list",
"column : OBJECT",
"value_list : value",
"value_list : value ',' value_list",
"value : VALUE",
"value : NUMBER",
"value : ALPHANUM",
"$$2 :",
"create_table : CREATE TABLE $$2 table parentesis_open table_column_attr parentesis_close semicolon",
"table_column_attr : column_create type attribute",
"table_column_attr : column_create type attribute ',' table_column_attr",
"type : INTEGER",
"$$3 :",
"type : VARCHAR $$3 parentesis_open NUMBER parentesis_close",
"type : DOUBLE",
"type : CHAR",
"column_create : OBJECT",
"attribute :",
"attribute : PRIMARY KEY",
"attribute : REFERENCES table_fk '(' column_fk ')'",
"table_fk : OBJECT",
"column_fk : OBJECT",
"$$4 :",
"$$5 :",
"drop_table : DROP TABLE $$4 OBJECT $$5 semicolon",
"$$6 :",
"$$7 :",
"create_database : CREATE DATABASE $$6 OBJECT $$7 semicolon",
"$$8 :",
"$$9 :",
"drop_database : DROP DATABASE $$8 OBJECT $$9 semicolon",
"$$10 :",
"select : SELECT $$10 info FROM table_select opt_where semicolon",
"opt_where :",
"opt_where : WHERE exp",
"info : column_list",
"info : '*'",
"op : OBJECT",
"op : ALPHANUM",
"op : VALUE",
"op : NUMBER",
"cop : COP",
"lop : LOP",
"table_select : OBJECT",
"exp : parentesis_open exp parentesis_close",
"exp : op cop op",
"exp : op lop op",
"exp : op cop exp",
"exp : op lop exp",
"exp : exp2 cop op",
"exp : exp2 lop op",
"exp : exp2 cop exp",
"exp : exp2 lop exp",
"exp2 : op cop op",
"exp2 : op lop op",

};
#endif

int      yydebug;
int      yynerrs;

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  10000
#endif
#endif

#define YYINITSTACKSIZE 200

typedef struct {
    unsigned stacksize;
    short    *s_base;
    short    *s_mark;
    short    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
} YYSTACKDATA;
/* variables for the parser stack */
static YYSTACKDATA yystack;

#if YYDEBUG
#include <stdio.h>		/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (short *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return -1;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return -1;

    data->l_base = newvs;
    data->l_mark = newvs + i;

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab

int
YYPARSE_DECL()
{
    int yym, yyn, yystate;
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack)) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = yyname[YYTRANSLATE(yychar)];
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
        {
            goto yyoverflow;
        }
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;

    yyerror("syntax error");

    goto yyerrlab;

yyerrlab:
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yystack.s_mark]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
                {
                    goto yyoverflow;
                }
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
                --yystack.s_mark;
                --yystack.l_mark;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = yyname[YYTRANSLATE(yychar)];
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    if (yym)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
    switch (yyn)
    {
case 11:
#line 58 "interface/yacc.y"
	{GLOBAL_PARSER.consoleFlag = 1; return 0;}
break;
case 20:
#line 67 "interface/yacc.y"
	{connect(*yytext); GLOBAL_PARSER.consoleFlag = 1; return 0;}
break;
case 21:
#line 69 "interface/yacc.y"
	{GLOBAL_PARSER.consoleFlag = 1; GLOBAL_PARSER.noerror = 0; return 0;}
break;
case 22:
#line 72 "interface/yacc.y"
	{exit(0);}
break;
case 23:
#line 74 "interface/yacc.y"
	{clear(); GLOBAL_PARSER.consoleFlag = 1; return 0;}
break;
case 24:
#line 76 "interface/yacc.y"
	{GLOBAL_PARSER.parentesis++;}
break;
case 25:
#line 78 "interface/yacc.y"
	{GLOBAL_PARSER.parentesis--;}
break;
case 26:
#line 81 "interface/yacc.y"
	{
    if(connected.conn_active) {
        printTable(yylval.strval);
        GLOBAL_PARSER.consoleFlag = 1;
    } else
        notConnected();
    return 0;
}
break;
case 27:
#line 91 "interface/yacc.y"
	{
    if(connected.conn_active) {
        printTable(NULL);
        GLOBAL_PARSER.consoleFlag = 1;
    } else
        notConnected();
    return 0;
}
break;
case 28:
#line 101 "interface/yacc.y"
	{
    showDB();
    GLOBAL_PARSER.consoleFlag = 1;
    return 0;
}
break;
case 29:
#line 108 "interface/yacc.y"
	{help(); GLOBAL_PARSER.consoleFlag = 1; return 0;}
break;
case 30:
#line 111 "interface/yacc.y"
	{contr(); GLOBAL_PARSER.consoleFlag = 1; return 0;}
break;
case 31:
#line 118 "interface/yacc.y"
	{setMode(OP_INSERT);}
break;
case 32:
#line 118 "interface/yacc.y"
	{
    if (GLOBAL_PARSER.col_count == GLOBAL_PARSER.val_count || GLOBAL_DATA.columnName == NULL)
        GLOBAL_DATA.N = GLOBAL_PARSER.val_count;
    else {
        printf("ERROR: The column counter doesn't match the value counter.\n");
        GLOBAL_PARSER.noerror=0;
    }
    return 0;
}
break;
case 34:
#line 130 "interface/yacc.y"
	{setObjName(yytext);}
break;
case 39:
#line 136 "interface/yacc.y"
	{setColumnInsert(yytext);}
break;
case 42:
#line 140 "interface/yacc.y"
	{setValueInsert(yylval.strval, 'D');}
break;
case 43:
#line 141 "interface/yacc.y"
	{setValueInsert(yylval.strval, 'I');}
break;
case 44:
#line 142 "interface/yacc.y"
	{setValueInsert(yylval.strval, 'S');}
break;
case 45:
#line 145 "interface/yacc.y"
	{setMode(OP_CREATE_TABLE);}
break;
case 46:
#line 145 "interface/yacc.y"
	{
    GLOBAL_DATA.N = GLOBAL_PARSER.col_count;

    return 0;
}
break;
case 49:
#line 153 "interface/yacc.y"
	{setColumnTypeCreate('I');}
break;
case 50:
#line 154 "interface/yacc.y"
	{setColumnTypeCreate('S');}
break;
case 51:
#line 154 "interface/yacc.y"
	{setColumnSizeCreate(yylval.strval);}
break;
case 52:
#line 155 "interface/yacc.y"
	{setColumnTypeCreate('D');}
break;
case 53:
#line 156 "interface/yacc.y"
	{setColumnTypeCreate('C');}
break;
case 54:
#line 158 "interface/yacc.y"
	{setColumnCreate(yytext);}
break;
case 56:
#line 161 "interface/yacc.y"
	{setColumnPKCreate();}
break;
case 58:
#line 164 "interface/yacc.y"
	{setColumnFKTableCreate(yytext);}
break;
case 59:
#line 166 "interface/yacc.y"
	{setColumnFKColumnCreate(yytext);}
break;
case 60:
#line 169 "interface/yacc.y"
	{setMode(OP_DROP_TABLE);}
break;
case 61:
#line 169 "interface/yacc.y"
	{setObjName(yytext);}
break;
case 62:
#line 169 "interface/yacc.y"
	{return 0;}
break;
case 63:
#line 172 "interface/yacc.y"
	{setMode(OP_CREATE_DATABASE);}
break;
case 64:
#line 172 "interface/yacc.y"
	{setObjName(yytext);}
break;
case 65:
#line 172 "interface/yacc.y"
	{return 0;}
break;
case 66:
#line 175 "interface/yacc.y"
	{setMode(OP_DROP_DATABASE);}
break;
case 67:
#line 175 "interface/yacc.y"
	{setObjName(yytext);}
break;
case 68:
#line 175 "interface/yacc.y"
	{return 0;}
break;
case 69:
#line 178 "interface/yacc.y"
	{setMode(OP_SELECT_ALL); }
break;
case 70:
#line 178 "interface/yacc.y"
	{
  GLOBAL_DATA.N = GLOBAL_PARSER.col_count;
  GLOBAL_DATA.ncond = GLOBAL_PARSER.cond_count;
  return 0;
}
break;
case 75:
#line 187 "interface/yacc.y"
	{setWhereCondition(yytext);}
break;
case 76:
#line 187 "interface/yacc.y"
	{setWhereCondition(yytext);}
break;
case 77:
#line 188 "interface/yacc.y"
	{setWhereCondition(yytext);}
break;
case 78:
#line 188 "interface/yacc.y"
	{setWhereCondition(yytext);}
break;
case 79:
#line 190 "interface/yacc.y"
	{setWhereCondition(yytext);}
break;
case 80:
#line 192 "interface/yacc.y"
	{setWhereCondition(yytext);}
break;
case 81:
#line 194 "interface/yacc.y"
	{setObjName(yytext);}
break;
#line 854 "interface/y.tab.c"
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
        if (yychar < 0)
        {
            if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = yyname[YYTRANSLATE(yychar)];
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yystack.s_mark, yystate);
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
    {
        goto yyoverflow;
    }
    *++yystack.s_mark = (short) yystate;
    *++yystack.l_mark = yyval;
    goto yyloop;

yyoverflow:
    yyerror("yacc stack overflow");

yyabort:
    yyfreestack(&yystack);
    return (1);

yyaccept:
    yyfreestack(&yystack);
    return (0);
}

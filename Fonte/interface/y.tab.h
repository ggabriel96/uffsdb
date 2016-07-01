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
extern YYSTYPE yylval;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shuntingyard.h"

int precedence(char *op) {
  switch (op[0]) {
    case '<':
    case '=':
    case '!':
    case '>':
      return 2;
  }
  return !strcmp(op, "and") || !strcmp(op, "or");
}

typedef struct Stack {
  int top;
  char **tokens;
} stack_t;

int empty(stack_t *s) {
  return s -> top == 0;
}

char *top(stack_t *s) {
  if (!empty(s)) return s -> tokens[s -> top - 1];
  return NULL;
}

char *pop(stack_t *s) {
  char *top = NULL;
  if (!empty(s)) {
    s -> top--;
    top = malloc((1 + strlen(s -> tokens[s -> top])) * sizeof (char));
    strcpy(top, s -> tokens[s -> top]);
    free(s -> tokens[s -> top]);
    s -> tokens = realloc(s -> tokens, s -> top * sizeof (char *));
  }
  return top;
}

void push(stack_t *s, char *elem) {
  // sim, funciona só o realloc :)
  s -> tokens = realloc(s -> tokens, (s -> top + 1) * sizeof (char *));
  s -> tokens[s -> top] = malloc((1 + strlen(elem)) * sizeof (char));
  strcpy(s -> tokens[s -> top], elem);
  s -> top++;
}

char **shuntingYard(char **tokens, int n) {
  stack_t op;
  int i, tcount, pi, ptop;
  op.top = 0;
  op.tokens = NULL;
  char **result = malloc(n * sizeof (char *));
  for (i = tcount = 0; i < n; i++) {
    pi = precedence(tokens[i]);
    if (pi == 0) {
      result[tcount] = malloc((strlen(tokens[i]) + 1) * sizeof (char));
      strcpy(result[tcount], tokens[i]);
      tcount++;
    }
    else {
      while (!empty(&op)) {
        ptop = precedence(top(&op));
        if (ptop < pi) break;
        result[tcount] = malloc((strlen(top(&op)) + 1) * sizeof (char));
        strcpy(result[tcount], pop(&op));
        tcount++;
      }
      push(&op, tokens[i]);
    }
  }
  while (!empty(&op)) {
    result[tcount] = malloc((strlen(top(&op)) + 1) * sizeof (char));
    strcpy(result[tcount], pop(&op));
    tcount++;
  }
  return result;
}

int isOperator(char * tk) {
  return tk[0] == '<'|| tk[0] == '<' || tk[0] == '=' || tk[0] == '!' || tk[0] == '>' || !strcmp(tk, "and") || !strcmp(tk, "or");
}

int getCod(char * tk) {
  printf("TK = %s\n", tk);
  if (tk[0] == '<') return tk[1] == '=' ? LE : LT;
  if (tk[0] == '=') return EQ;
  if (tk[0] == '!') return NE;
  if (tk[0] == '>') return tk[1] == '=' ? GE : GT;
  if (!strcmp(tk, "and")) return AND;
  if (!strcmp(tk, "or")) return OR;
  if (tk[0] == '\'') return STRING;
  if (tk[0] >= 'a' && tk[0] <='z') return COLUMN;
  return NUM;
}

char * getValue(char *attname, column *tupla, struct fs_objects objeto) {
  int j;
  printf("´´´´´´´´´´´´´´´´´´´´\n");
  for(j = 0; j < objeto.qtdCampos; j++) {
    printf("%.10s = %.10s ? \n", attname, tupla[j].nomeCampo);
    if(!strcmp(attname, tupla[j].nomeCampo)) return tupla[j].valorCampo;
  }
  return NULL;
}

int testwhere(column *tupla, char **tokens, int ncond, int nrec, struct fs_objects objeto) {
  int i; //int j;
  stack_t op; op.top = 0; op.tokens = NULL;
  int operand1, operand2, operator;
  char *op1, *op2;
  if (ncond == 0) return 0;
  return 0; // -> Coloquei para não interferir com seus testes (DEVE SER APAGADO DEPOIS)
  //return 1; //Estou acabando a função, é só para vocês poderem continuar testando/codando normalmente
  // for (i = 0; i < 100; i++) {
  //   printf("%.10s", tokens[i]);
  //   printf("   %d\n", i);
  // }
  //printf("-------------------\n");
  for (i = 0; i < ncond; i++) {
    if (isOperator(tokens[i])) {
      op1 = pop(&op); op2 = pop(&op); //Hm... Segundo o que você fez na outra função, eu não poderia fazer isso
      operand1 = getCod(op1); operand2 = getCod(op2);
      //printf("%d %d\n", operand1, operand2);
      //Precisa dar free nesse op aqui embaixo? Não sei colocar os valores da tupla em op1/op2 =(
      if (operand1 == COLUMN) { op1 = getValue(op1, tupla, objeto); getCod(op1); }
      if (operand2 == COLUMN) { op1 = getValue(op2, tupla, objeto); getCod(op2); }
      if (operand1 != operand2) return ERROR;
      //printf(">%s(%d) %s %s(%d)\n", op1, operand1, tokens[i], op2, operand2);
      //Fazer operação e devolver para pilha
      push(&op, "TMP"); //Para não dar falha de segmentação :P
    } else push(&op, tokens[i]);
  }
  if (!strcmp(top(&op), "1")) return 0; //Sucesso Weee
  return 1; //Falha
}

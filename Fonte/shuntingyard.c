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
    case 'a':
    case 'o':
      return 1;
  }
  return 0;
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

int getCod(char * tk) {
  printf("TK = %s\n", tk);
  if (tk[0] == '<') return tk[1] == '=' ? LE : LT;
  if (tk[0] == '=') return EQ;
  if (tk[0] == '!') return NE;
  if (tk[0] == '>') return tk[1] == '=' ? GE : GT;
  if (!strcmp(tk, "and")) return AND;
  if (!strcmp(tk, "or")) return OR;
  if (tk[0] == '\'') return STRING;
  if (tk[0] >= 'a' && tk[0] <='z' && tk[0] >='A' && tk[0] <= 'Z' ) return COLUMN;
  return NUM;
}

int testwhere(column *tupla, char **tokens, int ncond, int nrec, struct fs_objects objeto) {
  int i, j;
  int operand1, operand2, operator;
  if (ncond == 0) return 0;
  //return 1; //Estou acabando a função, é só para vocês poderem continuar testando/codando normalmente
  operand1 = getCod(tokens[0]);
  operand2 = getCod(tokens[1]);
  printf(">>> WHERE: \n");
  for (i = 0; i < ncond; i++) {
    printf("-> %s\n", tokens[i]);
  }
  printf("Operand1 = %d    Operand2 = %d\n", operand1, operand2);
  for(j = 0; j < objeto.qtdCampos; j++) {
    if(tupla[j].tipoCampo == 'S')
      printf(" %-20s ", tupla[j].valorCampo);
    else if(tupla[j].tipoCampo == 'I') {
      int *n = (int *)&tupla[j].valorCampo[0];
      printf(" %-10d ", *n);
    } else if(tupla[j].tipoCampo == 'C') {
      printf(" %-10c ", tupla[j].valorCampo[0]);
    } else if(tupla[j].tipoCampo == 'D') {
      double *n = (double *)&tupla[j].valorCampo[0];
      printf(" %-10f ", *n);
    }
    if(j >= 0 && ((j + 1)%objeto.qtdCampos) == 0) printf("\n");
    else printf("|");
  }
  printf(">>>>>>>>>>>>>>>>>>>>>>\n");
  return 0;
}

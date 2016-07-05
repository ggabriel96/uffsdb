#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "shuntingyard.h"

// não colocar precedência 0, porque isso é usado
// pra ver se é um operador ou não
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

char *pop(stack_t *s, int backup) {
  char *top = NULL;
  if (!empty(s)) {
    s -> top--;
    if (backup) {
      top = malloc((1 + strlen(s -> tokens[s -> top])) * sizeof (char));
      strcpy(top, s -> tokens[s -> top]);
    }
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
        result[tcount++] = pop(&op, 1);
      }
      push(&op, tokens[i]);
    }
  }
  while (!empty(&op))
    result[tcount++] = pop(&op, 1);
  return result;
}

int isOperator(char * tk) {
  return precedence(tk) != 0;
  // return tk[0] == '<'|| tk[0] == '=' || tk[0] == '!' || tk[0] == '>' || !strcmp(tk, "and") || !strcmp(tk, "or");
}

int getCod(char * tk) {
  //printf("TK = %s\n", tk);
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

char * getValue(char *attname, column *tupla, int nrec, struct fs_objects objeto) {
  int j;
  //printf("´´´´´´´´´´´´´´´´´´´´\n");
  char * resp = NULL;
  for(j = 0; j < objeto.qtdCampos && tupla + j < tupla + objeto.qtdCampos * nrec; j++) {
    //printf("%.10s = %.10s ? \n>", attname, tupla[j].nomeCampo);
    if(!strcmp(attname, tupla[j].nomeCampo)) {
      if (tupla[j].tipoCampo == 'S') {
        resp = malloc((strlen(tupla[j].valorCampo) + 3) * sizeof(char));
        sprintf(resp, "'%s'", tupla[j].valorCampo);
      } else if (tupla[j].tipoCampo == 'I') {
        resp = malloc(12 * sizeof(char)); //Maior valor de um int cabe em 12 caracteres
        sprintf(resp, "%d", *((int *)&tupla[j].valorCampo[0]));
      } else if(tupla[j].tipoCampo == 'C') {
        resp = malloc(4 * sizeof(char));
        sprintf(resp, "'%c'", tupla[j].valorCampo[0]);
      } else if(tupla[j].tipoCampo == 'D') { //Esse valor pode ser grande... E agora?... 20 caracteres ta bom?
        resp = malloc(20 * sizeof(char));
        sprintf(resp, "%f", *((double *)&tupla[j].valorCampo[0]));
      }
    }
  }
  free(attname);
  return resp;
}

double getNum(char *operand) {
  double ret = 0; int i;
  unsigned long long pot10 = 1; //Vai cagar se o double tiver mais que 19 casas decimais
  //printf("Operand: %s\n", operand);
  //for (i = 0; i < 1000; i++);
  for (i = strlen(operand) - 1; i >= 0; i--) {
    if (operand[i] == '-') ret *= -1;
    else if (operand[i] == '+') continue;
    else if (operand[i] == '.') { ret /= pot10;  pot10 = 1; }
    else { ret += (operand[i] - '0') * pot10; pot10 *= 10; }
  }
  return ret;
}

int testwhere(column *tupla, char **tokens, int ncond, int nrec, struct fs_objects objeto) {
  int i; //int j;
  stack_t op; op.top = 0; op.tokens = NULL;
  int operand1, operand2, operator;
  char *op1, *op2;
  if (ncond == 0) return 0;
  //return 0; // -> Coloquei para não interferir com seus testes (DEVE SER APAGADO DEPOIS)
  //printf("testwhere: \n");
  // for (i = 0; i < ncond; i++)
  //   printf("- token[%d] = %s\n", i, tokens[i]);
  for (i = 0; i < ncond; i++) {
    if (isOperator(tokens[i])) {
      op1 = pop(&op, 1); op2 = pop(&op, 1);
      operand1 = getCod(op1); operand2 = getCod(op2); //operator = getCod(tokens[i]);
      //printf(">%s(%d) %s(%d)\n", op1, operand1, op2, operand2);

      //Precisa dar free nesse op aqui embaixo? Não sei colocar os valores da tupla em op1/op2 =(
      if (operand1 == COLUMN) {
        op1 = getValue(op1, tupla, nrec, objeto);
        if (op1 == NULL) return ERROR_COLUMN;
        operand1 = getCod(op1);
      }
      if (operand2 == COLUMN) {
        op2 = getValue(op2, tupla, nrec, objeto);
        if (op2 == NULL) return ERROR_COLUMN;
        operand2 = getCod(op2);
      }
      // printf(">>>%s %s<<<\n", op1, op2);
      //return 0;
      if (operand1 != operand2) { free(op1); free(op2); return ERROR_COMPARISON; }
      //printf(">%s(%d) %s %s(%d)\n", op1, operand1, tokens[i], op2, operand2);
      //----------------------WIP(não sei nem se compila o que está comentado)--------------------/
      // Fazer operação e devolver para pilha //Boatos que tem que trocar os operandos
      operator = getCod(tokens[i]);
      char res[2]; res[1] = '\0';
      if (operand1 == STRING) {
        switch (operator) {
          case EQ: res[0] = !strcmp(op2, op1) + '0'; break;
          case LT: res[0] = (strcmp(op2, op1) < 0) + '0'; break;
          case LE: res[0] = (strcmp(op2, op1) <= 0) + '0'; break;
          case GT: res[0] = (strcmp(op2, op1) > 0) + '0'; break;
          case GE: res[0] = (strcmp(op2, op1) >= 0) + '0';
        }
      } else if (operand1 == NUM) {
        //printf("Laaaaa\n");
        double v1 = getNum(op2), v2 = getNum(op1); //Seria legal fazer com double que dai já tratariamos todos os casos :P
        //printf("Operator: %d\nv1: %.2lf\n v2:%.2lf\n", operator, v1, v2);
        switch (operator) {
          case EQ: res[0] = igualDouble(v1, v2) + '0'; break;
          case LT: res[0] = (v1 < v2) + '0'; break;
          case LE: res[0] = menorIgualDouble(v1, v2) + '0'; break;
          case GT: res[0] = (v1 > v2) + '0'; break;
          case GE: res[0] = maiorIgualDouble(v1, v2) + '0'; break;
          case AND: res[0] = (op1[0] == '1' && op2[0] == '1') + '0'; break;
          case OR: res[0] = (op1[0] == '1' || op2[0] == '1') + '0'; break;
        }
      }
      //printf("resp: %s\n", res);
      push(&op, res);
      //push(&op, "TMP"); //Para não dar falha de segmentação :P
      free(op1); free(op2);
    } else push(&op, tokens[i]);
  }
  if (!strcmp(top(&op), "1")) return 0; //Sucesso Weee
  return 1; //Falha
}

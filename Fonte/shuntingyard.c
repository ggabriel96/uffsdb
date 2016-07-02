#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shuntingyard.h"

int precedence(int op) {
  switch (op) {
    case LE:
    case LT:
    case EQ:
    case NE:
    case GE:
    case GT:
      return 2;
    case AND:
    case OR:
      return 1;
  }
  return 0;
}

char **shuntingYard(char **tokens, int n) {
  int i;
  char **result = malloc(n * sizeof (char *));
  for (i = 0; i < n; i++) {
    result[i] = malloc(sizeof (char) * (strlen(tokens[i]) + 1));
    strcpy(result[i], tokens[i]);
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

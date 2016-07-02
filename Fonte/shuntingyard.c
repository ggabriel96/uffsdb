#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "macros.h"
#include "interface/parser.h"
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

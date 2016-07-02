#include <stdio.h>
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

void shuntingYard(char **queue, int *stack) {
  int i;
  // for (i = 0; i < )
}

int testwhere(collumn )

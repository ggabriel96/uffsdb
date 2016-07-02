#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "macros.h"
#include "interface/parser.h"
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

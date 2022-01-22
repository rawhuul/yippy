#include "eval.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

lval *lval_num(long x) {
  lval *v = (lval *)malloc(sizeof(lval));
  v->type = LVAL_NUM;
  v->num = x;
  return v;
}

lval *lval_err(char *x) {
  lval *v = (lval *)malloc(sizeof(lval));
  v->type = LVAL_ERR;
  v->error = (char *)malloc(sizeof(x) + 1);
  strcpy(v->error, x);
  return v;
}

lval *lval_sym(char *s) {
  lval *v = malloc(sizeof(lval));
  v->type = LVAL_SYM;
  v->symbol = malloc(strlen(s) + 1);
  strcpy(v->symbol, s);
  return v;
}

lval *lval_sexpr(void) {
  lval *v = malloc(sizeof(lval));
  v->type = LVAL_SEXP;
  v->count = 0;
  v->cell = NULL;
  return v;
}

void lval_del(lval *v) {
  switch (v->type) {
  case LVAL_NUM:
    break;

  case LVAL_SYM: {
    free(v->symbol);
    break;
  }
  case LVAL_ERR: {
    free(v->error);
    break;
  }
  case LVAL_SEXP: {
    for (int i = 0; i < v->count; ++i) {
      lval_del(v->cell[i]);
    }
  }

  default:
    break;
  }

  free(v);
}

lval *lval_add(lval *v, lval *x) {
  v->count++;
  v->cell = realloc(v->cell, sizeof(lval *) * v->count);
  v->cell[v->count - 1] = x;
  return v;
}

lval *lval_read_num(mpc_ast_t *t) {
  errno = 0;
  long x = strtol(t->contents, NULL, 10);
  return errno != ERANGE ? lval_num(x) : lval_err("invalid number");
}

lval *lval_read(mpc_ast_t *t) {

  if (strstr(t->tag, "number")) {
    return lval_read_num(t);
  }
  if (strstr(t->tag, "symbol")) {
    return lval_sym(t->contents);
  }

  lval *x = NULL;
  if (!strcmp(t->tag, ">")) {
    x = lval_sexpr();
  }
  if (strstr(t->tag, "sexpr")) {
    x = lval_sexpr();
  }

  for (int i = 0; i < t->children_num; i++) {
    if (!strcmp(t->children[i]->contents, "(")) {
      continue;
    }
    if (!strcmp(t->children[i]->contents, ")")) {
      continue;
    }
    if (!strcmp(t->children[i]->tag, "regex")) {
      continue;
    }
    x = lval_add(x, lval_read(t->children[i]));
  }

  return x;
}

void lval_expr_print(lval *v, char open, char close) {
  putchar(open);
  for (int i = 0; i < v->count; i++) {

    /* Print Value contained within */
    lval_print(v->cell[i]);

    /* Don't print trailing space if last element */
    if (i != (v->count - 1)) {
      putchar(' ');
    }
  }
  putchar(close);
}

void lval_print(lval *v) {
  switch (v->type) {
  case LVAL_NUM:
    printf("%ld\n", v->num);
    break;
  case LVAL_ERR:
    printf("Error: %s\n", v->error);
    break;
  case LVAL_SYM:
    printf("%s\n", v->symbol);
    break;
  case LVAL_SEXP:
    lval_expr_print(v, '(', ')');
    break;
  }
}

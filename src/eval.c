#include "eval.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LASSERT(args, cond, err)                                               \
  if (!(cond)) {                                                               \
    lval_del(args);                                                            \
    return lval_err(err);                                                      \
  }

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

lval *lval_qexpr(void) {
  lval *v = malloc(sizeof(lval));
  v->type = LVAL_QEXP;
  v->count = 0;
  v->cell = NULL;
  return v;
}

void lval_del(lval *v) {
  switch (v->type) {
  case LVAL_NUM:
    break;

  case LVAL_SYM: {
    if (v->symbol)
      free(v->symbol);
    break;
  }
  case LVAL_ERR: {
    if (v->error)
      free(v->error);
    break;
  }
  case LVAL_SEXP:
  case LVAL_QEXP: {
    for (int i = 0; i < v->count; ++i) {
      lval_del(v->cell[i]);
    }
    if (v->cell) {
      free(v->cell);
    }
    break;
  }

  default:
    break;
  }
  if (v) {
    free(v);
  }
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
  if (strstr(t->tag, "qexpr")) {
    x = lval_qexpr();
  }

  for (int i = 0; i < t->children_num; i++) {
    if (!strcmp(t->children[i]->contents, "(")) {
      continue;
    }
    if (!strcmp(t->children[i]->contents, ")")) {
      continue;
    }
    if (!strcmp(t->children[i]->contents, "{")) {
      continue;
    }
    if (!strcmp(t->children[i]->contents, "}")) {
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

    lval_print(v->cell[i]);

    if (i != (v->count - 1)) {
      putchar(' ');
    }
  }
  putchar(close);
}

void lval_print(lval *v) {
  switch (v->type) {
  case LVAL_NUM:
    printf("%ld", v->num);
    break;
  case LVAL_ERR:
    printf("Error: %s", v->error);
    break;
  case LVAL_SYM:
    printf("%s", v->symbol);
    break;
  case LVAL_QEXP:
    lval_expr_print(v, '{', '}');
    break;
  case LVAL_SEXP:
    lval_expr_print(v, '(', ')');
    break;
  }
}

void lval_println(lval *v) {
  lval_print(v);
  putchar('\n');
}

lval *lval_pop(lval *v, int i) {

  lval *x = v->cell[i];

  memmove(&v->cell[i], &v->cell[i + 1], sizeof(lval *) * (v->count - i - 1));

  v->count--;

  v->cell = realloc(v->cell, sizeof(lval *) * v->count);
  return x;
}

lval *lval_take(lval *v, int i) {
  lval *x = lval_pop(v, i);
  lval_del(v);
  return x;
}

lval *builtin_op(lval *a, char *op) {

  for (int i = 0; i < a->count; i++) {
    if (a->cell[i]->type != LVAL_NUM) {
      lval_del(a);
      return lval_err("Cannot operate on non-number!");
    }
  }

  lval *x = lval_pop(a, 0);

  if ((strcmp(op, "-") == 0) && a->count == 0) {
    x->num = -x->num;
  }

  while (a->count > 0) {

    lval *y = lval_pop(a, 0);

    if (strcmp(op, "+") == 0) {
      x->num += y->num;
    }
    if (strcmp(op, "-") == 0) {
      x->num -= y->num;
    }
    if (strcmp(op, "*") == 0) {
      x->num *= y->num;
    }
    if (strcmp(op, "%") == 0) {
      x->num %= y->num;
    }
    if (strcmp(op, "|") == 0) {
      x->num |= y->num;
    }
    if (strcmp(op, "&") == 0) {
      x->num &= y->num;
    }
    if (strcmp(op, "/") == 0) {
      if (y->num == 0) {
        lval_del(x);
        lval_del(y);
        x = lval_err("Division By Zero!");
        break;
      }
      x->num /= y->num;
    }

    lval_del(y);
  }

  lval_del(a);
  return x;
}

lval *lval_eval_sexpr(lval *v) {

  for (int i = 0; i < v->count; i++) {
    v->cell[i] = lval_eval(v->cell[i]);
  }

  for (int i = 0; i < v->count; i++) {
    if (v->cell[i]->type == LVAL_ERR) {
      return lval_take(v, i);
    }
  }

  if (v->count == 0) {
    return v;
  }

  if (v->count == 1) {
    return lval_take(v, 0);
  }

  lval *f = lval_pop(v, 0);
  if (f->type != LVAL_SYM) {
    lval_del(f);
    lval_del(v);
    return lval_err("S-expression Does not start with symbol!");
  }

  lval *result = builtin(v, f->symbol);
  lval_del(f);
  return result;
}

lval *lval_eval(lval *v) {
  if (v->type == LVAL_SEXP) {
    return lval_eval_sexpr(v);
  }

  return v;
}

lval *builtin_head(lval *a) {

  LASSERT(a, a->count, "Function 'head' passed too many arguments!");

  LASSERT(a, a->cell[0]->type == LVAL_QEXP,
          "Function 'head' passed incorrect type!");

  LASSERT(a, a->cell[0]->count != 0, "Function 'head' passed {}!");

  lval *v = lval_take(a, 0);

  while (v->count > 1) {
    lval_del(lval_pop(v, 1));
  }
  return v;
}

lval *builtin_tail(lval *a) {
  LASSERT(a, a->count == 1, "Function 'tail' passed too many arguments!");
  LASSERT(a, a->cell[0]->type == LVAL_QEXP,
          "Function 'tail' passed incorrect type!");
  LASSERT(a, a->cell[0]->count != 0, "Function 'tail' passed {}!");

  lval *v = lval_take(a, 0);
  lval_del(lval_pop(v, 0));
  return v;
}

lval *builtin_list(lval *a) {
  a->type = LVAL_QEXP;
  return a;
}

lval *builtin_eval(lval *a) {
  LASSERT(a, a->count == 1, "Function 'eval' passed too many arguments!");
  LASSERT(a, a->cell[0]->type == LVAL_QEXP,
          "Function 'eval' passed wrong type of arguements!");

  lval *x = lval_take(a, 0);
  x->type = LVAL_SEXP;
  return lval_eval(x);
}

lval *lval_join(lval *x, lval *y) {
  while (y->count) {
    x = lval_add(x, lval_pop(y, 0));
  }

  lval_del(y);
  return x;
}

lval *builtin_join(lval *a) {
  for (int i = 0; i < a->count; ++i) {
    LASSERT(a, a->cell[i]->type == LVAL_QEXP,
            "Function 'join' passed wrong type of arguements.");
  }

  lval *x = lval_pop(a, 0);

  while (a->count) {
    x = lval_join(x, lval_pop(a, 0));
  }

  lval_del(a);
  return x;
}

lval *builtin(lval *a, char *func) {
  if (!strcmp("list", func)) {
    return builtin_list(a);
  }
  if (!strcmp("eval", func)) {
    return builtin_eval(a);
  }
  if (!strcmp("head", func)) {
    return builtin_head(a);
  }
  if (!strcmp("tail", func)) {
    return builtin_tail(a);
  }
  if (!strcmp("join", func)) {
    return builtin_join(a);
  }
  if (strstr("+-/*%&|", func)) {
    return builtin_op(a, func);
  }
  lval_del(a);
  return lval_err("Unknown Function!");
}

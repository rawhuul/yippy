#include "types.h"
#include <stdlib.h>
#include <string.h>

char *type_name(ltype t) {
  switch (t) {
  case LVAL_FUNC:
    return "Function";
  case LVAL_NUM:
    return "Number";
  case LVAL_QEXP:
    return "Q-expression";
  case LVAL_SEXP:
    return "S-expression";
  case LVAL_STR:
    return "String";
  case LVAL_SYM:
    return "Symbol";
  case LVAL_ERR:
    return "Error";
  default:
    return "Unknown";
  }
}

lval *lval_num(long x) {
  lval *v = (lval *)malloc(sizeof(lval));
  v->type = LVAL_NUM;
  v->num = x;
  return v;
}

lval *lval_str(char *str) {
  lval *v = malloc(sizeof(lval));
  v->type = LVAL_STR;
  v->string = malloc(strlen(str) + 1);
  strcpy(v->string, str);
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

lval *lval_func(lbuiltin func) {
  lval *v = malloc(sizeof(lval));
  v->type = LVAL_FUNC;
  v->func = func;
  return v;
}

lval *lval_lambda(lval *formals, lval *body) {
  lval *v = malloc(sizeof(lval));
  v->type = LVAL_FUNC;
  v->func = NULL;
  v->env = lenv_new();
  v->formals = formals;
  v->body = body;
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
  if (v != NULL) {
    switch (v->type) {
    case LVAL_NUM:
      break;
    case LVAL_FUNC:
      /* if (!v->func) { */
      /*   lenv_del(v->env); */
      /*   lval_del(v->formals); */
      /*   lval_del(v->body); */
      /* } */
      break;

    case LVAL_SYM: {
      if (v->symbol)
        free(v->symbol);
      break;
    }
    case LVAL_STR: {
      if (v->string)
        free(v->string);
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
      free(v->cell);
      break;
    }
    }
  }
}

lval *lval_add(lval *v, lval *x) {
  v->count++;
  v->cell = realloc(v->cell, sizeof(lval *) * v->count);
  v->cell[v->count - 1] = x;
  return v;
}

lval *lval_copy(lval *v) {
  lval *x = malloc(sizeof(lval));
  x->type = v->type;

  switch (v->type) {
  case LVAL_NUM: {

    x->num = v->num;
    break;
  }
  case LVAL_FUNC: {
    if (v->func) {
      x->func = v->func;
    } else {
      x->func = NULL;
      x->env = lenv_copy(v->env);
      x->formals = lval_copy(v->formals);
      x->body = lval_copy(v->body);
    }
    break;
  }
  case LVAL_ERR: {
    x->error = malloc(strlen(v->error + 1));
    strcpy(x->error, v->error);
    break;
  }
  case LVAL_SYM: {
    x->symbol = malloc(strlen(v->symbol + 1));
    strcpy(x->symbol, v->symbol);
    break;
  }
  case LVAL_STR: {
    x->string = malloc(strlen(v->string) + 1);
    strcpy(x->string, v->string);
    break;
  }
  case LVAL_SEXP:
  case LVAL_QEXP: {
    x->count = v->count;
    x->cell = malloc(sizeof(lval *) * x->count);
    for (int i = 0; i < x->count; ++i) {
      x->cell[i] = lval_copy(v->cell[i]);
    }
    break;
  }

  default:
    x->type = LVAL_ERR;
    x->error = "Unknown type find!";
    break;
  }

  return x;
}

lenv *lenv_new(void) {
  lenv *e = malloc(sizeof(lenv));
  e->count = 0;
  e->parent = NULL;
  e->syms = NULL;
  e->vals = NULL;
  return e;
}

void lenv_del(lenv *e) {
  if (e) {
    for (int i = 0; i < e->count; i++) {
      lval_del(e->vals[i]);
      free(e->syms[i]);
    }
    free(e->syms);
    free(e->vals);
    free(e);
  }
}

lenv *lenv_copy(lenv *e) {
  lenv *new = malloc(sizeof(lenv));

  new->parent = e->parent;
  new->count = e->count;
  new->syms = malloc(sizeof(char *) * new->count);
  new->vals = malloc(sizeof(lval *) * new->count);

  for (int i = 0; i < new->count; ++i) {
    new->syms[i] = malloc(strlen(e->syms[i]) + 1);
    strcpy(new->syms[i], e->syms[i]);
    new->vals[i] = lval_copy(e->vals[i]);
  }

  return new;
}

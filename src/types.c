#include "types.h"
#include <gc.h>
#include <gc/gc.h>
#include <math.h>
#include <string.h>

char *type_name(kind t) {
  switch (t) {
  case FUNCTION:
    return "Function";
  case NUMBER:
    return "Number";
  case QEXPRESSION:
    return "Q-expression";
  case SEXPRESSION:
    return "S-expression";
  case STRING:
    return "String";
  case SYMBOL:
    return "Symbol";
  case ERR:
    return "Error";
  case OK:
    return "";
  default:
    return "Unknown";
  }
}

int ifDouble(double a) {
  double x, y;

  y = modf(a, &x);

  if (y > 0) {
    return 1;
  } else {
    return 0;
  }
}

value *new_num(double x) {
  value *new = (value *)GC_malloc(sizeof(value));
  new->type = NUMBER;
  new->num = x;
  return new;
}

value *new_string(char *str) {
  value *new = GC_malloc(sizeof(value));
  new->type = STRING;
  new->string = GC_malloc(strlen(str) + 1);
  strcpy(new->string, str);
  return new;
}

value *new_symbol(char *s) {
  value *new = GC_malloc(sizeof(value));
  new->type = SYMBOL;
  new->symbol = GC_malloc(strlen(s) + 1);
  strcpy(new->symbol, s);
  return new;
}

value *new_sexp(void) {
  value *new = GC_malloc(sizeof(value));
  new->type = SEXPRESSION;
  new->count = 0;
  new->cell = NULL;
  return new;
}

value *new_func(function func) {
  value *new = GC_malloc(sizeof(value));
  new->type = FUNCTION;
  new->func = func;
  return new;
}

value *new_lambda(value *formals, value *body) {
  value *new = GC_malloc(sizeof(value));
  new->type = FUNCTION;
  new->func = NULL;
  new->env = new_scope();
  new->formals = formals;
  new->body = body;
  return new;
}

value *new_qexp(void) {
  value *new = GC_malloc(sizeof(value));
  new->type = QEXPRESSION;
  new->count = 0;
  new->cell = NULL;
  return new;
}

value *ok(void) {
  value *new = GC_malloc(sizeof(value));
  new->type = OK;
  new->count = 0;
  new->cell = NULL;
  return new;
}

value *add_value(value *dest, value *src) {
  dest->count++;
  dest->cell = GC_realloc(dest->cell, sizeof(value *) * dest->count);
  dest->cell[dest->count - 1] = src;
  return dest;
}

value *copy_value(value *src) {
  value *dest = GC_malloc(sizeof(value));
  dest->type = src->type;

  switch (src->type) {
  case NUMBER: {
    dest->num = src->num;
    break;
  }
  case FUNCTION: {
    if (src->func) {
      dest->func = src->func;
    } else {
      dest->func = NULL;
      dest->env = copy_scope(src->env);
      dest->formals = copy_value(src->formals);
      dest->body = copy_value(src->body);
    }
    break;
  }
  case ERR: {
    dest->error = GC_malloc(strlen(src->error + 1));
    strcpy(dest->error, src->error);
    break;
  }
  case SYMBOL: {
    dest->symbol = GC_malloc(strlen(src->symbol + 1));
    strcpy(dest->symbol, src->symbol);
    break;
  }
  case STRING: {
    dest->string = GC_malloc(strlen(src->string) + 1);
    strcpy(dest->string, src->string);
    break;
  }
  case SEXPRESSION:
  case QEXPRESSION: {
    dest->count = src->count;
    dest->cell = GC_malloc(sizeof(value *) * dest->count);
    for (unsigned int i = 0; i < dest->count; i++) {
      dest->cell[i] = copy_value(src->cell[i]);
    }
    break;
  }

  default:
    dest->type = ERR;
    dest->error = "Unknown type find!";
    break;
  }

  return dest;
}

void del_value(value *src) {
  if (src != NULL) {
    switch (src->type) {
    case NUMBER:
      break;
    case FUNCTION:
      /* if (!v->func) { */
      /*   lenv_del(v->env); */
      /*   value_del(v->formals); */
      /*   value_del(v->body); */
      /* } */
      break;

    case SYMBOL: {
      break;
    }
    case STRING: {
      break;
    }
    case OK:
      break;
    case ERR: {
      break;
    }
    case SEXPRESSION:
    case QEXPRESSION: {
      for (unsigned int i = 0; i < src->count; ++i) {
        del_value(src->cell[i]);
      }
      break;
    }
    }
  }
}

scope *new_scope(void) {
  scope *new = GC_malloc(sizeof(scope));
  new->count = 0;
  new->parent = NULL;
  new->syms = NULL;
  new->vals = NULL;
  return new;
}

scope *copy_scope(scope *src) {
  scope *dest = GC_malloc(sizeof(scope));

  dest->parent = src->parent;
  dest->count = src->count;
  dest->syms = GC_malloc(sizeof(char *) * dest->count);
  dest->vals = GC_malloc(sizeof(value *) * dest->count);

  for (int i = 0; i < dest->count; i++) {
    dest->syms[i] = GC_malloc(strlen(src->syms[i]) + 1);
    strcpy(dest->syms[i], src->syms[i]);
    dest->vals[i] = copy_value(src->vals[i]);
  }

  return dest;
}

void del_scope(scope *src) {
  if (src) {
    for (int i = 0; i < src->count; i++) {
      del_value(src->vals[i]);
    }
  }
}

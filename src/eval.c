#include "eval.h"
#include "error.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* JUST DECLERATIONS */

void lval_expr_print(lval *v, char open, char close);
void lval_print(lval *v);
void lval_println(lval *v);

lval *lval_num(long x);
lval *lval_sym(char *s);
lval *lval_sexpr(void);
lval *lval_func(lbuiltin func);
lval *lval_lambda(lval *formals, lval *body);
lval *lval_qexpr(void);
lval *lval_add(lval *v, lval *x);
lval *lval_read_num(mpc_ast_t *t);
lval *lval_read(mpc_ast_t *t);
lval *lval_pop(lval *v, int i);
lval *lval_take(lval *v, int i);
lval *builtin_op(lenv *env, lval *a, char *op);
lval *lval_eval_sexpr(lenv *e, lval *v);
lval *builtin_head(lenv *e, lval *a);
lval *builtin_tail(lenv *e, lval *a);
lval *builtin_list(lenv *e, lval *a);
lval *builtin_eval(lenv *e, lval *a);
lval *lval_join(lval *x, lval *y);
lval *builtin_join(lenv *e, lval *a);
lval *builtin(lenv *e, lval *a, char *func);
lval *lval_copy(lval *v);
lval *lenv_get(lenv *env, lval *k);
lval *lval_eval(lenv *e, lval *v);
lval *builtin_add(lenv *env, lval *a);
lval *builtin_minus(lenv *env, lval *a);
lval *builtin_div(lenv *env, lval *a);
lval *builtin_product(lenv *env, lval *a);
lval *builtin_let(lenv *env, lval *val, char *scope);
lval *builtin_lambda(lenv *env, lval *a);
lval *func_call(lenv *env, lval *f, lval *v);

lenv *lenv_new(void);
void lenv_del(lenv *e);
lenv *lenv_copy(lenv *e);
void lenv_def_global(lenv *env, lval *k, lval *v);

lval *lval_num(long x) {
  lval *v = (lval *)malloc(sizeof(lval));
  v->type = LVAL_NUM;
  v->num = x;
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
  switch (v->type) {
  case LVAL_NUM:
  case LVAL_FUNC: {
    if (!v->func) {
      // FIXMEEEEEE
      /* lval_del(v->formals); */
      /* lval_del(v->body); */
      lenv_del(v->env);
    }
  } break;

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
    free(v->cell);
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
  return errno != ERANGE ? lval_num(x)
                         : lval_err("%s is invalid "
                                    "number",
                                    x);
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
    if ((!strcmp(t->children[i]->contents, "(")) ||
        (!strcmp(t->children[i]->contents, ")")) ||
        (!strcmp(t->children[i]->contents, "{")) ||
        (!strcmp(t->children[i]->contents, "}")) ||
        (!strcmp(t->children[i]->tag, "regex"))) {
      continue;
    } else {
      x = lval_add(x, lval_read(t->children[i]));
    }
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
  case LVAL_FUNC: {
    if (v->func) {
      printf("<function>");
    } else {
      printf("(lambda");
      lval_print(v->formals);
      putchar(' ');
      lval_print(v->body);
      putchar(')');
    }
  } break;
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

lval *builtin_op(lenv *env, lval *a, char *op) {

  for (int i = 0; i < a->count; i++) {
    if (a->cell[i]->type != LVAL_NUM) {
      lval *err = lval_err("Cannot operate on "
                           "non-number, as argument %d "
                           "is %s!",
                           i + 1, type_name(a->cell[i]->type));
      lval_del(a);
      return err;
    }
  }

  lval *x = lval_pop(a, 0);

  if ((strcmp(op, "+") == 0) && a->count == 0) {
    x->num = (x->num);
  }
  if ((strcmp(op, "-") == 0) && a->count == 0) {
    x->num = -(x->num);
  }
  if ((strcmp(op, "~") == 0) && a->count == 0) {
    x->num = ~(x->num);
  }
  if ((strcmp(op, "!") == 0) && a->count == 0) {
    x->num = !(x->num);
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
        x = lval_err("You're trying to divide "
                     "%d By Zero, which is not "
                     "allowed!",
                     x->num);
        break;
      }
      x->num /= y->num;
    }

    lval_del(y);
  }

  lval_del(a);
  return x;
}

lval *lval_eval_sexpr(lenv *e, lval *v) {

  for (int i = 0; i < v->count; i++) {
    v->cell[i] = lval_eval(e, v->cell[i]);
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
  if (f->type != LVAL_FUNC) {
    lval *err = lval_err("%s is not a function!", type_name(f->type));
    lval_del(f);
    lval_del(v);
    return err;
  }

  lval *result = func_call(e, f, v);
  lval_del(f);
  return result;
}

lval *builtin_head(lenv *e, lval *a) {

  LASSERT(a, a->count == 1,
          "Function 'head' passed too many "
          "arguments. Got %i, expected %i.",
          a->count, 1);

  LASSERT(a, a->cell[0]->type == LVAL_QEXP,
          "Function 'head' passed "
          "incorrect type for "
          "arguement 0. Got %s, "
          "expected %s.",
          type_name(a->cell[0]->type), type_name(LVAL_QEXP));

  LASSERT(a, a->cell[0]->count != 0,
          "Function 'head' passed %d "
          "arguments!",
          a->cell[0]->count);

  lval *v = lval_take(a, 0);

  while (v->count > 1) {
    lval_del(lval_pop(v, 1));
  }
  return v;
}

lval *builtin_tail(lenv *e, lval *a) {
  LASSERT(a, a->count == 1,
          "Function 'tail' passed too many "
          "arguments. Got %i, expected %i.",
          a->count, 1);

  LASSERT(a, a->cell[0]->type == LVAL_QEXP,
          "Function 'tail' passed "
          "incorrect type for "
          "arguement 0. Got %s, "
          "expected %s.",
          type_name(a->cell[0]->type), type_name(LVAL_QEXP));

  LASSERT(a, a->cell[0]->count != 0,
          "Function 'tail' passed %d "
          "arguments!",
          a->cell[0]->count);

  lval *v = lval_take(a, 0);
  lval_del(lval_pop(v, 0));
  return v;
}

lval *builtin_list(lenv *e, lval *a) {
  a->type = LVAL_QEXP;
  return a;
}

lval *builtin_eval(lenv *e, lval *a) {
  LASSERT(a, a->count == 1,
          "Function 'eval' passed %d "
          "arguments, expected %d!",
          a->count, 1);
  LASSERT(a, a->cell[0]->type == LVAL_QEXP,
          "Function 'eval' passed "
          "wrong type of arguements. "
          "Expected %s, got %s!",
          type_name(LVAL_QEXP), type_name(a->cell[0]->type));

  lval *x = lval_take(a, 0);
  x->type = LVAL_SEXP;
  return lval_eval(e, x);
}

lval *lval_join(lval *x, lval *y) {
  while (y->count) {
    x = lval_add(x, lval_pop(y, 0));
  }

  lval_del(y);
  return x;
}

lval *builtin_join(lenv *e, lval *a) {
  for (int i = 0; i < a->count; ++i) {
    LASSERT(a, a->cell[i]->type == LVAL_QEXP,
            "Function 'join' passed "
            "wrong type of arguements. "
            "Got %s, expected %s.",
            type_name(a->cell[i]->type), type_name(LVAL_QEXP));
  }

  lval *x = lval_pop(a, 0);

  while (a->count) {
    x = lval_join(x, lval_pop(a, 0));
  }

  lval_del(a);
  return x;
}

lval *builtin(lenv *e, lval *a, char *func) {
  if (!strcmp("list", func)) {
    return builtin_list(e, a);
  } else if (!strcmp("eval", func)) {
    return builtin_eval(e, a);
  } else if (!strcmp("head", func)) {
    return builtin_head(e, a);
  } else if (!strcmp("tail", func)) {
    return builtin_tail(e, a);
  } else if (!strcmp("join", func)) {
    return builtin_join(e, a);
  } else if (strstr("+~!-/*%&|", func)) {
    return builtin_op(e, a, func);
  } else {
    lval_del(a);
    return lval_err("Got '%s', unknown Function!", func);
  }
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
  for (int i = 0; i < e->count; i++) {
    // FIXMEEEEEEEEEEEEEEEEEEEEEEEEEEE
    /* lval_del(e->vals[i]); */
    /* free(e->syms[i]); */
  }
  free(e->syms);
  free(e->vals);
  free(e);
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

void lenv_put(lenv *env, lval *key, lval *val) {
  for (int i = 0; i < env->count; ++i) {
    if (strcmp(env->syms[i], key->symbol) == 0) {
      lval_del(env->vals[i]);
      env->vals[i] = lval_copy(val);
      return;
    }
  }

  env->count++;
  env->vals = realloc(env->vals, (sizeof(lval *) * env->count));
  env->syms = realloc(env->syms, (sizeof(char *) * env->count));

  env->vals[env->count - 1] = lval_copy(val);
  env->syms[env->count - 1] = malloc(strlen(key->symbol) + 1);
  strcpy(env->syms[env->count - 1], key->symbol);
  return;
}

lval *lenv_get(lenv *env, lval *k) {
  for (int i = 0; i < env->count; ++i) {
    if (!strcmp(env->syms[i], k->symbol))
      return lval_copy(env->vals[i]);
  }

  if (env->parent) {
    return lenv_get(env->parent, k);
  } else {
    return lval_err("Symbol '%s' not found!", k->symbol);
  }
}

void lenv_def_global(lenv *env, lval *k, lval *v) {
  while (env->parent) {
    env = env->parent;
  }
  lenv_put(env, k, v);
}

lval *lval_eval(lenv *e, lval *v) {
  if (v->type == LVAL_SEXP) {
    return lval_eval_sexpr(e, v);
  }
  if (v->type == LVAL_SYM) {
    lval *x = lenv_get(e, v);
    lval_del(v);
    return x;
  }

  return v;
}

lval *builtin_add(lenv *env, lval *a) { return builtin_op(env, a, "+"); }

lval *builtin_minus(lenv *env, lval *a) { return builtin_op(env, a, "-"); }

lval *builtin_div(lenv *env, lval *a) { return builtin_op(env, a, "/"); }

lval *builtin_product(lenv *env, lval *a) { return builtin_op(env, a, "*"); }

lval *builtin_global(lenv *env, lval *val) {
  return builtin_let(env, val, "let");
}

lval *builtin_local(lenv *env, lval *val) { return builtin_let(env, val, "="); }

lval *builtin_let(lenv *env, lval *val, char *scope) {
  LASSERT(val, val->cell[0]->type == LVAL_QEXP,
          "Passed incorrect type, got "
          "%s whereas %s was expected!",
          type_name(val->cell[0]->type), type_name(LVAL_QEXP));

  lval *symbol = val->cell[0];

  for (int i = 0; i < symbol->count; i++) {
    LASSERT(val, symbol->cell[i]->type == LVAL_SYM,
            "Passed incorrect type: "
            "You can't define values "
            "other than symbol "
            "whereas %s passed"
            "to a variable!",
            symbol->cell[i]->type);
  }

  LASSERT(val, symbol->count == val->count - 1,
          "Incorrect number of values "
          "passed. Got %d, expected %d!",
          val->count - 1, symbol->count);

  for (int i = 0; i < symbol->count; ++i) {
    if (!strcmp(scope, "let")) {
      lenv_def_global(env, symbol->cell[i], val->cell[i + 1]);
    }
    if (!strcmp(scope, "=")) {
      lenv_put(env, symbol->cell[i], val->cell[i + 1]);
    }
  }

  lval_del(val);
  return lval_sexpr();
}

lval *func_call(lenv *env, lval *f, lval *v) {
  if (f->func) {
    return f->func(env, v);
  }

  int passed = v->count;
  int required = f->formals->count;

  while (v->count) {
    if (!f->formals->count) {
      lval_del(v);
      return lval_err(
          "Function passed too many arguements. Passed %d, expected %d.",
          passed, required);
    }

    lval *symbol = lval_pop(f->formals, 0);
    lval *val = lval_pop(v, 0);
    lenv_put(f->env, symbol, val);
    lval_del(symbol);
    lval_del(val);
  }

  lval_del(v);

  if (!f->formals->count) {
    f->env->parent = env;
    return builtin_eval(f->env, lval_add(lval_sexpr(), lval_copy(f->body)));
  } else {
    return lval_copy(f);
  }
}

lval *builtin_lambda(lenv *env, lval *a) {
  LASSERT_NUM("lambda", a, 2);
  LASSERT_TYPE("lambda", a, 0, LVAL_QEXP);
  LASSERT_TYPE("lambda", a, 1, LVAL_QEXP);

  for (int i = 0; i < a->cell[0]->count; ++i) {
    LASSERT(a, (a->cell[0]->cell[i]->type == LVAL_SYM),
            "Cannot define non-symbol. Got "
            "%s, expected %s",
            type_name(a->cell[0]->cell[i]->type), type_name(LVAL_SYM));
  }

  lval *formals = lval_pop(a, 0);
  lval *body = lval_pop(a, 0);
  lval_del(a);

  return lval_lambda(formals, body);
}

void lenv_add_builtin(lenv *env, char *name, lbuiltin func) {
  lval *k = lval_sym(name);
  lval *v = lval_func(func);
  lenv_put(env, k, v);
  lval_del(k);
  lval_del(v);
}

void lenv_add_builtins(lenv *env) {
  /* Operations on List */
  lenv_add_builtin(env, "list", builtin_list);
  lenv_add_builtin(env, "head", builtin_head);
  lenv_add_builtin(env, "tail", builtin_tail);
  lenv_add_builtin(env, "eval", builtin_eval);
  lenv_add_builtin(env, "join", builtin_join);

  /* Basic Operations */
  lenv_add_builtin(env, "+", builtin_add);
  lenv_add_builtin(env, "-", builtin_minus);
  lenv_add_builtin(env, "*", builtin_product);
  lenv_add_builtin(env, "/", builtin_div);

  /* Variable Declaration */
  lenv_add_builtin(env, "let", builtin_global);
  lenv_add_builtin(env, "=", builtin_local);

  /* Function Declaration */
  lenv_add_builtin(env, "lambda", builtin_lambda);
}

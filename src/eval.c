#include "eval.h"
#include "builtins.h"
#include "error.h"
#include "mpc.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* JUST DECLERATIONS */

void lval_expr_print(lval *v, char open, char close);
void lval_str_print(lval *v);
void lval_print(lval *v);
void lval_println(lval *v);

lval *lval_read_num(mpc_ast_t *t);
lval *lval_read_str(mpc_ast_t *t);
lval *lval_read(mpc_ast_t *t);
lval *lval_eval_sexpr(lenv *e, lval *v);

lval *lenv_get(lenv *env, lval *k);
lval *lval_eval(lenv *e, lval *v);

lval *func_call(lenv *env, lval *f, lval *v);

lenv *lenv_new(void);
void lenv_del(lenv *e);
lenv *lenv_copy(lenv *e);
void lenv_def_global(lenv *env, lval *k, lval *v);

lval *lval_read_num(mpc_ast_t *t) {
  errno = 0;
  long x = strtol(t->contents, NULL, 10);
  return errno != ERANGE ? lval_num(x)
                         : lval_err("%s is invalid "
                                    "number",
                                    x);
}

lval *lval_read_str(mpc_ast_t *t) {
  t->contents[strlen(t->contents) - 1] = '\0';
  char *formatted = malloc(strlen(t->contents) + 1);
  /* Passing '+1' because we're trimming both of '"'. */
  strcpy(formatted, t->contents + 1);
  formatted = mpcf_unescape(formatted);
  lval *str = lval_str(formatted);
  free(formatted);
  return str;
}

lval *lval_read(mpc_ast_t *t) {
  if (strstr(t->tag, "number")) {
    return lval_read_num(t);
  }
  if (strstr(t->tag, "string")) {
    return lval_read_str(t);
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
        (strstr(t->children[i]->tag, "comments")) ||
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

void lval_str_print(lval *v) {
  char *tmp = malloc(strlen(v->string) + 1);
  strcpy(tmp, v->string);
  tmp = mpcf_escape(tmp);
  fprintf(stdout, "\"%s\"", tmp);
  free(tmp);
}

void lval_print(lval *v) {
  switch (v->type) {
  case LVAL_NUM:
    fprintf(stdout, "%ld", v->num);
    break;
  case LVAL_FUNC: {
    if (v->func) {
      fprintf(stdout, "<function>");
    } else {
      fprintf(stdout, "(lambda");
      lval_print(v->formals);
      putchar(' ');
      lval_print(v->body);
      putchar(')');
    }
  } break;
  case LVAL_ERR:
    fprintf(stdout, "Error: %s", v->error);
    break;
  case LVAL_SYM:
    fprintf(stdout, "%s", v->symbol);
    break;
  case LVAL_STR:
    lval_str_print(v);
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

lval *lval_join(lval *x, lval *y) {
  while (y->count) {
    x = lval_add(x, lval_pop(y, 0));
  }

  lval_del(y);
  return x;
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

lval *func_call(lenv *e, lval *f, lval *a) {

  if (f->func) {
    return f->func(e, a);
  }

  int given = a->count;
  int total = f->formals->count;

  while (a->count) {

    if (f->formals->count == 0) {
      lval_del(a);
      return lval_err("Function passed too many arguments. "
                      "Got %i, Expected %i.",
                      given, total);
    }

    lval *sym = lval_pop(f->formals, 0);

    if (strcmp(sym->symbol, "&") == 0) {

      if (f->formals->count != 1) {
        lval_del(a);
        return lval_err("Function format invalid. "
                        "Symbol '&' not followed by single symbol.");
      }

      lval *nsym = lval_pop(f->formals, 0);
      lenv_put(f->env, nsym, builtin_list(e, a));
      lval_del(sym);
      lval_del(nsym);
      break;
    }

    lval *val = lval_pop(a, 0);

    lenv_put(f->env, sym, val);

    lval_del(sym);
    lval_del(val);
  }

  lval_del(a);

  if (f->formals->count > 0 && strcmp(f->formals->cell[0]->symbol, "&") == 0) {

    if (f->formals->count != 2) {
      return lval_err("Function format invalid. "
                      "Symbol '&' not followed by single symbol.");
    }

    lval_del(lval_pop(f->formals, 0));

    lval *sym = lval_pop(f->formals, 0);
    lval *val = lval_qexpr();

    lenv_put(f->env, sym, val);
    lval_del(sym);
    lval_del(val);
  }

  if (f->formals->count == 0) {

    f->env->parent = e;

    return builtin_eval(f->env, lval_add(lval_sexpr(), lval_copy(f->body)));
  } else {
    return lval_copy(f);
  }
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

  /* Comparison Operators */
  lenv_add_builtin(env, ">", builtin_gt);
  lenv_add_builtin(env, ">=", builtin_gte);
  lenv_add_builtin(env, "<", builtin_lt);
  lenv_add_builtin(env, "<=", builtin_lte);

  /* Equality Operators */
  lenv_add_builtin(env, "==", builtin_eq);
  lenv_add_builtin(env, "!=", builtin_neq);

  /* IF.. */
  lenv_add_builtin(env, "if", builtin_if);

  /* Variable Declaration */
  lenv_add_builtin(env, "let", builtin_global);
  lenv_add_builtin(env, "=", builtin_local);

  /* Function Declaration */
  lenv_add_builtin(env, "lambda", builtin_lambda);

  lenv_add_builtin(env, "print", builtin_print);
  lenv_add_builtin(env, "err", builtin_error);
  /* FIXMEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE */
  /* lenv_add_builtin(env, "load", builtin_load); */
}

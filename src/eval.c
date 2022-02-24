#include "eval.h"
#include "builtins.h"
#include "error.h"
#include "mpc.h"
#include "types.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* JUST DECLERATIONS */

void print_expr(value *v, char open, char close);
void print_str(value *v);
void print(value *v);
void println(value *v);

value *read_num(mpc_ast_t *t);
value *read_str(mpc_ast_t *t);
value *read_expr(mpc_ast_t *t);
value *eval_sexpr(scope *e, value *v);

value *get_scope(scope *env, value *k);
value *eval(scope *e, value *v);

value *func_call(scope *env, value *f, value *v);

value *read_num(mpc_ast_t *t) {
  errno = 0;
  double x = strtod(t->contents, NULL);
  return errno != ERANGE ? new_num(x)
                         : new_err("%s is invalid "
                                   "number",
                                   x);
}

value *read_str(mpc_ast_t *t) {
  t->contents[strlen(t->contents) - 1] = '\0';
  char *formatted = malloc(strlen(t->contents) + 1);
  /* Passing '+1' because we're trimming both of '"'. */
  strcpy(formatted, t->contents + 1);
  formatted = mpcf_unescape(formatted);
  value *str = new_string(formatted);
  free(formatted);
  return str;
}

value *read_expr(mpc_ast_t *t) {
  if (strstr(t->tag, "number")) {
    return read_num(t);
  }
  if (strstr(t->tag, "string")) {
    return read_str(t);
  }

  if (strstr(t->tag, "symbol")) {
    return new_symbol(t->contents);
  }

  value *x = NULL;

  if (!strcmp(t->tag, ">")) {
    x = new_sexp();
  }
  if (strstr(t->tag, "sexpr")) {
    x = new_sexp();
  }
  if (strstr(t->tag, "qexpr")) {
    x = new_qexp();
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
      x = add_value(x, read_expr(t->children[i]));
    }
  }

  return x;
}

void print_expr(value *v, char open, char close) {
  putchar(open);
  for (int i = 0; i < v->count; i++) {

    print(v->cell[i]);

    if (i != (v->count - 1)) {
      putchar(' ');
    }
  }
  putchar(close);
}

void print_str(value *v) {
  char *tmp = malloc(strlen(v->string) + 1);
  strcpy(tmp, v->string);
  tmp = mpcf_escape(tmp);
  fprintf(stdout, "\"%s\"", tmp);
  free(tmp);
}

void print(value *v) {
  switch (v->type) {
  case NUMBER: {
    if (ifDouble(v->num) == 1) {
      fprintf(stdout, "%Lf", v->num);
    } else {
      fprintf(stdout, "%ld", (long)v->num);
    }
  } break;
  case FUNCTION: {
    if (v->func) {
      fprintf(stdout, "<function>");
    } else {
      fprintf(stdout, "(lambda");
      print(v->formals);
      putchar(' ');
      print(v->body);
      putchar(')');
    }
  } break;
  case OK:
    break;
  case ERR:
    fprintf(stdout, "Error: %s", v->error);
    break;
  case SYMBOL:
    fprintf(stdout, "%s", v->symbol);
    break;
  case STRING:
    print_str(v);
    break;
  case QEXPRESSION:
    print_expr(v, '{', '}');
    break;
  case SEXPRESSION:
    print_expr(v, '(', ')');
    break;
  }
}

void println(value *v) {
  if (v->type != OK) {
    print(v);
    putchar('\n');
  }
}

value *pop(value *v, int i) {

  value *x = v->cell[i];

  memmove(&v->cell[i], &v->cell[i + 1], sizeof(value *) * (v->count - i - 1));

  v->count--;

  v->cell = realloc(v->cell, sizeof(value *) * v->count);
  return x;
}

value *take(value *v, int i) {
  value *x = pop(v, i);
  del_value(v);
  return x;
}

value *eval_sexpr(scope *e, value *v) {

  for (int i = 0; i < v->count; i++) {
    v->cell[i] = eval(e, v->cell[i]);
  }

  for (int i = 0; i < v->count; i++) {
    if (v->cell[i]->type == ERR) {
      return take(v, i);
    }
  }

  if (v->count == 0) {
    return v;
  }

  if (v->count == 1) {
    return take(v, 0);
  }

  value *f = pop(v, 0);
  if (f->type != FUNCTION) {
    value *err = new_err("%s is not a function!", type_name(f->type));
    del_value(f);
    del_value(v);
    return err;
  }

  value *result = func_call(e, f, v);
  del_value(f);
  return result;
}

value *join(value *x, value *y) {
  while (y->count) {
    x = add_value(x, pop(y, 0));
  }

  del_value(y);
  return x;
}

void put(scope *env, value *key, value *val) {
  for (int i = 0; i < env->count; ++i) {
    if (strcmp(env->syms[i], key->symbol) == 0) {
      del_value(env->vals[i]);
      env->vals[i] = copy_value(val);
      return;
    }
  }

  env->count++;
  env->vals = realloc(env->vals, (sizeof(value *) * env->count));
  env->syms = realloc(env->syms, (sizeof(char *) * env->count));

  env->vals[env->count - 1] = copy_value(val);
  env->syms[env->count - 1] = malloc(strlen(key->symbol) + 1);
  strcpy(env->syms[env->count - 1], key->symbol);
  return;
}

value *get(scope *env, value *k) {
  for (int i = 0; i < env->count; ++i) {
    if (!strcmp(env->syms[i], k->symbol))
      return copy_value(env->vals[i]);
  }

  if (env->parent) {
    return get(env->parent, k);
  } else {
    return new_err("Symbol '%s' not found!", k->symbol);
  }
}

void global_scope(scope *env, value *k, value *v) {
  while (env->parent) {
    env = env->parent;
  }
  put(env, k, v);
}

value *eval(scope *e, value *v) {
  if (v->type == SEXPRESSION) {
    return eval_sexpr(e, v);
  }
  if (v->type == SYMBOL) {
    value *x = get(e, v);
    del_value(v);
    return x;
  }

  return v;
}

value *func_call(scope *e, value *f, value *a) {

  if (f->func) {
    return f->func(e, a);
  }

  int given = a->count;
  int total = f->formals->count;

  while (a->count) {

    if (f->formals->count == 0) {
      del_value(a);
      return new_err("Function passed too many arguments. "
                     "Got %i, Expected %i.",
                     given, total);
    }

    value *sym = pop(f->formals, 0);

    if (strcmp(sym->symbol, "&") == 0) {

      if (f->formals->count != 1) {
        del_value(a);
        return new_err("Function format invalid. "
                       "Symbol '&' not followed by single symbol.");
      }

      value *nsym = pop(f->formals, 0);
      put(f->env, nsym, builtin_list(e, a));
      del_value(sym);
      del_value(nsym);
      break;
    }

    value *val = pop(a, 0);

    put(f->env, sym, val);

    del_value(sym);
    del_value(val);
  }

  del_value(a);

  if (f->formals->count > 0 && strcmp(f->formals->cell[0]->symbol, "&") == 0) {

    if (f->formals->count != 2) {
      return new_err("Function format invalid. "
                     "Symbol '&' not followed by single symbol.");
    }

    del_value(pop(f->formals, 0));

    value *sym = pop(f->formals, 0);
    value *val = new_qexp();

    put(f->env, sym, val);
    del_value(sym);
    del_value(val);
  }

  if (f->formals->count == 0) {

    f->env->parent = e;

    return builtin_eval(f->env, add_value(new_sexp(), copy_value(f->body)));
  } else {
    return copy_value(f);
  }
}

void add_builtin(scope *env, char *name, function func) {
  value *k = new_symbol(name);
  value *v = new_func(func);
  put(env, k, v);
  del_value(k);
  del_value(v);
}

void add_builtins(scope *env) {
  /* Operations on List */
  add_builtin(env, "list", builtin_list);
  add_builtin(env, "head", builtin_head);
  add_builtin(env, "tail", builtin_tail);
  add_builtin(env, "eval", builtin_eval);
  add_builtin(env, "join", builtin_join);

  /* Basic Operations */
  add_builtin(env, "+", builtin_add);
  add_builtin(env, "-", builtin_minus);
  add_builtin(env, "*", builtin_product);
  add_builtin(env, "/", builtin_div);
  add_builtin(env, "%", builtin_modulus);
  add_builtin(env, "!", builtin_not);
  add_builtin(env, "~", builtin_negate);
  add_builtin(env, "^", builtin_bin_xor);
  add_builtin(env, "|", builtin_bin_or);
  add_builtin(env, "&", builtin_bin_and);
  add_builtin(env, "||", builtin_log_or);
  add_builtin(env, "&&", builtin_log_and);
  add_builtin(env, "<<", builtin_lshift);
  add_builtin(env, ">>", builtin_rshift);

  /* Comparison Operators */
  add_builtin(env, ">", builtin_gt);
  add_builtin(env, ">=", builtin_gte);
  add_builtin(env, "<", builtin_lt);
  add_builtin(env, "<=", builtin_lte);

  /* Equality Operators */
  add_builtin(env, "==", builtin_eq);
  add_builtin(env, "!=", builtin_neq);

  /* IF.. */
  add_builtin(env, "if", builtin_if);

  /* Variable Declaration */
  add_builtin(env, "let", builtin_global);
  add_builtin(env, "=", builtin_local);

  /* Function Declaration */
  add_builtin(env, "lambda", builtin_lambda);

  add_builtin(env, "loop", builtin_loop);

  add_builtin(env, "exit", builtin_exit);

  /* Mathematical Function */
  add_builtin(env, "**", builtin_pow);
  add_builtin(env, "sqrt", builtin_sqrt);

  add_builtin(env, "sin", builtin_sin);
  add_builtin(env, "cos", builtin_cos);
  add_builtin(env, "tan", builtin_tan);

  add_builtin(env, "asin", builtin_asin);
  add_builtin(env, "acos", builtin_acos);
  add_builtin(env, "atan", builtin_atan);

  /* String Functions */
  add_builtin(env, "strlen", builtin_strlen);
  add_builtin(env, "strcmp", builtin_strcmp);
  add_builtin(env, "to_uppercase", builtin_touppercase);
  add_builtin(env, "to_lowercase", builtin_tolowercase);

  /* Random Number Function */
  add_builtin(env, "rand", builtin_rand);
  add_builtin(env, "rand_str", builtin_randstr);
  add_builtin(env, "frand", builtin_frand);

  /* Utility Function */
  add_builtin(env, "print", builtin_print);
  add_builtin(env, "err", builtin_error);
  add_builtin(env, "load", builtin_load);
}

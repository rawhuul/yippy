#include "builtins.h"
#include "eval.h"
#include "parser.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

lval *builtin_load(lenv *env, lval *a) {
  LASSERT_NUM("load", a, 1);
  LASSERT_TYPE("load", a, 0, LVAL_STR);

  parser *p = parse();
  mpc_result_t r;
  if (mpc_parse_contents(a->cell[0]->string, p->Yippy, &r)) {

    lval *expr = lval_read(r.output);
    mpc_ast_delete(r.output);

    while (expr->count) {
      lval *x = lval_eval(env, lval_pop(expr, 0));
      if (x->type == LVAL_ERR) {
        lval_println(x);
      }
      lval_del(x);
    }

    lval_del(expr);
    lval_del(a);

    p = parse_clean(p);
    return lval_ok();
  } else {
    char *error_msg = mpc_err_string(r.error);
    mpc_err_delete(r.error);
    lval *err = lval_err("Could not load library %s", error_msg);
    free(error_msg);
    lval_del(a);

    p = parse_clean(p);
    return err;
  }
}

lval *builtin_print(lenv *env, lval *a) {
  for (int i = 0; i < a->count; ++i) {
    lval_print(a->cell[i]);
    putchar(' ');
  }

  putchar('\n');
  lval_del(a);
  return lval_ok();
}

lval *builtin_error(lenv *env, lval *a) {
  LASSERT_NUM("error", a, 1);
  LASSERT_TYPE("error", a, 0, LVAL_STR);

  lval *err = lval_err(a->cell[0]->string);
  lval_del(a);
  return err;
}

lval *builtin_add(lenv *env, lval *a) { return builtin_op(env, a, "+"); }
lval *builtin_minus(lenv *env, lval *a) { return builtin_op(env, a, "-"); }
lval *builtin_div(lenv *env, lval *a) { return builtin_op(env, a, "/"); }
lval *builtin_product(lenv *env, lval *a) { return builtin_op(env, a, "*"); }
lval *builtin_modulus(lenv *env, lval *a) { return builtin_op(env, a, "%"); }
lval *builtin_not(lenv *env, lval *a) { return builtin_op(env, a, "!"); }
lval *builtin_negate(lenv *env, lval *a) { return builtin_op(env, a, "~"); }

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

  if (a->count == 0) {
    if (!strcmp(op, "+")) {
      x->num = (x->num);
    } else if (!strcmp(op, "-")) {
      x->num = -(x->num);
    } else if (!strcmp(op, "~")) {
      x->num = ~((long)x->num);
    } else if (!strcmp(op, "!")) {
      x->num = !(x->num);
    }
  }

  while (a->count > 0) {

    lval *y = lval_pop(a, 0);

    if (!strcmp(op, "!") || !strcmp(op, "~")) {
      return lval_err("'%s' works with only 1 argument.", op);
    }

    if (!strcmp(op, "+")) {
      x->num += y->num;
    } else if (!strcmp(op, "-")) {
      x->num -= y->num;
    } else if (!strcmp(op, "*")) {
      x->num *= y->num;
    } else if (!strcmp(op, "%")) {
      x->num = (long)x->num % (long)y->num;
    } else if (strcmp(op, "/") == 0) {
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

lval *builtin_bin_xor(lenv *env, lval *a) {
  return builtin_logical_op(env, a, "^");
}
lval *builtin_bin_and(lenv *env, lval *a) {
  return builtin_logical_op(env, a, "&");
}
lval *builtin_bin_or(lenv *env, lval *a) {
  return builtin_logical_op(env, a, "|");
}

lval *builtin_log_and(lenv *env, lval *a) {
  return builtin_logical_op(env, a, "&&");
}
lval *builtin_log_or(lenv *env, lval *a) {
  return builtin_logical_op(env, a, "||");
}

lval *builtin_lshift(lenv *env, lval *a) {
  return builtin_logical_op(env, a, "<<");
}
lval *builtin_rshift(lenv *env, lval *a) {
  return builtin_logical_op(env, a, ">>");
}

lval *builtin_logical_op(lenv *env, lval *a, char *operator) {
  LASSERT_NUM(operator, a, 2);
  LASSERT_TYPE(operator, a, 0, LVAL_NUM);
  LASSERT_TYPE(operator, a, 1, LVAL_NUM);

  int result = 0;
  if (!strcmp(operator, ">>")) {
    result = ((long)a->cell[0]->num >> (long)a->cell[1]->num);
  } else if (!strcmp(operator, "<<")) {
    result = ((long)a->cell[0]->num << (long)a->cell[1]->num);
  } else if (!strcmp(operator, "&&")) {
    result = (a->cell[0]->num && a->cell[1]->num);
  } else if (!strcmp(operator, "||")) {
    result = (a->cell[0]->num || a->cell[1]->num);
  } else if (!strcmp(operator, "&")) {
    result = ((long)a->cell[0]->num & (long)a->cell[1]->num);
  } else if (!strcmp(operator, "|")) {
    result = ((long)a->cell[0]->num | (long)a->cell[1]->num);
  } else if (!strcmp(operator, "^")) {
    result = ((long)a->cell[0]->num ^ (long)a->cell[1]->num);
  }

  lval_del(a);
  return lval_num(result);
}

lval *builtin_cmp(lenv *env, lval *a, char *operator) {
  LASSERT_NUM(operator, a, 2);
  LASSERT_TYPE(operator, a, 0, LVAL_NUM);
  LASSERT_TYPE(operator, a, 1, LVAL_NUM);

  int result = 0;
  if (!strcmp(operator, ">")) {
    result = (a->cell[0]->num > a->cell[1]->num);
  }

  if (!strcmp(operator, ">=")) {
    result = (a->cell[0]->num >= a->cell[1]->num);
  }

  if (!strcmp(operator, "<")) {
    result = (a->cell[0]->num < a->cell[1]->num);
  }

  if (!strcmp(operator, "<=")) {
    result = (a->cell[0]->num <= a->cell[1]->num);
  }

  lval_del(a);
  return lval_num(result);
}

lval *builtin_gt(lenv *env, lval *a) { return builtin_cmp(env, a, ">"); }
lval *builtin_gte(lenv *env, lval *a) { return builtin_cmp(env, a, ">="); }
lval *builtin_lt(lenv *env, lval *a) { return builtin_cmp(env, a, "<"); }
lval *builtin_lte(lenv *env, lval *a) { return builtin_cmp(env, a, "<="); }

int lval_eq(lval *x, lval *y) {
  if (x->type != y->type) {
    return 0;
  }

  switch (x->type) {
  case LVAL_NUM:
    return (x->num == y->num);

  case LVAL_ERR:
    return (!strcmp(x->error, y->error));
  case LVAL_SYM:
    return (!strcmp(x->symbol, y->symbol));
  case LVAL_STR:
    return (!strcmp(x->string, y->string));

  case LVAL_FUNC: {
    if (x->func || y->func) {
      return x->func == y->func;
    } else {
      return (lval_eq(x->formals, y->formals) && lval_eq(x->body, y->body));
    }
  }
  case LVAL_SEXP:
  case LVAL_QEXP: {
    if (x->count != y->count) {
      return 0;
    }
    for (int i = 0; i < x->count; ++i) {
      if (!lval_eq(x->cell[i], y->cell[i])) {
        return 0;
      }
    }
    return 1;
  }

  default:
    break;
  }

  return 0;
}

lval *builtin_eq(lenv *env, lval *a) { return builtin_equality(env, a, "=="); }
lval *builtin_neq(lenv *env, lval *a) { return builtin_equality(env, a, "!="); }
lval *builtin_equality(lenv *env, lval *a, char *operator) {
  LASSERT_NUM(operator, a, 2);

  int result = 0;

  if (!strcmp(operator, "==")) {
    result = lval_eq(a->cell[0], a->cell[1]);
  }
  if (!strcmp(operator, "!=")) {
    result = !lval_eq(a->cell[0], a->cell[1]);
  }

  lval_del(a);
  return lval_num(result);
}

lval *builtin_if(lenv *env, lval *a) {
  LASSERT_NUM("if", a, 3);
  LASSERT_TYPE("if", a, 0, LVAL_NUM);
  LASSERT_TYPE("if", a, 1, LVAL_QEXP);
  LASSERT_TYPE("if", a, 2, LVAL_QEXP);

  lval *res = NULL;

  a->cell[1]->type = LVAL_SEXP;
  a->cell[2]->type = LVAL_SEXP;

  if (a->cell[0]->num) {
    res = lval_eval(env, lval_pop(a, 1));
  } else {
    res = lval_eval(env, lval_pop(a, 2));
  }

  lval_del(a);
  return res;
}

lval *builtin_global(lenv *env, lval *val) {
  return builtin_let(env, val, "let");
}

lval *builtin_local(lenv *env, lval *val) { return builtin_let(env, val, "="); }

lval *builtin_let(lenv *env, lval *val, char *scope) {
  LASSERT_TYPE(scope, val, 0, LVAL_QEXP);

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
  return lval_ok();
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

lval *builtin_loop(lenv *env, lval *v) {
  LASSERT_NUM("repeat", v, 2);
  LASSERT_TYPE("repeat", v, 0, LVAL_NUM);
  LASSERT_TYPE("repeat", v, 1, LVAL_QEXP);
  v->cell[1]->type = LVAL_SEXP;
  int count = v->cell[0]->num;
  lval *res;

  for (int i = 0; i < count; ++i) {
    res = lval_pop(lval_copy(v), 1);
    res = lval_eval(env, res);

    res = NULL;
  }

  lval_del(v);
  return lval_ok();
}

lval *builtin_exit(lenv *env, lval *v) {
  LASSERT_NUM("exit", v, 1);
  LASSERT_TYPE("exit", v, 0, LVAL_NUM);

  exit(v->cell[0]->count <= 0);

  return lval_ok();
}

lval *builtin_pow(lenv *env, lval *v) {
  LASSERT_NUM("**", v, 2);
  LASSERT_TYPE("**", v, 0, LVAL_NUM);
  LASSERT_TYPE("**", v, 1, LVAL_NUM);

  lval *res = lval_num(pow(v->cell[0]->num, v->cell[1]->num));

  lval_del(v);
  return res;
}

lval *builtin_sqrt(lenv *env, lval *v) {
  LASSERT_NUM("sqrt", v, 1);
  LASSERT_TYPE("sqrt", v, 0, LVAL_NUM);

  lval *res = lval_num(sqrt(v->cell[0]->num));

  lval_del(v);
  return res;
}

lval *builtin_sin(lenv *env, lval *v) {
  LASSERT_NUM("sin", v, 1);
  LASSERT_TYPE("sin", v, 0, LVAL_NUM);

  lval *res = lval_num(sin(v->cell[0]->num));

  lval_del(v);
  return res;
}

lval *builtin_cos(lenv *env, lval *v) {
  LASSERT_NUM("cos", v, 1);
  LASSERT_TYPE("cos", v, 0, LVAL_NUM);

  lval *res = lval_num(cos(v->cell[0]->num));

  lval_del(v);
  return res;
}

lval *builtin_tan(lenv *env, lval *v) {
  LASSERT_NUM("tan", v, 1);
  LASSERT_TYPE("tan", v, 0, LVAL_NUM);

  lval *res = lval_num(tan(v->cell[0]->num));

  lval_del(v);
  return res;
}

lval *builtin_asin(lenv *env, lval *v) {
  LASSERT_NUM("asin", v, 1);
  LASSERT_TYPE("asin", v, 0, LVAL_NUM);

  lval *res = lval_num(asin(v->cell[0]->num));

  lval_del(v);
  return res;
}

lval *builtin_acos(lenv *env, lval *v) {
  LASSERT_NUM("acos", v, 1);
  LASSERT_TYPE("acos", v, 0, LVAL_NUM);

  lval *res = lval_num(acos(v->cell[0]->num));

  lval_del(v);
  return res;
}

lval *builtin_atan(lenv *env, lval *v) {
  LASSERT_NUM("atan", v, 1);
  LASSERT_TYPE("atan", v, 0, LVAL_NUM);

  lval *res = lval_num(atan(v->cell[0]->num));

  lval_del(v);
  return res;
}

lval *builtin_strlen(lenv *env, lval *v) {
  LASSERT_NUM("str_len", v, 1);
  LASSERT_TYPE("str_len", v, 0, LVAL_STR);

  lval *res = lval_num(strlen(v->cell[0]->string));

  lval_del(v);
  return res;
}

lval *builtin_strcmp(lenv *env, lval *v) {
  LASSERT_NUM("str_cmp", v, 2);
  LASSERT_TYPE("str_cmp", v, 0, LVAL_STR);
  LASSERT_TYPE("str_cmp", v, 1, LVAL_STR);

  int res = strcmp(v->cell[0]->string, v->cell[1]->string);

  lval_del(v);
  return res == 0 ? lval_num(1) : lval_num(0);
}

char *toUpper(char *string) {
  for (int i = 0; string[i] != '\0'; i++) {
    if ((string[i] >= 'a') && (string[i] <= 'z')) {
      string[i] = string[i] - 32;
    }
  }

  return string;
}

lval *builtin_touppercase(lenv *env, lval *v) {
  LASSERT_NUM("to_upper", v, 1);
  LASSERT_TYPE("to_upper", v, 0, LVAL_STR);

  char str[4096];
  strcpy(str, v->cell[0]->string);

  lval *res = lval_str(toUpper(str));

  lval_del(v);
  return res;
}

char *toLower(char *string) {
  for (int i = 0; string[i] != '\0'; i++) {
    if (string[i] >= 'A' && string[i] <= 'Z') {
      string[i] = string[i] + 32;
    }
  }
  return string;
}

lval *builtin_tolowercase(lenv *env, lval *v) {
  LASSERT_NUM("to_lower", v, 1);
  LASSERT_TYPE("to_lower", v, 0, LVAL_STR);

  char str[4096];
  strcpy(str, v->cell[0]->string);

  lval *res = lval_str(toLower(str));

  lval_del(v);
  return res;
}

lval *builtin_rand(lenv *env, lval *a) {
  LASSERT_NUM("rand", a, 2);
  LASSERT_TYPE("rand", a, 0, LVAL_NUM);
  LASSERT_TYPE("rand", a, 1, LVAL_NUM);

  int res = 0;
  int start = a->cell[0]->num;
  int end = a->cell[1]->num;

  srand(time(NULL));
  if (start > end) {
    res = start;
  } else if (start == 0 && end == 0) {
    res = rand();
  } else {
    res = start + (rand() % end);
  }

  lval_del(a);
  return lval_num(res);
}

char *random_str(int len) {
  char *str = malloc(len);

  if (!str) {
    return NULL;
  }

  char book[] = "ABCDEFGHIJKLMNOPQRSTUWXYZabcdefghijklmnopqrstuvwxyz0123456789";
  int count = strlen(book);

  srand(time(NULL));

  for (int i = 0; i < len; i++) {
    int key = rand() % count;
    str[i] = book[key];
  }

  str[len] = '\0';
  return str;
}

lval *builtin_randstr(lenv *env, lval *a) {
  LASSERT_NUM("rand_str", a, 1);
  LASSERT_TYPE("rand_str", a, 0, LVAL_NUM);

  unsigned int count = a->cell[0]->num;
  char *str = random_str(count);
  if (!str) {
    return lval_err("Unexpected, internal error!!");
  }

  lval_del(a);
  return lval_str(str);
}

lval *builtin_frand(lenv *env, lval *a) {
  LASSERT_NUM("frand", a, 1);
  LASSERT_TYPE("frand", a, 0, LVAL_NUM);

  srand(time(NULL));
  double result = ((double)rand() / (double)RAND_MAX) * (a->cell[0]->num);

  return lval_num(result);
}

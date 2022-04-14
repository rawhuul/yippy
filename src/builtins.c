#include "builtins.h"
#include "eval.h"
#include "parser.h"
#include "types.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

value *builtin_load(scope *env, value *a) {
  LASSERT_NUM("load", a, 1);
  LASSERT_TYPE("load", a, 0, STRING);

  parser *p = parse();
  mpc_result_t r;
  if (mpc_parse_contents(a->cell[0]->string, p->Yippy, &r)) {

    value *expr = read_expr(r.output);
    mpc_ast_delete(r.output);

    while (expr->count) {
      value *x = eval(env, pop(expr, 0));
      if (x->type == ERR) {
        println(x);
      }
      del_value(x);
    }

    del_value(expr);
    del_value(a);

    p = parse_clean(p);
    return ok();
  } else {
    char *error_msg = mpc_err_string(r.error);
    mpc_err_delete(r.error);
    value *err = new_err("Could not load library %s", error_msg);
    free(error_msg);
    del_value(a);

    p = parse_clean(p);
    return err;
  }
}

#ifndef COMP_WASM
value *builtin_print(scope *env, value *a) {
  for (int i = 0; i < a->count; ++i) {
    print(a->cell[i]);
    putchar(' ');
  }

  putchar('\n');
  del_value(a);
  return ok();
}
#else
value *builtin_print(scope *env, value *a) {
  char result[4096];

  for (int i = 0; i < a->count; ++i) {
    print(a->cell[i]);
    strcat(result, " ");
  }

  strcat(result, "\n");
  del_value(a);
  return new_string(result);
}
#endif

value *builtin_error(scope *env, value *a) {
  LASSERT_NUM("error", a, 1);
  LASSERT_TYPE("error", a, 0, STRING);

  value *err = new_err(a->cell[0]->string);
  del_value(a);
  return err;
}

value *builtin_add(scope *env, value *a) { return builtin_op(env, a, "+"); }
value *builtin_minus(scope *env, value *a) { return builtin_op(env, a, "-"); }
value *builtin_div(scope *env, value *a) { return builtin_op(env, a, "/"); }
value *builtin_product(scope *env, value *a) { return builtin_op(env, a, "*"); }
value *builtin_modulus(scope *env, value *a) { return builtin_op(env, a, "%"); }
value *builtin_not(scope *env, value *a) { return builtin_op(env, a, "!"); }
value *builtin_negate(scope *env, value *a) { return builtin_op(env, a, "~"); }

value *builtin_op(scope *env, value *a, char *op) {

  for (int i = 0; i < a->count; i++) {
    if (a->cell[i]->type != NUMBER) {
      value *err = new_err("Cannot operate on "
                           "non-number, as argument %d "
                           "is %s!",
                           i + 1, type_name(a->cell[i]->type));
      del_value(a);
      return err;
    }
  }

  value *x = pop(a, 0);

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

    value *y = pop(a, 0);

    if (!strcmp(op, "!") || !strcmp(op, "~")) {
      return new_err("'%s' works with only 1 argument.", op);
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
        del_value(x);
        del_value(y);
        x = new_err("You're trying to divide "
                    "%d By Zero, which is not "
                    "allowed!",
                    x->num);
        break;
      }
      x->num /= y->num;
    }

    del_value(y);
  }

  del_value(a);
  return x;
}

value *builtin_bin_xor(scope *env, value *a) {
  return builtin_logical_op(env, a, "^");
}
value *builtin_bin_and(scope *env, value *a) {
  return builtin_logical_op(env, a, "&");
}
value *builtin_bin_or(scope *env, value *a) {
  return builtin_logical_op(env, a, "|");
}

value *builtin_log_and(scope *env, value *a) {
  return builtin_logical_op(env, a, "&&");
}
value *builtin_log_or(scope *env, value *a) {
  return builtin_logical_op(env, a, "||");
}

value *builtin_lshift(scope *env, value *a) {
  return builtin_logical_op(env, a, "<<");
}
value *builtin_rshift(scope *env, value *a) {
  return builtin_logical_op(env, a, ">>");
}

value *builtin_logical_op(scope *env, value *a, char *operator) {
  LASSERT_NUM(operator, a, 2);
  LASSERT_TYPE(operator, a, 0, NUMBER);
  LASSERT_TYPE(operator, a, 1, NUMBER);

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

  del_value(a);
  return new_num(result);
}

value *builtin_cmp(scope *env, value *a, char *operator) {
  LASSERT_NUM(operator, a, 2);
  LASSERT_TYPE(operator, a, 0, NUMBER);
  LASSERT_TYPE(operator, a, 1, NUMBER);

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

  del_value(a);
  return new_num(result);
}

value *builtin_gt(scope *env, value *a) { return builtin_cmp(env, a, ">"); }
value *builtin_gte(scope *env, value *a) { return builtin_cmp(env, a, ">="); }
value *builtin_lt(scope *env, value *a) { return builtin_cmp(env, a, "<"); }
value *builtin_lte(scope *env, value *a) { return builtin_cmp(env, a, "<="); }

int eq(value *x, value *y) {
  if (x->type != y->type) {
    return 0;
  }

  switch (x->type) {
  case NUMBER:
    return (x->num == y->num);

  case ERR:
    return (!strcmp(x->error, y->error));
  case SYMBOL:
    return (!strcmp(x->symbol, y->symbol));
  case STRING:
    return (!strcmp(x->string, y->string));

  case FUNCTION: {
    if (x->func || y->func) {
      return x->func == y->func;
    } else {
      return (eq(x->formals, y->formals) && eq(x->body, y->body));
    }
  }
  case SEXPRESSION:
  case QEXPRESSION: {
    if (x->count != y->count) {
      return 0;
    }
    for (int i = 0; i < x->count; ++i) {
      if (!eq(x->cell[i], y->cell[i])) {
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

value *builtin_eq(scope *env, value *a) {
  return builtin_equality(env, a, "==");
}
value *builtin_neq(scope *env, value *a) {
  return builtin_equality(env, a, "!=");
}
value *builtin_equality(scope *env, value *a, char *operator) {
  LASSERT_NUM(operator, a, 2);

  int result = 0;

  if (!strcmp(operator, "==")) {
    result = eq(a->cell[0], a->cell[1]);
  }
  if (!strcmp(operator, "!=")) {
    result = !eq(a->cell[0], a->cell[1]);
  }

  del_value(a);
  return new_num(result);
}

value *builtin_if(scope *env, value *a) {
  LASSERT_NUM("if", a, 3);
  LASSERT_TYPE("if", a, 0, NUMBER);
  LASSERT_TYPE("if", a, 1, QEXPRESSION);
  LASSERT_TYPE("if", a, 2, QEXPRESSION);

  value *res = NULL;

  a->cell[1]->type = SEXPRESSION;
  a->cell[2]->type = SEXPRESSION;

  if (a->cell[0]->num) {
    res = eval(env, pop(a, 1));
  } else {
    res = eval(env, pop(a, 2));
  }

  del_value(a);
  return res;
}

value *builtin_global(scope *env, value *val) {
  return builtin_let(env, val, "let");
}

value *builtin_local(scope *env, value *val) {
  return builtin_let(env, val, "=");
}

value *builtin_let(scope *env, value *val, char *scope) {
  LASSERT_TYPE(scope, val, 0, QEXPRESSION);

  value *symbol = val->cell[0];

  for (int i = 0; i < symbol->count; i++) {
    LASSERT(val, symbol->cell[i]->type == SYMBOL,
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
      global_scope(env, symbol->cell[i], val->cell[i + 1]);
    }
    if (!strcmp(scope, "=")) {
      put(env, symbol->cell[i], val->cell[i + 1]);
    }
  }

  del_value(val);
  return ok();
}

value *builtin(scope *e, value *a, char *func) {
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
    del_value(a);
    return new_err("Got '%s', unknown Function!", func);
  }
}

value *builtin_join(scope *e, value *a) {
  for (int i = 0; i < a->count; ++i) {
    LASSERT_TYPE("join", a, i, QEXPRESSION);
  }

  value *x = pop(a, 0);

  while (a->count) {
    x = join(x, pop(a, 0));
  }

  del_value(a);
  return x;
}

value *builtin_head(scope *e, value *a) {
  LASSERT_NUM("head", a, 1)
  LASSERT_TYPE("head", a, 0, QEXPRESSION);
  LASSERT(a, a->cell[0]->count != 0,
          "Function 'head' passed %d "
          "arguments!",
          a->cell[0]->count);

  value *v = take(a, 0);

  while (v->count > 1) {
    del_value(pop(v, 1));
  }
  return v;
}

value *builtin_tail(scope *e, value *a) {
  LASSERT_NUM("tail", a, 1);
  LASSERT_TYPE("tail", a, 0, QEXPRESSION);
  LASSERT(a, a->cell[0]->count != 0,
          "Function 'tail' passed %d "
          "arguments!",
          a->cell[0]->count);

  value *v = take(a, 0);
  del_value(pop(v, 0));
  return v;
}

value *builtin_list(scope *e, value *a) {
  a->type = QEXPRESSION;
  return a;
}

value *builtin_range(scope *e, value *v) {
  LASSERT_NUM("range", v, 2);
  LASSERT_TYPE("range", v, 0, NUMBER);
  LASSERT_TYPE("range", v, 1, NUMBER);

  long double start = v->cell[0]->num;
  long double end = v->cell[1]->num;
  value *result = new_qexp();

  for (long double i = start; i <= end; ++i) {
    result = add_value(result, new_num(i));
  }

  del_value(v);
  return result;
}

value *builtin_eval(scope *e, value *a) {
  LASSERT_NUM("eval", a, 1);
  LASSERT_TYPE("eval", a, 0, QEXPRESSION);

  value *x = take(a, 0);
  x->type = SEXPRESSION;
  return eval(e, x);
}

value *builtin_lambda(scope *env, value *a) {
  LASSERT_NUM("lambda", a, 2);
  LASSERT_TYPE("lambda", a, 0, QEXPRESSION);
  LASSERT_TYPE("lambda", a, 1, QEXPRESSION);

  for (int i = 0; i < a->cell[0]->count; ++i) {
    LASSERT_TYPE("lambda", a->cell[0], i, SYMBOL);
  }

  value *formals = pop(a, 0);
  value *body = pop(a, 0);
  del_value(a);

  return new_lambda(formals, body);
}

value *builtin_loop(scope *env, value *v) {
  LASSERT_NUM("repeat", v, 2);
  LASSERT_TYPE("repeat", v, 0, NUMBER);
  LASSERT_TYPE("repeat", v, 1, QEXPRESSION);
  v->cell[1]->type = SEXPRESSION;
  int count = v->cell[0]->num;
  value *res;

  for (int i = 0; i < count; ++i) {
    res = pop(copy_value(v), 1);
    res = eval(env, res);

    res = NULL;
  }

  del_value(v);
  return ok();
}

value *builtin_exit(scope *env, value *v) {
  LASSERT_NUM("exit", v, 1);
  LASSERT_TYPE("exit", v, 0, NUMBER);

  exit(v->cell[0]->count <= 0);

  return ok();
}

value *builtin_pow(scope *env, value *v) {
  LASSERT_NUM("**", v, 2);
  LASSERT_TYPE("**", v, 0, NUMBER);
  LASSERT_TYPE("**", v, 1, NUMBER);

  value *res = new_num(pow(v->cell[0]->num, v->cell[1]->num));

  del_value(v);
  return res;
}

value *builtin_sqrt(scope *env, value *v) {
  LASSERT_NUM("sqrt", v, 1);
  LASSERT_TYPE("sqrt", v, 0, NUMBER);

  value *res = new_num(sqrt(v->cell[0]->num));

  del_value(v);
  return res;
}

value *builtin_sin(scope *env, value *v) {
  LASSERT_NUM("sin", v, 1);
  LASSERT_TYPE("sin", v, 0, NUMBER);

  value *res = new_num(sin(v->cell[0]->num));

  del_value(v);
  return res;
}

value *builtin_cos(scope *env, value *v) {
  LASSERT_NUM("cos", v, 1);
  LASSERT_TYPE("cos", v, 0, NUMBER);

  value *res = new_num(cos(v->cell[0]->num));

  del_value(v);
  return res;
}

value *builtin_tan(scope *env, value *v) {
  LASSERT_NUM("tan", v, 1);
  LASSERT_TYPE("tan", v, 0, NUMBER);

  value *res = new_num(tan(v->cell[0]->num));

  del_value(v);
  return res;
}

value *builtin_asin(scope *env, value *v) {
  LASSERT_NUM("asin", v, 1);
  LASSERT_TYPE("asin", v, 0, NUMBER);

  value *res = new_num(asin(v->cell[0]->num));

  del_value(v);
  return res;
}

value *builtin_acos(scope *env, value *v) {
  LASSERT_NUM("acos", v, 1);
  LASSERT_TYPE("acos", v, 0, NUMBER);

  value *res = new_num(acos(v->cell[0]->num));

  del_value(v);
  return res;
}

value *builtin_atan(scope *env, value *v) {
  LASSERT_NUM("atan", v, 1);
  LASSERT_TYPE("atan", v, 0, NUMBER);

  value *res = new_num(atan(v->cell[0]->num));

  del_value(v);
  return res;
}

value *builtin_strlen(scope *env, value *v) {
  LASSERT_NUM("str_len", v, 1);
  LASSERT_TYPE("str_len", v, 0, STRING);

  value *res = new_num(strlen(v->cell[0]->string));

  del_value(v);
  return res;
}

value *builtin_strcmp(scope *env, value *v) {
  LASSERT_NUM("str_cmp", v, 2);
  LASSERT_TYPE("str_cmp", v, 0, STRING);
  LASSERT_TYPE("str_cmp", v, 1, STRING);

  int res = strcmp(v->cell[0]->string, v->cell[1]->string);

  del_value(v);
  return res == 0 ? new_num(1) : new_num(0);
}

char *toUpper(char *string) {
  for (int i = 0; string[i] != '\0'; i++) {
    if ((string[i] >= 'a') && (string[i] <= 'z')) {
      string[i] = string[i] - 32;
    }
  }

  return string;
}

value *builtin_touppercase(scope *env, value *v) {
  LASSERT_NUM("to_upper", v, 1);
  LASSERT_TYPE("to_upper", v, 0, STRING);

  char str[4096];
  strcpy(str, v->cell[0]->string);

  value *res = new_string(toUpper(str));

  del_value(v);
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

value *builtin_tolowercase(scope *env, value *v) {
  LASSERT_NUM("to_lower", v, 1);
  LASSERT_TYPE("to_lower", v, 0, STRING);

  char str[4096];
  strcpy(str, v->cell[0]->string);

  value *res = new_string(toLower(str));

  del_value(v);
  return res;
}

value *builtin_rand(scope *env, value *a) {
  LASSERT_NUM("rand", a, 2);
  LASSERT_TYPE("rand", a, 0, NUMBER);
  LASSERT_TYPE("rand", a, 1, NUMBER);

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

  del_value(a);
  return new_num(res);
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

value *builtin_randstr(scope *env, value *a) {
  LASSERT_NUM("rand_str", a, 1);
  LASSERT_TYPE("rand_str", a, 0, NUMBER);

  unsigned int count = a->cell[0]->num;
  char *str = random_str(count);
  if (!str) {
    return new_err("Unexpected, internal error!!");
  }

  del_value(a);
  return new_string(str);
}

value *builtin_frand(scope *env, value *a) {
  LASSERT_NUM("frand", a, 1);
  LASSERT_TYPE("frand", a, 0, NUMBER);

  srand(time(NULL));
  int arg = a->cell[0]->num;

  if (arg <= 0) {
    return new_err("Wrong range passed!!");
  }

  double result = ((double)rand() / (double)RAND_MAX) + (arg - 1);

  return new_num(result);
}

value *builtin_concat(scope *sc, value *a) {
  long long len = 0;
  for (int i = 0; i < a->count; ++i) {
    if (a->cell[i]->type != STRING) {
      value *err =
          new_err("Expected string, got %s", type_name(a->cell[i]->type));
      return err;
    }
    len = len + strlen(a->cell[i]->string);
  }

  if (a->count == 0) {
    value *fst = new_string(a->cell[0]->string);
    return fst;
  }

  char res[len];

  for (int i = 0; i < a->count; ++i) {
    strcat(res, a->cell[i]->string);
  }

  value *result = new_string(res);
  del_value(a);
  return result;
}

#include "builtins.h"
#include "error.h"
#include "eval.h"
#include "mpc.h"
#include "parser.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static char tmp[2048];

#define CONCAT(string, fmt, ...)                                               \
  sprintf(tmp, fmt, ##__VA_ARGS__);                                            \
  strcat(string, tmp)

void wa_print(value *v, char *str);

static scope *global = NULL;

void wa_print_expr(value *v, char open, char close, char *str) {
  CONCAT(str, "%c", open);

  for (int i = 0; i < v->count; i++) {

    wa_print(v->cell[i], str);

    if (i != (v->count - 1)) {
      CONCAT(str, " ");
    }
  }
  CONCAT(str, "%c", close);
}

void wa_print_str(value *v, char *str) {
  char *tmp = malloc(strlen(v->string) + 1);
  strcpy(tmp, v->string);
  tmp = mpcf_escape(tmp);
  CONCAT(str, "\"%s\"", tmp);
  free(tmp);
}

void wa_print(value *v, char *str) {
  switch (v->type) {
  case NUMBER: {
    if (ifDouble(v->num) == 1) {
      CONCAT(str, "%Lf", v->num);
    } else {
      CONCAT(str, "%ld", (long)v->num);
    }
  } break;
  case FUNCTION: {
    if (v->func) {
      CONCAT(str, "<function>");
    } else {
      CONCAT(str, "(lambda");
      wa_print(v->formals, str);
      CONCAT(str, " ");
      wa_print(v->body, str);
      CONCAT(str, ")");
    }
  } break;
  case OK:
    break;
  case ERR:
    CONCAT(str, "Error: %s", v->error);
    break;
  case SYMBOL:
    CONCAT(str, "%s", v->symbol);
    break;
  case STRING:
    wa_print_str(v, str);
    break;
  case QEXPRESSION:
    wa_print_expr(v, '{', '}', str);
    break;
  case SEXPRESSION:
    wa_print_expr(v, '(', ')', str);
    break;
  }
}

char *wa_println(value *v) {
  if (v->type != OK) {
    char res[4096];
    wa_print(v, res);
    /* CONCAT(res, "\n"); */

    char *string = malloc(strlen(res) + 1);

    if (!string) {
      return NULL;
    } else {
      strcpy(string, res);
    }

    return string;
  }
  return NULL;
}

char *eval_wasm(char *code) {
  char *result = NULL;

  parser *p = parse();

  if (!global) {
    global = new_scope();
  }

  add_builtins(global);

  mpc_result_t r;

  if (mpc_parse("<stdin>", code, p->Yippy, &r)) {
    value *x = eval(global, read_expr(r.output));
    result = wa_println(x);
    del_value(x);
    mpc_ast_delete(r.output);
  } else {
    mpc_err_print(r.error);
    mpc_err_delete(r.error);
  }

  p = parse_clean(p);

  return result;
}

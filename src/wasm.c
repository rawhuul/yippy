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

void wa_print(value *v, char *str);

static scope *global = NULL;

void wa_print_expr(value *v, char open, char close, char *str) {
  sprintf(str + strlen(str), "%c", open);
  for (int i = 0; i < v->count; i++) {

    wa_print(v->cell[i], str);

    if (i != (v->count - 1)) {
      strcat(str, " ");
    }
  }
  sprintf(str + strlen(str), "%c", close);
}

void wa_print_str(value *v, char *str) {
  char *tmp = malloc(strlen(v->string) + 1);
  strcpy(tmp, v->string);
  tmp = mpcf_escape(tmp);
  sprintf(str + strlen(str), "\"%s\"", tmp);
  free(tmp);
}

void wa_print(value *v, char *str) {
  switch (v->type) {
  case NUMBER: {
    if (ifDouble(v->num) == 1) {
      sprintf(str + strlen(str), "%Lf", v->num);
    } else {
      sprintf(str + strlen(str), "%ld", (long)v->num);
    }
  } break;
  case FUNCTION: {
    if (v->func) {
      sprintf(str + strlen(str), "<function>");
    } else {
      strcat(str, "(lambda");
      wa_print(v->formals, str);
      strcat(str, " ");
      wa_print(v->body, str);
      strcat(str, ")");
    }
  } break;
  case OK:
    break;
  case ERR:
    sprintf(str + strlen(str), "Error: %s", v->error);
    break;
  case SYMBOL:
    sprintf(str + strlen(str), "%s", v->symbol);
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
    char *res = malloc(4096);
    wa_print(v, res);
    strcat(res, "\n");
    return res;
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

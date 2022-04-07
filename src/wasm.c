#include "builtins.h"
#include "error.h"
#include "eval.h"
#include "mpc.h"
#include "parser.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static scope *global = NULL;

void eval_wasm(char *code) {
  parser *p = parse();

  if (!global) {
    global = new_scope();
  }

  add_builtins(global);

  mpc_result_t r;

  if (mpc_parse("<stdin>", code, p->Yippy, &r)) {
    value *x = eval(global, read_expr(r.output));
    del_value(x);
    mpc_ast_delete(r.output);
  } else {
    mpc_err_print(r.error);
    mpc_err_delete(r.error);
  }

  p = parse_clean(p);
}

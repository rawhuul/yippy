#include "builtins.h"
#include "eval.h"
#include "mpc.h"
#include "parser.h"
#include "types.h"

static scope *global = NULL;

void eval_wasm(char *code, scope *env) {
  parser *p = parse();
  add_builtins(env);

  mpc_result_t r;

  if (mpc_parse("<stdin>", code, p->Yippy, &r)) {
    value *x = eval(env, read_expr(r.output));
    println(x);
    del_value(x);
    mpc_ast_delete(r.output);
  } else {
    mpc_err_print(r.error);
    mpc_err_delete(r.error);
  }

  global = env;
  del_scope(env);
  p = parse_clean(p);
}

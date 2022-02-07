#include "builtins.h"
#include <stdlib.h>

lval *builtin_load(lenv *env, lval *a, mpc_parser_t *yippy) {
  LASSERT_NUM("load", a, 1);
  LASSERT_TYPE("load", a, 0, LVAL_STR);

  mpc_result_t r;
  if (mpc_parse_contents(a->cell[0]->string, yippy, &r)) {

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

    return lval_sexpr();
  } else {
    char *error_msg = mpc_err_string(r.error);
    mpc_err_delete(r.error);
    lval *err = lval_err("Could not load library %s", error_msg);
    free(error_msg);
    lval_del(a);

    return err;
  }
}

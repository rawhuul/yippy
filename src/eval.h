#ifndef YIPPY_EVAL
#define YIPPY_EVAL

#include "mpc.h"
#include "types.h"

lval *lval_read(mpc_ast_t *t);
lval *lval_eval(lenv *e, lval *v);
void lval_println(lval *v);
void lval_del(lval *v);

lenv *lenv_new(void);
void lenv_add_builtins(lenv *env);
lenv *lenv_del(lenv *e);

#endif

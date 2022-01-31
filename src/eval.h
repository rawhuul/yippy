#ifndef YIPPY_EVAL
#define YIPPY_EVAL

#include "mpc.h"
#include "types.h"

void lval_print(lval *v);
void lval_println(lval *v);
lval *lval_eval(lenv *e, lval *v);
void lval_del(lval *v);
lval *builtin(lenv *e, lval *a, char *func);
lval *lval_read(mpc_ast_t *t);
lenv *lenv_new(void);
lenv *lenv_del(lenv *e);
void lenv_add_builtins(lenv *env);
#endif

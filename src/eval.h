#ifndef YIPPY_EVAL
#define YIPPY_EVAL

#include "mpc.h"
#include "types.h"

lval *lval_read(mpc_ast_t *t);
lval *lval_eval(lenv *e, lval *v);
void lval_println(lval *v);
void lval_print(lval *v);
void lval_del(lval *v);
lval *lval_sexpr(void);
lval *lval_pop(lval *v, int i);
lval *lval_add(lval *v, lval *x);
lval *lval_str(char *str);
lval *lval_pop(lval *v, int i);
lval *lval_take(lval *v, int i);
lval *lval_join(lval *x, lval *y);

lenv *lenv_new(void);
void lenv_add_builtins(lenv *env);
void lenv_del(lenv *e);
void lenv_put(lenv *env, lval *key, lval *val);
void lenv_def_global(lenv *env, lval *k, lval *v);

#endif

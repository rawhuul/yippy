#ifndef YIPPY_EVAL
#define YIPPY_EVAL

#include "mpc.h"

typedef enum {
  LVAL_NUM,
  LVAL_SYM,
  LVAL_FUNC,
  LVAL_SEXP,
  LVAL_QEXP,
  LVAL_ERR,
} ltype;

typedef struct lval lval;

typedef struct lenv lenv;

typedef lval *(*lbuiltin)(lenv *, lval *);

void lval_print(lval *v);
void lval_println(lval *v);
lval *lval_eval(lval *v);
void lval_del(lval *v);
lval *builtin(lval *a, char *func);
lval *lval_read(mpc_ast_t *t);
#endif

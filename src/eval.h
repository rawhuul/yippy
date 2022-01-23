#ifndef YIPPY_EVAL
#define YIPPY_EVAL

#include "mpc.h"

typedef enum {
  LVAL_NUM,
  LVAL_SYM,
  LVAL_SEXP,
  LVAL_ERR,
} ltype;

typedef struct lval {
  ltype type;
  long num;
  char *error;
  char *symbol;
  struct lval **cell;
  unsigned int count;
} lval;

void lval_print(lval *v);
void lval_println(lval *v);
lval *lval_eval(lval *v);
void lval_del(lval *v);
lval *lval_read(mpc_ast_t *t);
#endif

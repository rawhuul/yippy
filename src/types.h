#ifndef YIPPY_TYPES
#define YIPPY_TYPES

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


struct lval {
  ltype type;

  long num;
  char *error;
  char *symbol;
  lbuiltin func;

  struct lval **cell;
  unsigned int count;
};

struct lenv {
  int count;
  char **syms;
  lval **vals;
};

char *type_name(ltype t);

#endif

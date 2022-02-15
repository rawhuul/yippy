#ifndef YIPPY_TYPES
#define YIPPY_TYPES

typedef enum {
  LVAL_NUM,
  LVAL_SYM,
  LVAL_STR,
  LVAL_FUNC,
  LVAL_SEXP,
  LVAL_QEXP,
  /* Straight from Rust */
  LVAL_OK,
  LVAL_ERR,
} ltype;

typedef struct lval lval;

typedef struct lenv lenv;

typedef lval *(*lbuiltin)(lenv *, lval *);

struct lval {
  ltype type;

  double num;
  char *error;
  char *symbol;
  char *string;

  lbuiltin func;
  lenv *env;
  lval *formals;
  lval *body;

  lval **cell;
  unsigned int count;
};

struct lenv {
  lenv *parent;
  int count;
  char **syms;
  lval **vals;
};

char *type_name(ltype t);

lval *lval_num(double x);
lval *lval_str(char *str);
lval *lval_sym(char *s);
lval *lval_sexpr(void);
lval *lval_func(lbuiltin func);
lval *lval_lambda(lval *formals, lval *body);
lval *lval_qexpr(void);
void lval_del(lval *v);
lval *lval_add(lval *v, lval *x);
lval *lval_copy(lval *v);

lenv *lenv_new(void);
void lenv_del(lenv *e);
lenv *lenv_copy(lenv *e);

int ifDouble(double a);

#endif

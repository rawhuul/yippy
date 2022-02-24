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
} kind;

typedef struct value value;

typedef struct context context;

typedef value *(*lbuiltin)(context *, value *);

struct value {
  kind type;

  long double num;
  char *error;
  char *symbol;
  char *string;

  lbuiltin func;
  context *env;
  value *formals;
  value *body;

  value **cell;
  unsigned int count;
};

struct context {
  context *parent;
  int count;
  char **syms;
  value **vals;
};

char *type_name(kind t);

value *value_num(double x);
value *value_str(char *str);
value *value_sym(char *s);
value *value_sexpr(void);
value *value_func(lbuiltin func);
value *value_lambda(value *formals, value *body);
value *value_qexpr(void);
void value_del(value *v);
value *value_add(value *v, value *x);
value *value_copy(value *v);

context *context_new(void);
void context_del(context *e);
context *context_copy(context *e);

int ifDouble(double a);

#endif

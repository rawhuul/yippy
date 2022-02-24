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

typedef struct scope scope;

typedef value *(*function)(scope *, value *);

struct value {
  kind type;

  long double num;
  char *error;
  char *symbol;
  char *string;

  function func;
  scope *env;
  value *formals;
  value *body;

  value **cell;
  unsigned int count;
};

struct scope {
  scope *parent;
  int count;
  char **syms;
  value **vals;
};

char *type_name(kind t);

value *new_num(double x);
value *new_string(char *str);
value *new_symbol(char *s);
value *new_sexp(void);
value *new_func(function func);
value *new_lambda(value *formals, value *body);
value *new_qexp(void);
void del_value(value *v);
value *add_value(value *v, value *x);
value *copy_value(value *v);

scope *new_scope(void);
void del_scope(scope *e);
scope *copy_scope(scope *e);

int ifDouble(double a);

#endif

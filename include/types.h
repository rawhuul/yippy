#ifndef YIPPY_TYPES
#define YIPPY_TYPES

typedef enum {
  NUMBER,
  SYMBOL,
  STRING,
  FUNCTION,
  SEXPRESSION,
  QEXPRESSION,
  /* Straight from Rust */
  OK,
  ERR,
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
int ifDouble(double a);

value *new_num(double x);
value *new_string(char *str);
value *new_symbol(char *s);
value *new_sexp(void);
value *new_func(function func);
value *new_lambda(value *formals, value *body);
value *new_qexp(void);
value *ok(void);

value *add_value(value *v, value *x);
value *copy_value(value *v);
void del_value(value *v);

scope *new_scope(void);
scope *copy_scope(scope *e);
void del_scope(scope *e);

#endif

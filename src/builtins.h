#ifndef YIPPY_BUILTINS
#define YIPPY_BUILTINS

#include "error.h"
#include "eval.h"
#include "types.h"

/* Function Registrar */
lval *builtin(lenv *e, lval *a, char *func);

/* List Operaions */
lval *builtin_head(lenv *e, lval *a);
lval *builtin_tail(lenv *e, lval *a);
lval *builtin_list(lenv *e, lval *a);
lval *builtin_eval(lenv *e, lval *a);
lval *builtin_join(lenv *e, lval *a);

/* Arithmetic Functions */
lval *builtin_add(lenv *env, lval *a);
lval *builtin_minus(lenv *env, lval *a);
lval *builtin_div(lenv *env, lval *a);
lval *builtin_product(lenv *env, lval *a);
lval *builtin_op(lenv *env, lval *a, char *op);

/* Comparison Operator */
lval *builtin_gt(lenv *env, lval *a);
lval *builtin_gte(lenv *env, lval *a);
lval *builtin_lt(lenv *env, lval *a);
lval *builtin_lte(lenv *env, lval *a);
lval *builtin_cmp(lenv *env, lval *a, char *operator);

/* Equality Operator */
int lval_eq(lval *x, lval *y);
lval *builtin_eq(lenv *env, lval *a);
lval *builtin_neq(lenv *env, lval *a);
lval *builtin_equality(lenv *env, lval *a, char *operator);

/* IF operator */
lval *builtin_if(lenv *env, lval *a);

/* Variable and Functions */
lval *builtin_let(lenv *env, lval *val, char *scope);
lval *builtin_lambda(lenv *env, lval *a);

#endif
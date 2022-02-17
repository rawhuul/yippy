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
lval *builtin_modulus(lenv *env, lval *a);
lval *builtin_not(lenv *env, lval *a);
lval *builtin_negate(lenv *env, lval *a);
lval *builtin_op(lenv *env, lval *a, char *op);

/* Logical Operators */
lval *builtin_bin_xor(lenv *env, lval *a);
lval *builtin_bin_and(lenv *env, lval *a);
lval *builtin_bin_or(lenv *env, lval *a);
lval *builtin_log_and(lenv *env, lval *a);
lval *builtin_log_or(lenv *env, lval *a);
lval *builtin_rshift(lenv *env, lval *a);
lval *builtin_lshift(lenv *env, lval *a);
lval *builtin_logical_op(lenv *env, lval *a, char *operator);

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

lval *builtin_loop(lenv *env, lval *a);

lval *builtin_exit(lenv *env, lval *a);

/* Variable and Functions */
lval *builtin_let(lenv *env, lval *val, char *scope);
lval *builtin_lambda(lenv *env, lval *a);

lval *builtin_global(lenv *env, lval *val);
lval *builtin_local(lenv *env, lval *val);

/* Load function */
lval *builtin_load(lenv *env, lval *a);
lval *builtin_print(lenv *env, lval *a);
lval *builtin_error(lenv *env, lval *a);

/* Mathematical Functions */
lval *builtin_pow(lenv *env, lval *v);
lval *builtin_sqrt(lenv *env, lval *v);

lval *builtin_sin(lenv *env, lval *v);
lval *builtin_cos(lenv *env, lval *v);
lval *builtin_tan(lenv *env, lval *v);

lval *builtin_asin(lenv *env, lval *v);
lval *builtin_acos(lenv *env, lval *v);
lval *builtin_atan(lenv *env, lval *v);

/* String Functions */
lval *builtin_strlen(lenv *env, lval *v);
#endif

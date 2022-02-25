#ifndef YIPPY_BUILTINS
#define YIPPY_BUILTINS

#include "error.h"
#include "eval.h"
#include "types.h"

/* Function Registrar */
value *builtin(scope *e, value *a, char *func);

/* List Operaions */
value *builtin_head(scope *e, value *a);
value *builtin_tail(scope *e, value *a);
value *builtin_list(scope *e, value *a);
value *builtin_eval(scope *e, value *a);
value *builtin_join(scope *e, value *a);
value *builtin_range(scope *e, value *v);

/* Arithmetic Functions */
value *builtin_add(scope *env, value *a);
value *builtin_minus(scope *env, value *a);
value *builtin_div(scope *env, value *a);
value *builtin_product(scope *env, value *a);
value *builtin_modulus(scope *env, value *a);
value *builtin_not(scope *env, value *a);
value *builtin_negate(scope *env, value *a);
value *builtin_op(scope *env, value *a, char *op);

/* Logical Operators */
value *builtin_bin_xor(scope *env, value *a);
value *builtin_bin_and(scope *env, value *a);
value *builtin_bin_or(scope *env, value *a);
value *builtin_log_and(scope *env, value *a);
value *builtin_log_or(scope *env, value *a);
value *builtin_rshift(scope *env, value *a);
value *builtin_lshift(scope *env, value *a);
value *builtin_logical_op(scope *env, value *a, char *operator);

/* Comparison Operator */
value *builtin_gt(scope *env, value *a);
value *builtin_gte(scope *env, value *a);
value *builtin_lt(scope *env, value *a);
value *builtin_lte(scope *env, value *a);
value *builtin_cmp(scope *env, value *a, char *operator);

/* Equality Operator */
int value_eq(value *x, value *y);
value *builtin_eq(scope *env, value *a);
value *builtin_neq(scope *env, value *a);
value *builtin_equality(scope *env, value *a, char *operator);

/* IF operator */
value *builtin_if(scope *env, value *a);

value *builtin_loop(scope *env, value *a);

value *builtin_exit(scope *env, value *a);

/* Variable and Functions */
value *builtin_let(scope *env, value *val, char *scope);
value *builtin_lambda(scope *env, value *a);

value *builtin_global(scope *env, value *val);
value *builtin_local(scope *env, value *val);

/* Load function */
value *builtin_load(scope *env, value *a);
value *builtin_print(scope *env, value *a);
value *builtin_error(scope *env, value *a);

/* Mathematical Functions */
value *builtin_pow(scope *env, value *v);
value *builtin_sqrt(scope *env, value *v);

value *builtin_sin(scope *env, value *v);
value *builtin_cos(scope *env, value *v);
value *builtin_tan(scope *env, value *v);

value *builtin_asin(scope *env, value *v);
value *builtin_acos(scope *env, value *v);
value *builtin_atan(scope *env, value *v);

/* String Functions */
value *builtin_strlen(scope *env, value *v);
value *builtin_strcmp(scope *env, value *v);
value *builtin_touppercase(scope *env, value *v);
value *builtin_tolowercase(scope *env, value *v);

/* Random Number Functions */
value *builtin_rand(scope *env, value *a);
value *builtin_randstr(scope *env, value *a);
value *builtin_frand(scope *env, value *a);

#endif

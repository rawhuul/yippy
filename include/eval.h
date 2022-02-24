#ifndef YIPPY_EVAL
#define YIPPY_EVAL

#include "mpc.h"
#include "types.h"

value *read(mpc_ast_t *t);
value *eval(scope *e, value *v);

void print(value *v);
void println(value *v);

value *pop(value *v, int i);
value *take(value *v, int i);
value *join(value *x, value *y);

void put(scope *env, value *key, value *val);

void add_builtins(scope *sc);
void put_scope(scope *env, value *key, value *val);
void global_scope(scope *sc, value *k, value *v);

#endif

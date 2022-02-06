#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

lval *lval_err(char *fmt, ...) {
  lval *v = (lval *)malloc(sizeof(lval));
  v->type = LVAL_ERR;

  va_list va;
  va_start(va, fmt);

  v->error = malloc(512);

  vsnprintf(v->error, 512 - 1, fmt, va);

  v->error = realloc(v->error, strlen(v->error) + 1);

  va_end(va);

  return v;
}

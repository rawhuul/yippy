#include "error.h"
#include <gc.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

value *new_err(char *fmt, ...) {
  value *v = (value *)GC_malloc(sizeof(value));
  v->type = ERR;

  va_list va;
  va_start(va, fmt);

  v->error = GC_malloc(512);

  vsnprintf(v->error, 512 - 1, fmt, va);

  v->error = GC_realloc(v->error, strlen(v->error) + 1);

  va_end(va);

  return v;
}

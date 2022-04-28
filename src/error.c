#include "error.h"
#include "mem.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

value *new_err(char *fmt, ...) {
  value *v = (value *)malloc(sizeof(value));
  v->type = ERR;

  va_list va;
  va_start(va, fmt);

  v->error = malloc(512);

  vsnprintf(v->error, 512 - 1, fmt, va);

  v->error = realloc(v->error, strlen(v->error) + 1);

  va_end(va);

  return v;
}

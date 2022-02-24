#ifndef YIPPY_ERROR
#define YIPPY_ERROR

#include "types.h"

#define EPRINT(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)

#define LASSERT(args, cond, fmt, ...)                                          \
  if (!(cond)) {                                                               \
    value *err = new_err(fmt, ##__VA_ARGS__);                                  \
    del_value(args);                                                           \
    return err;                                                                \
  }

#define LASSERT_TYPE(func, args, index, expect)                                \
  LASSERT(args, args->cell[index]->type == expect,                             \
          "Function '%s' passed incorrect type for argument %i. Got %s, "      \
          "Expected %s.",                                                      \
          func, index, type_name(args->cell[index]->type), type_name(expect))

#define LASSERT_NUM(func, args, num)                                           \
  LASSERT(args, args->count == num,                                            \
          "Function '%s' passed incorrect number of arguments. Got %i, "       \
          "Expected %i.",                                                      \
          func, args->count, num)

#define LASSERT_NOT_EMPTY(func, args, index)                                   \
  LASSERT(args, args->cell[index]->count != 0,                                 \
          "Function '%s' passed {} for argument %i.", func, index);

value *new_err(char *fmt, ...);

#endif

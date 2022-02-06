#include "types.h"

char *type_name(ltype t) {
  switch (t) {
  case LVAL_FUNC:
    return "Function";
  case LVAL_NUM:
    return "Number";
  case LVAL_QEXP:
    return "Q-expression";
  case LVAL_SEXP:
    return "S-expression";
  case LVAL_STR:
    return "String";
  case LVAL_SYM:
    return "Symbol";
  case LVAL_ERR:
    return "Error";
  default:
    return "Unknown";
  }
}

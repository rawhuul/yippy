#ifndef YIPPY_PARSER
#define YIPPY_PARSER

#include "mpc.h"

typedef struct parser {
  mpc_parser_t *Number;
  mpc_parser_t *Symbol;
  mpc_parser_t *Sexpr;
  mpc_parser_t *Qexpr;
  mpc_parser_t *Expr;
  mpc_parser_t *Yippy;
} parser;

parser *parse();
parser *parse_clean(parser *to_free);

#endif

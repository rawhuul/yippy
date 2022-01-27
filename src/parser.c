#include "parser.h"
#include "mpc.h"
#include <stdio.h>
#include <stdlib.h>

static char grammer[] = "                                          \
    number : /-?[0-9]+/ ;						\
    symbol : /[a-zA-Z0-9_+\\-*%&|\\/\\\\=<>!]+/;			\
    sexpr  : '(' <expr>* ')' ;						\
    qexpr  : '{' <expr>* '}' ;						\
    expr   : <number> | <symbol> | <qexpr> | <sexpr> ;			\
    yippy  : /^/ <expr>* /$/ ;						\
  ";

mpc_parser_t *Number;
mpc_parser_t *Symbol;
mpc_parser_t *Sexpr;
mpc_parser_t *Qexpr;
mpc_parser_t *Expr;
mpc_parser_t *Yippy;

parsed *parse() {
  Number = mpc_new("number");
  Symbol = mpc_new("symbol");
  Sexpr = mpc_new("sexpr");
  Qexpr = mpc_new("qexpr");
  Expr = mpc_new("expr");
  Yippy = mpc_new("yippy");

  mpca_lang(MPCA_LANG_DEFAULT, grammer, Number, Symbol, Sexpr, Qexpr, Expr,
            Yippy);

  return Yippy;
}

void parse_clean(void) {
  mpc_cleanup(6, Number, Symbol, Sexpr, Qexpr, Expr, Yippy);
}

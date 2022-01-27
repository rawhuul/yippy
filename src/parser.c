#include "parser.h"
#include "mpc.h"
#include <stdio.h>

static char grammer[] = "                                          \
    number : /-?[0-9]+/ ;						\
    symbol : \"head\" | \"list\" | \"join\" | \"tail\" |		\
\"eval\" | '+' | '-' | '*' | '/' | '%' | '&' | '|';			\
    sexpr  : '(' <expr>* ')' ;						\
    qexpr  : '{' <expr>* '}' ;						\
    expr   : <number> | <symbol> | <qexpr> | <sexpr> ;			\
    yippy  : /^/ <expr>* /$/ ;						\
  ";

parsed *parse() {
  mpc_parser_t *Number = mpc_new("number");
  mpc_parser_t *Symbol = mpc_new("symbol");
  mpc_parser_t *Sexpr = mpc_new("sexpr");
  mpc_parser_t *Qexpr = mpc_new("qexpr");
  mpc_parser_t *Expr = mpc_new("expr");
  mpc_parser_t *Yippy = mpc_new("yippy");

  mpca_lang(MPCA_LANG_DEFAULT, grammer, Number, Symbol, Sexpr, Qexpr, Expr,
            Yippy);

  return Yippy;
}

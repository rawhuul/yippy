#include "parser.h"
#include "mem.h"
#include <stdio.h>

#define GRAMMER                                                                \
  "                                          \
    comments : /;[^\\r\\n]*/;						\
    number : /-?[0-9]+(\\.[0-9]*)?/;					\
    symbol : /[a-zA-Z0-9_+\\-*%&|\\/\\\\=<>!~^\"]+/;			\
    string : /\"(\\\\.|[^\"])*\"/;					\
    sexpr  : '(' <expr>* ')';						\
    qexpr  : '{' <expr>* '}';						\
    expr   : <comments> | <number> | <string>				\
             | <symbol> | <qexpr> | <sexpr>;				\
    yippy  : /^/ <expr>* /$/;						\
  "

parser *parse() {
  parser *to_parse = (parser *)malloc(sizeof(parser));
  to_parse->Comments = mpc_new("comments");
  to_parse->Number = mpc_new("number");
  to_parse->String = mpc_new("string");
  to_parse->Symbol = mpc_new("symbol");
  to_parse->Sexpr = mpc_new("sexpr");
  to_parse->Qexpr = mpc_new("qexpr");
  to_parse->Expr = mpc_new("expr");
  to_parse->Yippy = mpc_new("yippy");

  mpca_lang(MPCA_LANG_DEFAULT, GRAMMER, to_parse->Comments, to_parse->Number,
            to_parse->String, to_parse->Symbol, to_parse->Sexpr,
            to_parse->Qexpr, to_parse->Expr, to_parse->Yippy);

  return to_parse;
}

parser *parse_clean(parser *to_free) {
  mpc_cleanup(8, to_free->Comments, to_free->Number, to_free->String,
              to_free->Symbol, to_free->Sexpr, to_free->Qexpr, to_free->Expr,
              to_free->Yippy);
  to_free = NULL;
  return to_free;
}

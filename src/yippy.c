#include "eval.h"
#include "linenoise.h"
#include "mpc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HIST_FILE ".yippie_hsts"
#define YIPPY_PROMPT ">>> "

int main(void) {
  char *input;
  linenoiseHistoryLoad(HIST_FILE);

  while (1) {
    input = linenoise(YIPPY_PROMPT);

    if (!input) {
      printf("BYE!\n");
      break;
    }

    if (!strlen(input)) {
      continue;
    }

    mpc_parser_t *Number = mpc_new("number");
    mpc_parser_t *Symbol = mpc_new("symbol");
    mpc_parser_t *Sexpr = mpc_new("sexpr");
    mpc_parser_t *Qexpr = mpc_new("qexpr");
    mpc_parser_t *Expr = mpc_new("expr");
    mpc_parser_t *Lispy = mpc_new("lispy");

    mpca_lang(MPCA_LANG_DEFAULT, "                                          \
    number : /-?[0-9]+/ ;						\
    symbol : \"head\" | \"list\" | \"join\" | \"tail\" |		\
\"eval\" | '+' | '-' | '*' | '/' | '%' | '&' | '|';			\
    sexpr  : '(' <expr>* ')' ;						\
    qexpr  : '{' <expr>* '}' ;						\
    expr   : <number> | <symbol> | <qexpr> | <sexpr> ;			\
    lispy  : /^/ <expr>* /$/ ;						\
  ",
              Number, Symbol, Sexpr, Qexpr, Expr, Lispy);

    mpc_result_t *r = (mpc_result_t *)malloc(sizeof(mpc_result_t));
    if (mpc_parse("<stdin>", input, Lispy, r)) {
      lval *x = lval_eval(lval_read(r->output));
      lval_println(x);
      lval_del(x);
      mpc_ast_delete(r->output);
    } else {
      mpc_err_print(r->error);
      mpc_err_delete(r->error);
    }

    linenoiseHistoryAdd(input);

    mpc_cleanup(6, Number, Symbol, Sexpr, Qexpr, Expr, Lispy);

    free(r);
    linenoiseFree(input);
  }

  linenoiseHistorySave(HIST_FILE);
  return 0;
}

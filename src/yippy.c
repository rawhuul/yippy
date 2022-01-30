#include "eval.h"
#include "linenoise.h"
#include "mpc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FREE(x)                                                                \
  if (x != NULL) {                                                             \
    free(x);                                                                   \
    x = NULL;                                                                  \
  }

#define HIST_FILE ".yippy_hsts"
#define YIPPY_PROMPT ">>> "

#define GRAMMER                                                                \
  "                                          \
    number : /-?[0-9]+/ ;						\
    symbol : /[a-zA-Z0-9_+\\-*%&|\\/\\\\=<>!~\"]+/;			\
    sexpr  : '(' <expr>* ')' ;						\
    qexpr  : '{' <expr>* '}' ;						\
    expr   : <number> | <symbol> | <qexpr> | <sexpr> ;			\
    yippy  : /^/ <expr>* /$/ ;						\
  "

int main(void) {
  char *input;
  linenoiseHistoryLoad(HIST_FILE);
  lenv *env = lenv_new();
  lenv_add_builtins(env);

  mpc_parser_t *Number = mpc_new("number");
  mpc_parser_t *Symbol = mpc_new("symbol");
  mpc_parser_t *Sexpr = mpc_new("sexpr");
  mpc_parser_t *Qexpr = mpc_new("qexpr");
  mpc_parser_t *Expr = mpc_new("expr");
  mpc_parser_t *Yippy = mpc_new("yippy");

  mpca_lang(MPCA_LANG_DEFAULT, GRAMMER, Number, Symbol, Sexpr, Qexpr, Expr,
            Yippy);

  while (1) {
    input = linenoise(YIPPY_PROMPT);

    if (!input) {
      linenoiseFree(input);
      printf("BYE!!!\n");
      break;
    }

    if (!strlen(input)) {
      linenoiseFree(input);
      continue;
    }

    linenoiseHistoryAdd(input);
    linenoiseHistorySave(HIST_FILE);

    mpc_result_t r;
    if (mpc_parse("<stdin>", input, Yippy, &r)) {
      lval *x = lval_eval(env, lval_read(r.output));
      lval_println(x);
      lval_del(x);
      mpc_ast_delete(r.output);
    } else {
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }

    linenoiseFree(input);
  }

  mpc_cleanup(6, Number, Symbol, Sexpr, Qexpr, Expr, Yippy);
  lenv_del(env);

  return 0;
}

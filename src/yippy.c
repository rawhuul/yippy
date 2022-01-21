#include "linenoise.h"
#include "mpc.h"
#include <stdio.h>
#include <stdlib.h>

#define HIST_FILE ".yippie_hsts"
#define YIPPY_PROMPT ">>> "

int main(void) {
  char *input;
  linenoiseHistoryLoad(HIST_FILE);

  while (1) {
    input = linenoise(YIPPY_PROMPT);

    if (!input) {
      printf("Internal Error!\n");
      break;
    }

    printf("%s\n", input);

    mpc_parser_t *Number = mpc_new("number");
    mpc_parser_t *Operator = mpc_new("operator");
    mpc_parser_t *Expr = mpc_new("expr");
    mpc_parser_t *Lispy = mpc_new("lispy");

    mpca_lang(MPCA_LANG_DEFAULT,
              "                                                     \
    number   : /-?[0-9]+/ ;                             \
    operator : '+' | '-' | '*' | '/' ;                  \
    expr     : <number> | '(' <operator> <expr>+ ')' ;  \
    lispy    : /^/ <operator> <expr>+ /$/ ;             \
  ",
              Number, Operator, Expr, Lispy);

    mpc_result_t *r = (mpc_result_t *)malloc(sizeof(mpc_result_t));
    if (mpc_parse("<stdin>", input, Lispy, r)) {
      /* On Success Print the AST */
      mpc_ast_print(r->output);
      mpc_ast_delete(r->output);
    } else {
      /* Otherwise Print the Error */
      mpc_err_print(r->error);
      mpc_err_delete(r->error);
    }

    linenoiseHistoryAdd(input);

    linenoiseFree(input);
  }

  linenoiseHistorySave(HIST_FILE);
  return 0;
}

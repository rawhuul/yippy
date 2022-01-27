#include "eval.h"
#include "linenoise.h"
#include "mpc.h"
#include "parser.h"
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
      printf("BYE!\n");
      break;
    }

    if (!strlen(input)) {
      continue;
    }

    mpc_result_t *r = (mpc_result_t *)malloc(sizeof(mpc_result_t));
    if (mpc_parse("<stdin>", input, parse(), r)) {
      lval *x = lval_eval(lval_read(r->output));
      lval_println(x);
      lval_del(x);
      mpc_ast_delete(r->output);
    } else {
      mpc_err_print(r->error);
      mpc_err_delete(r->error);
    }

    linenoiseHistoryAdd(input);

    free(r);
    linenoiseFree(input);
  }

  linenoiseHistorySave(HIST_FILE);
  return 0;
}

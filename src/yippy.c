#include "eval.h"
#include "mpc.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _WIN32
#include "linenoise.h"
#endif
#include "builtins.h"
#include "yippy.h"

char *line(char *prompt) {
  char buffer[4096];
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char *input = malloc(strlen(buffer) + 1);
  strcpy(input, buffer);
  input[strlen(input) - 1] = '\0';
  return input;
}

void eval() {
  printf("Welcome to %s v%s\n", PROG_NAME, VERSION);
  char *input;
#ifndef _WIN32
  linenoiseHistoryLoad(HIST_FILE);
#endif
  lenv *env = lenv_new();
  lenv_add_builtins(env);

  parser *p = parse();

  while (1) {
    input = GET_INPUT(YIPPY_PROMPT);

    if (!input) {
      free(input);
      printf("BYE!!!\n");
      break;
    }

    if (!strlen(input)) {
      continue;
    }

#ifndef _WIN32
    linenoiseHistoryAdd(input);
    linenoiseHistorySave(HIST_FILE);
#endif

    mpc_result_t r;
    if (mpc_parse("<stdin>", input, p->Yippy, &r)) {
      lval *x = lval_eval(env, lval_read(r.output));
      lval_println(x);
      lval_del(x);
      mpc_ast_delete(r.output);
    } else {
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }

    free(input);
  }

  p = parse_clean(p);
  lenv_del(env);
}

void eval_file(int argc, char **argv) {
  parser *p = parse();
  lenv *env = lenv_new();
  lenv_add_builtins(env);
  for (int i = 2; i < argc; ++i) {
    lval *args = lval_add(lval_sexpr(), lval_str(argv[i]));
    lval *x = builtin_load(env, args, p->Yippy);

    if (x->type == LVAL_ERR) {
      lval_println(x);
    }
    lval_println(x);
    lval_del(x);
  }
  lenv_del(env);
  p = parse_clean(p);
}

void eval_inline(char *code) {
  parser *p = parse();
  lenv *env = lenv_new();
  lenv_add_builtins(env);

  mpc_result_t r;

  if (mpc_parse("<stdin>", code, p->Yippy, &r)) {
    lval *x = lval_eval(env, lval_read(r.output));
    lval_println(x);
    lval_del(x);
    mpc_ast_delete(r.output);
  } else {
    mpc_err_print(r.error);
    mpc_err_delete(r.error);
  }

  lenv_del(env);

  p = parse_clean(p);
}

#include "eval.h"
#ifndef _WIN32
#include "linenoise.h"
#endif
#include "mpc.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROG_NAME "yippy"
#define VERSION "0.0.1"
#define URL "https://github.com/basicfunc/yippy"

#define AUTHOR "Rahul"
#define LICENSE "MIT"

#define HIST_FILE ".yippy_hsts"
#define YIPPY_PROMPT ">>> "

#define WELCOME_MSG(a, b, c, d) a##"Welcome to"##b##"v"##c##d##"\n"

#ifdef _WIN32
#define GET_INPUT(x) line(x)
#else
#define GET_INPUT(x) linenoise(x)
#endif

char *line(char *prompt) {
  char buffer[4096];
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char *input = malloc(strlen(buffer) + 1);
  strcpy(input, buffer);
  input[strlen(input) - 1] = '\0';
  return input;
}

int main(void) {
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

  return 0;
}

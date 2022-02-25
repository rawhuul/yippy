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
#include <dirent.h>
#include <unistd.h>

typedef enum { CURRENT_DIR, SYSTEM_DIR, NOT_FOUND } where;

char *line(char *prompt) {
  char buffer[4096];
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char *input = malloc(strlen(buffer) + 1);
  strcpy(input, buffer);
  input[strlen(input) - 1] = '\0';
  return input;
}

int ifstdlib() {
  if (!access("/usr/lib/yippy/stdlib/", R_OK)) {
    return SYSTEM_DIR;
  } else if (!access("./stdlib/", R_OK)) {
    return CURRENT_DIR;
  } else {
    return NOT_FOUND;
  }
}

void fileinstdlib(scope *env, where here) {
  char fullpath[4096];
  DIR *d;
  char directory[4096];
  struct dirent *dir;
  if (here == CURRENT_DIR) {
    strcpy(directory, "stdlib/");
  } else if (here == SYSTEM_DIR) {
    strcpy(directory, "/usr/lib/yippy/stdlib/");
  } else {
    return;
  }
  d = opendir(directory);
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      chdir(directory);
      realpath(dir->d_name, fullpath);
      value *args = add_value(new_sexp(), new_string(fullpath));
      value *x = builtin_load(env, args);
      del_value(x);
    }
    closedir(d);
    chdir("../");
  }
}

const char *get_extension(const char *filename) {
  const char *dot = strrchr(filename, '.');
  if (!dot || dot == filename)
    return "";
  return dot + 1;
}

void eval_line() {
  printf("Welcome to %s v%0.1f\n", PROG_NAME, VERSION);

  char *input;

#ifndef _WIN32
  linenoiseHistoryLoad(HIST_FILE);
#endif

  scope *env = new_scope();
  add_builtins(env);

  int stdlib = ifstdlib();

  if (stdlib != NOT_FOUND) {
    fileinstdlib(env, stdlib);
  } else {
    puts("Won't able to find stblib, Please check your confiuration.");
  }

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
      value *x = eval(env, read_expr(r.output));
      println(x);
      del_value(x);
      mpc_ast_delete(r.output);
    } else {
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }

    free(input);
  }

  p = parse_clean(p);
  del_scope(env);
}

void eval_file(char *file) {
  scope *env = new_scope();
  add_builtins(env);

  if (fopen(file, "r") == NULL) {
    fprintf(stderr, "[Error]: \"%s\" file not found.\n", file);
    exit(-1);
  }

  value *args = add_value(new_sexp(), new_string(file));
  value *x = builtin_load(env, args);

  if (x->type == ERR) {
    println(x);
  }
  del_value(x);

  del_scope(env);
}

void eval_inline(char *code) {
  parser *p = parse();
  scope *env = new_scope();
  add_builtins(env);

  mpc_result_t r;

  if (mpc_parse("<stdin>", code, p->Yippy, &r)) {
    value *x = eval(env, read_expr(r.output));
    println(x);
    del_value(x);
    mpc_ast_delete(r.output);
  } else {
    mpc_err_print(r.error);
    mpc_err_delete(r.error);
  }

  del_scope(env);

  p = parse_clean(p);
}

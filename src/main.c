#include "error.h"
#include "yippy.h"
#include <gc.h>
#include <stdio.h>
#include <string.h>

const char *HELP_TEXT = "A LISP like interpreted language.\n\n\
USAGE:\n\
	yippy [ARGUEMENTS]\n\
\n\
ARGUEMENTS:\n\
    -h, --help            Prints help information\n\
    -v, --version         Prints version information\n\
    -e, --eval		  Evaluate the S-Expression\n\
    -f, --file		  Evaluate specified file\n";

int main(int argc, char *argv[]) {
  if (argc > 3) {
    EPRINT("Passed too many arguements. Passed %d, expected 3.\n", argc);
    return -1;
  }

  if (argc == 1) {
    eval_line();
  } else if (!strcmp(argv[1], "--version") || !strcmp(argv[1], "-v")) {
    printf("%s %s\n", PROG_NAME, VERSION);
    printf("Compiled on %s for %s.\n", __TIMESTAMP__, OS);

  } else if (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) {
    printf("%s %s\n\n%s\n", PROG_NAME, VERSION, HELP_TEXT);
  } else if (!strcmp(argv[1], "--file") || !strcmp(argv[1], "-f")) {
    if (argv[2] == NULL || !strlen(argv[2])) {
      EPRINT("[Error]: You forgot to pass a filename.\n");
      return -1;
    }
    if (strcmp(get_extension(argv[2]), "yy")) {
      EPRINT("[WARNING]: File name must end with \"yy\"\n");
    }
    eval_file(argv[2]);
  } else if (!strcmp(argv[1], "--eval") || !strcmp(argv[1], "-e")) {
    if (argv[2] == NULL || !strlen(argv[2])) {
      EPRINT("[Error]: You forgot to pass a S-expression.\n");
      return -1;
    } else {
      eval_inline(argv[2]);
    }
  } else {
    EPRINT("Unknown argument passed. Type -h for help.\n\n");
    printf("%s %s\n\n%s\n", PROG_NAME, VERSION, HELP_TEXT);
  }

  return 0;
}

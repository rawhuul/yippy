#include "error.h"
#include "yippy.h"
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
    eval();
  } else if (!strcmp(argv[1], "--version") || !strcmp(argv[1], "-v")) {
    printf("%s %0.1f\n", PROG_NAME, VERSION);
  } else if (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) {
    printf("%s %0.1f\n\n%s\n", PROG_NAME, VERSION, HELP_TEXT);
  } else if (!strcmp(argv[1], "--file") || !strcmp(argv[1], "-f")) {
    if (strcmp(get_extension(argv[2]), "yy")) {
      printf("[WARNING]: File name must end with \"yy\"\n");
    }
    eval_file(argv[2]);
  } else if (!strcmp(argv[1], "--eval") || !strcmp(argv[1], "-e")) {
    eval_inline(argv[2]);
  } else {
    printf("Unknown argument passed. Type -h for help.\n\n");
    printf("%s %0.1f\n\n%s\n", PROG_NAME, VERSION, HELP_TEXT);
  }

  return 0;
}

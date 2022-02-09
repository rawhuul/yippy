#include "yippy.h"
#include <stdio.h>
#include <string.h>

const char *HELP_TEXT = "A LISP like interpreted language.\n\
USAGE:\n\
	yippy [ARGUEMENTS]\n\
\n\
ARGUEMENTS:\n\
    -h, --help            Prints help information\n\
    -v, --version         Prints version information\n\
    -e, --eval		  Evaluate the S-Expression\n\
    -f, --file		  Evaluate specified file\n";

int main(int argc, char *argv[]) {

  /* for (int i = 0; i < argc; ++i) { */
  /* printf("%d.%s-->[%p]\n", i + 1, argv[i], argv); */
  /* } */

  if (argc > 3) {
    fprintf(stderr, "Passed too many arguements. Passed %d, expected 3.", argc);
    return -1;
  }

  if (argc == 1) {
    eval();
  } else if (!strcmp(argv[1], "--version") || !strcmp(argv[1], "-v")) {
    printf("%s %s\n", PROG_NAME, VERSION);
  } else if (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) {
    printf("%s %s\n%s\n", PROG_NAME, VERSION, HELP_TEXT);
  } else if (!strcmp(argv[1], "--file") || !strcmp(argv[1], "-f")) {
    eval_file(argc, argv);
  } else if (!strcmp(argv[1], "--eval") || !strcmp(argv[1], "-e")) {
    eval_inline(argv);
  }

  return 0;
}

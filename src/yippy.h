#ifndef YIPPY
#define YIPPY

#ifdef _WIN32
#define GET_INPUT(x) line(x)
#else
#define GET_INPUT(x) linenoise(x)
#endif

#ifndef OS
#define OS "unknown"
#endif

#define PROG_NAME "yippy"
#define VERSION "0.0.1"
#define URL "https://github.com/basicfunc/yippy"

#define AUTHOR "Rahul"
#define LICENSE "MIT"

#define HIST_FILE ".yippy_hsts"
#define YIPPY_PROMPT ">>> "

const char *HELP_TEXT = "A LISP like interpreted language.\n\
USAGE:\n\
	yippy [ARGUEMENTS]\n\
\n\
ARGUEMENTS:\n\
    -h, --help            Prints help information\n\
    -v, --version         Prints version information\n\
    -e, --eval		  Evaluate the S-Expression\n\
    -f, --file		  Evaluate specified file\n";

/* #define WELCOME_MSG(a, b, c, d) a##"Welcome to"##b##"v"##c##d##"\n" */

char *line(char *prompt);
void eval();
void eval_file(int argc, char **argv);
#endif

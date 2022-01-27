#ifndef YIPPY_PARSER
#define YIPPY_PARSER

#include "mpc.h"

typedef mpc_parser_t parsed;

parsed *parse();
void parse_clean(void);

#endif

#include "parser.h"
#include "mpc.h"
#include <stdio.h>
#include <stdlib.h>

/* parser *parse() { */
/*   parser *to_parse = (parser *)malloc(sizeof(parser)); */
/*   to_parse->Number = mpc_new("number"); */
/*   to_parse->Symbol = mpc_new("operator"); */
/*   to_parse->Sexpr = mpc_new("sexpr"); */
/*   to_parse->Qexpr = mpc_new("qexpr"); */
/*   to_parse->Expr = mpc_new("expr"); */
/*   to_parse->Yippy = mpc_new("yippy"); */

/*   mpca_lang(MPCA_LANG_DEFAULT, grammer, to_parse->Number, to_parse->Symbol,
 */
/*             to_parse->Sexpr, to_parse->Qexpr, to_parse->Expr,
 * to_parse->Yippy); */

/*   return to_parse; */
/* } */

/* void parse_clean(parser *to_free) { */
/*   mpc_cleanup(6, to_free->Number, to_free->Symbol, to_free->Sexpr, */
/*               to_free->Qexpr, to_free->Expr, to_free->Yippy); */
/*   free(to_free); */
/* } */

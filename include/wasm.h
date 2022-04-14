#ifndef YIPPY_WASM
#define YIPPY_WASM

#include "types.h"

char *eval_wasm(char *code);
void wa_print(value *v, char *str);

#endif

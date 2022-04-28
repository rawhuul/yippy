#include <gc.h>

#define malloc(x) GC_malloc(x)
#define calloc(n, x) GC_malloc((n) * (x))
#define realloc(p, x) GC_realloc((p), (x))
#define free(x) (x) = NULL

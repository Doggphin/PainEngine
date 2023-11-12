#ifndef MACROS_H
#define MACROS_H
#include <stdlib.h>

#define QMALLOC(NAME, TYPE) \
    struct TYPE* NAME = malloc(sizeof(TYPE));

#endif
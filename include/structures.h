#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <limits.h>

#define MAX(a, b) ((a < b) ? b : a)

typedef char T;

// Les énumérations sont des int (4 octets)
typedef enum { FALSE, TRUE } BOOL;

typedef enum { STACK, QUEUE } LIST_STRUCT;

#endif

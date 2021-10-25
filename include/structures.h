#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define MAX(a, b) (a<b)?b:a

typedef char T;

// Les énumérations sont des int (4 octets)
typedef enum {
	FALSE,
	TRUE
} BOOL;

#endif

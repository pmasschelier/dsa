#ifndef DISTTYPE_H
#define DISTTYPE_H

#include <limits.h>
#include "config.h"

typedef STRUCT_DIST_TYPE dist_t;

static inline dist_t dist_add_truncate_overflow(dist_t a, dist_t b) {
	if (a > STRUCT_DIST_INF - b)
		return STRUCT_DIST_INF;
	else
		return a + b;
}

#endif	// !DISTTYPE_H

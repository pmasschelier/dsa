#ifndef DISTTYPE_H
#define DISTTYPE_H

#include <limits.h>
#include "config.h"
#include "config.h.in"

typedef GRAPH_WEIGHT_TYPE graph_weight_t;

static inline graph_weight_t dist_add_truncate_overflow(graph_weight_t a,
														graph_weight_t b) {
	if (a > (GRAPH_WEIGHT_INF - 1) - b)
		return GRAPH_WEIGHT_INF - 1;
	else
		return a + b;
}

#endif	// !DISTTYPE_H

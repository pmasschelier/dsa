#ifndef DISTTYPE_H
#define DISTTYPE_H

#include <limits.h>
#include "config.h"

typedef GRAPH_WEIGHT_TYPE graph_weight_t;

static inline graph_weight_t dist_add_truncate_overflow(graph_weight_t a,
														graph_weight_t b) {
	if (a > GRAPH_WEIGHT_INF - b)
		return GRAPH_WEIGHT_INF;
	else
		return a + b;
}

#endif	// !DISTTYPE_H

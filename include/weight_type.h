#ifndef DISTTYPE_H
#define DISTTYPE_H

#include <limits.h>
#include "config.h"
#include "config.h.in"
#include "structures.h"

typedef GRAPH_WEIGHT_TYPE graph_weight_t;

static inline graph_weight_t weight_add_truncate_overflow(graph_weight_t a,
														  graph_weight_t b) {
	// If the sign of the weigts are the same
	BOOL same_sign = (1 & ((a ^ b) >> (GRAPH_WEIGHT_WIDTH - 1))) == 0;
	if (same_sign == FALSE)
		return a + b;
	// And one of the weights or both is infinity
	BOOL overflow_inf = (a == GRAPH_WEIGHT_INF || b == GRAPH_WEIGHT_INF);
	if (overflow_inf == TRUE)
		return GRAPH_WEIGHT_INF;
	// If the sign of the weigts are the same
	BOOL overflow_max = (a > 0 && (a > (GRAPH_WEIGHT_INF - 1) - b)) ||
						(a < 0 && (-a < (GRAPH_WEIGHT_INF - 1) + b));
	if (overflow_max == TRUE)
		return a > 0 ? GRAPH_WEIGHT_INF - 1 : -(GRAPH_WEIGHT_INF - 1);
	return a + b;
}

#endif	// !DISTTYPE_H

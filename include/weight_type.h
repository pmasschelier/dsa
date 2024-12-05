#ifndef DISTTYPE_H
#define DISTTYPE_H

#include <limits.h>
#include "config.h"
#include "config.h.in"

typedef GRAPH_WEIGHT_TYPE graph_weight_t;

graph_weight_t weight_add_truncate_overflow(graph_weight_t a, graph_weight_t b);

#endif	// !DISTTYPE_H

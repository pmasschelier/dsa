#include <assert.h>
#include <weight_type.h>

int main(void) {
	graph_weight_t a;
	graph_weight_t b;
	a = GRAPH_WEIGHT_INF;
	b = 2;
	assert(weight_add_truncate_overflow(a, b) == GRAPH_WEIGHT_INF);
	a = 2;
	b = GRAPH_WEIGHT_INF;
	assert(weight_add_truncate_overflow(a, b) == GRAPH_WEIGHT_INF);
	a = -GRAPH_WEIGHT_INF;
	b = -2;
	assert(weight_add_truncate_overflow(a, b) == GRAPH_WEIGHT_INF);
	a = -2;
	b = -GRAPH_WEIGHT_INF;
	assert(weight_add_truncate_overflow(a, b) == GRAPH_WEIGHT_INF);
	return 0;
}

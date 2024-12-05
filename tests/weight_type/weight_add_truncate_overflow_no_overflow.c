#include <assert.h>
#include <weight_type.h>

int main(void) {
	graph_weight_t a;
	graph_weight_t b;
	a = GRAPH_WEIGHT_INF - 4;
	b = 2;
	assert(weight_add_truncate_overflow(a, b) == GRAPH_WEIGHT_INF - 2);
	a = -(GRAPH_WEIGHT_INF - 4);
	b = -2;
	assert(weight_add_truncate_overflow(a, b) == -(GRAPH_WEIGHT_INF - 2));
	a = -7;
	b = -7;
	assert(weight_add_truncate_overflow(a, b) == -14);
	return 0;
}

#include <assert.h>
#include <weight_type.h>

int main(void) {
	graph_weight_t a;
	graph_weight_t b;
	a = GRAPH_WEIGHT_INF - 4;
	b = 4;
	assert(weight_add_truncate_overflow(a, b) == GRAPH_WEIGHT_INF - 1);
	a = -(GRAPH_WEIGHT_INF - 4);
	b = -4;
	assert(weight_add_truncate_overflow(a, b) == -(GRAPH_WEIGHT_INF - 1));
	return 0;
}

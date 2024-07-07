#include <graph/graph_list.h>

int main(void) {
	graph_list_t* g = create_graph_list(5, FALSE);

	free_graph_list(g);
	return 0;
}

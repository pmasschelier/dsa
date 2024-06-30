#include <graph_mat.h>

int main(void) {
	graph_mat_t* g = create_graph_mat(5, FALSE);

	free_graph_mat(g);
	return 0;
}

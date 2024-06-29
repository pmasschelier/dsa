
#include <assert.h>
#include <btree_ref.h>

int main(void) {
	btree_path_t path = ROOT_PATH;
	path_lhs(path_rhs(path_lhs(path_rhs(path_lhs(path_lhs(&path))))));
	//       0      1       0         1        0        0

	assert(path.length == 6);
	assert(path.path == 0x14);

	return 0;
}

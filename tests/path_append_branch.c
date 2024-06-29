
#include <assert.h>
#include <btree_ref.h>

int main(void) {
	btree_path_t path = ROOT_PATH;
	for (int i = 0; i < 10; i++) {
		path_append_branch(
			&path, (i & 1) ? RHS_PATH : ((i & 7) ? LHS_PATH : RHS_PATH));
	}
	assert(path.length == 10);
	assert(path.path == 0x3AB);

	return 0;
}

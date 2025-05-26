#include <assert.h>
#include "compare.h"
#include "errors.h"
#include "htable_ref/htable_list_ref.h"

DEFINE_EQUALS_SCALAR(int)

int main(void) {
	htable_list_ref_t* htable = create_htable_list(100, equals_int, sizeof(int));
	for (int i = 0; i < 100; i++) {
		void* found;
		int res;
		res = htable_list_insert(htable, i, &i, &found);
		assert(res == -ERROR_NO_ERROR);
		res = htable_list_insert(htable, i, &i, &found);
		assert(res == -ERROR_KEY_ALREADY_EXISTS);
		assert(*(int*)found == i);
	}
	free_htable_list(htable);
	return 0;
}

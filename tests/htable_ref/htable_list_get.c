#include <assert.h>
#include "compare.h"
#include "errors.h"
#include "htable_ref/htable_list_ref.h"
#include "ptr.h"

#define BUCKETS 100
DEFINE_EQUALS_SCALAR(int)

int main(void) {
	htable_list_ref_t* htable = create_htable_list(BUCKETS, equals_int);
	for (int i = 0; i < BUCKETS; i += 2) {
		int res = htable_list_insert(htable, i, ptr(TYPE_INT, i), NULL);
		assert(res == -ERROR_NO_ERROR);
	}
	for (int i = 0; i < BUCKETS; i++) {
		void* value = htable_list_get(htable, i, &i);
		assert((value != NULL) == (i % 2 == 0));
		if (value != NULL)
			assert(*(int*)value == i);
	}
	free_htable_list(htable);
	return 0;
}

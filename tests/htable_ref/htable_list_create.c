#include "htable_ref/htable_list_ref.h"

int main(void) {
	htable_list_ref_t* htable = create_htable_list(100, NULL);
	free_htable_list(htable);
	return 0;
}

#include <stddef.h>
#include <array_list.h>
#include <stdint.h>
#include <tap.h>

#define ARRAY_TYPE int

int main(void) {
	array_list_t* array = array_list_create(sizeof(ARRAY_TYPE));
    ok(array != NULL, "array_list_create return non-null");
	cmp_ok(array->size, "==", 0, "array_list_create => size");
	cmp_ok(array->capacity, "==", 0, "array_list_create => capacity");
	cmp_ok(array->size_bytes, "==", sizeof(ARRAY_TYPE), "array_list_create => size_bytes");
	ok(array->data == NULL, "array_list_create => data");
	array_list_free(array);
	return 0;
}

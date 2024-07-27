#ifndef COMPARE_H
#define COMPARE_H

typedef int (*compare_fn_t)(void*, void*);

#define DEFINE_COMPARE_SCALAR(type)                               \
	int compare_##type(void* a, void* b) {                        \
		return (*(type*)a > *(type*)b) - (*(type*)a < *(type*)b); \
	}

#endif	// !COMPARE_H

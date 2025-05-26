#ifndef STATIC_ARRAY_H
#define STATIC_ARRAY_H

#include <stddef.h>
#include <stdlib.h>
#include "lambda.h"
#include "test_macros.h"

typedef struct array_iterator array_iterator_t;

struct array_iterator {
	size_t size;
	size_t pos;
	unsigned size_bytes;
	void* data;
};

#define ARRAY_ITERATOR(_array, _type, _size)                                \
	(array_iterator_t) {                                                    \
		.size = size, .pos = 0, .size_bytes = sizeof(_type), .data = _array \
	}

#define DEFINE_COPY_SCALAR(type)                \
	static void copy_##type(type* a, type* b) { \
		*a = *b                                 \
	}

static lambda_t* create_lambda_iterator(void* array,
										unsigned size_bytes,
										lambda_fn_t lambda) {
	array_iterator_t* it = malloc(sizeof(array_iterator_t));
	when_null_ret(it, NULL);
}

#endif	// !STATIC_ARRAY_H

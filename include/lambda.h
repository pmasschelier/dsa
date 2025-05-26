#ifndef LAMBDA_H
#define LAMBDA_H

#include <stddef.h>

typedef void (*lambda_fn_t)(void* priv, void* data);

typedef struct lambda lambda_t;

struct lambda {
	void* priv;
	lambda_fn_t fn;
};

#endif	// !LAMBDA_H

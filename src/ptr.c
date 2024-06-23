#include "ptr.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

void* ptr(enum type t, ...) {
	va_list ap;
	va_start(ap, t);
	void* p = NULL;

	switch (t) {
	case TYPE_INT:
		p = malloc(sizeof(int));
		int i = va_arg(ap, int);
		memcpy(p, &i, sizeof(int));
		break;
	case TYPE_DOUBLE:
		p = malloc(sizeof(double));
		double d = va_arg(ap, double);
		memcpy(p, &d, sizeof(double));
		break;
	default:
		break;
	}

	va_end(ap);
	return p;
}

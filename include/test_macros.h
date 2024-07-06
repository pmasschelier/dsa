#ifndef TEST_MACROS_H
#define TEST_MACROS_H

#ifndef TEST_FAIL_FUNC

#ifdef TEST_FAIL_FUNC_NOCHECK
#define TEST_FAIL_FUNC(...)
#elif defined(TEST_FAIL_FUNC_RET)
#define TEST_FAIL_FUNC(boolean, ret, cb) \
	do {                                 \
		if (!(boolean)) {                \
			cb;                          \
			return ret;                  \
		}                                \
	} while (0)
#else
#include <assert.h>
#define TEST_FAIL_FUNC(boolean, ...) assert(boolean)
#endif

#endif	// !TEST_FAIL_FUNC

#ifndef TEST_PTR_FAIL_FUNC
#define TEST_PTR_FAIL_FUNC(ptr, ...) TEST_FAIL_FUNC(ptr != NULL, __VA_ARGS__)
#endif	// !TEST_PTR_FAIL_FUNC

#endif	// !TEST_MACROS_H

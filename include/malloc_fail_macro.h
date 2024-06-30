#ifndef MALLOC_FAIL_TEST_FUNC
#ifdef MALLOC_FAIL_TEST_NOCHECK
#define MALLOC_FAIL_TEST_FUNC(...)
#elif defined(MALLOC_FAIL_TEST_RET)
#define MALLOC_FAIL_TEST_FUNC(ptr, ret, cb) \
	if (ptr == NULL) {                      \
		cb;                                 \
		return ret;                         \
	}
#else
#include <assert.h>
#define MALLOC_FAIL_TEST_FUNC(ptr, ...) assert(ptr != NULL)
#endif
#endif	// !DEBUG

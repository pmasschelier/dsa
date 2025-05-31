#include "circular_buffer.h"
#include <assert.h>
#include <stddef.h>

#define BUF_LEN 10
#define BUF_TYPE int

int main(void) {
  circular_buffer_t *buffer = circular_buffer_create(sizeof(BUF_TYPE), BUF_LEN);
  assert(buffer != NULL);
  assert(buffer->capacity == BUF_LEN);
  assert(buffer->first == 0);
  assert(buffer->last == -1);
  assert(buffer->size == 0);
  assert(buffer->data != NULL);
  assert(buffer->size_bytes == sizeof(BUF_TYPE));
  circular_buffer_free(buffer);
  return 0;
}

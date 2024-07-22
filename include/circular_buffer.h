#ifndef STRUCT_CBUFFER_H
#define STRUCT_CBUFFER_H

#define get_buffer_last(buffer, type) \
	(type*)((buffer)->data + buffer->last * buffer->size_bytes)

typedef struct cbuffer_ref circular_buffer_t;

struct cbuffer_ref {
	char* data;
	unsigned size_bytes;
	unsigned capacity;
	int first;
	int last;
	int size;
};

circular_buffer_t* create_circular_buffer(unsigned size_bytes,
										  unsigned capacity);

void free_circular_buffer(circular_buffer_t* b);

int circular_buffer_pop_front(circular_buffer_t* b, void* p);

int circular_buffer_pop_back(circular_buffer_t* b, void* p);

int circular_buffer_push_front(circular_buffer_t* b, void* d);

int circular_buffer_push_back(circular_buffer_t* b, void* d);

int circular_buffer_size(circular_buffer_t* b);

#endif	// !STRUCT_CBUFFER_H

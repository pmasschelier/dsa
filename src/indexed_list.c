#include "indexed_list.h"
#include "errors.h"
#include "test_macros.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

indexed_list_t* indexed_list_create(size_t size_byte) {
    indexed_list_t* ret = malloc(sizeof(indexed_list_t));
    when_null_ret(ret, NULL);
    ret->begin = -1;
    ret->end = -1;
    ret->capacity = 0;
    ret->size = 0;
    ret->size_bytes = size_byte;
    ret->data = NULL;
    ret->next = NULL;
    ret->prev = NULL;
    return ret;
}

void indexed_list_free(indexed_list_t *list) {
    indexed_list_clean(list);
    free(list);
}

indexed_list_t* indexed_list_from_tab(void* tab, size_t size, unsigned length);

void* indexed_list_to_tab(indexed_list_t* list, void* tab);

// TODO: Bulk add
indexed_list_t* indexed_list_from_tab(void* tab, size_t size_bytes, unsigned length) {
    indexed_list_t* list = indexed_list_create(size_bytes);
    for(unsigned i = 0; i < length; i++)
        indexed_list_insert(list, list->end, (uint8_t*)tab + i * size_bytes);
    return list;
}

unsigned indexed_list_length(indexed_list_t* list) {
    return list->size;
}

int indexed_list_min_capacity = 8;

int indexed_list_extend(indexed_list_t* list) {
    if(list->size >= list->capacity) {
        if(list->capacity < indexed_list_min_capacity)
            list->capacity = indexed_list_min_capacity;
        else
            list->capacity *= 2;
        list->data = realloc(list->data, list->capacity * list->size_bytes);
        list->next = realloc(list->next, list->capacity * sizeof(int));
        list->prev = realloc(list->prev, list->capacity * sizeof(int));
    }
    list->size += 1;
    return list->size - 1;
}

int indexed_list_insert(indexed_list_t* list, int prev, void* data) {
    when_false_ret(prev < list->size, -ERROR_INVALID_PARAM2);
    int index = indexed_list_extend(list);
    int next = prev < 0 ? list->begin : list->next[prev];
    list->prev[index] = prev;
    list->next[index] = next;
    memcpy(list->data + index * list->size_bytes, data, list->size_bytes);
    if(prev < 0)
        list->begin = index;
    else
        list->next[prev] = index;
    if(next < 0)
        list->end = index;
    else
        list->prev[next] = index;
    return index;
}

int indexed_list_remove(indexed_list_t* list, int index, void* data) {
    when_false_ret(index >= 0 && index < list->size, -ERROR_INVALID_PARAM2);
    int prev = list->prev[index];
    int next = list->next[index];
    if(prev >= 0)
        list->next[prev] = next;
    else
        list->begin = next;
    if(next >= 0)
        list->prev[next] = prev;
    else
        list->end = prev;
    if(data != NULL)
        memcpy(data, list->data + index * list->size_bytes, list->size_bytes);
    if(index < list->size - 1) {
        int prev = list->prev[list->size - 1];
        int next = list->next[list->size - 1];
        list->next[index] = next;
        list->prev[index] = prev;
        if(prev >= 0)
            list->next[prev] = index;
        else
            list->begin = index;
        if(next >= 0)
            list->prev[next] = index;
        else
            list->end = index;
        memcpy(
            list->data + index * list->size_bytes,
            list->data + (list->size - 1) * list->size_bytes,
            list->size_bytes
        );
    }
    list->size -= 1;
    return next;
}

void indexed_list_clean(indexed_list_t* list) {
    free(list->data);
    free(list->next);
    free(list->prev);
    list->begin = -1;
    list->end = -1;
    list->capacity = 0;
    list->size = 0;
    list->data = NULL;
    list->next = NULL;
    list->prev = NULL;
}

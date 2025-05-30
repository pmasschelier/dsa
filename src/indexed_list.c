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
    indexed_list_init(ret, size_byte);
    return ret;
}

void indexed_list_free(indexed_list_t *list) {
    indexed_list_clean(list);
    free(list);
}

void indexed_list_init(indexed_list_t* list, size_t size_bytes) {
    list->begin = -1;
    list->end = -1;
    list->capacity = 0;
    list->size = 0;
    list->size_bytes = size_bytes;
    list->data = NULL;
    list->next = NULL;
    list->prev = NULL;
}

void indexed_list_deinit(indexed_list_t* list) {
    indexed_list_clean(list);
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

int indexed_list_insert(indexed_list_t* list, int prev, const void* data) {
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

int indexed_list_swap(indexed_list_t* list, int a, int b) {
    when_false_ret(a >= 0 && a < list->size, -ERROR_INVALID_PARAM2);
    when_false_ret(b >= 0 && b < list->size, -ERROR_INVALID_PARAM3);
    if(a == b)
        return -ERROR_NO_ERROR;
    int preva = list->prev[a];
    int nexta = list->next[a];
    int prevb = list->prev[b];
    int nextb = list->next[b];
    if(preva >= 0)
        list->next[preva] = b;
    else
        list->begin = b;
    if(nexta >= 0)
        list->prev[nexta] = b;
    else
        list->end = b;
    if(prevb >= 0)
        list->next[prevb] = a;
    else
        list->begin = a;
    if(nextb >= 0)
        list->prev[nextb] = a;
    else
        list->end = a;

    if(a == prevb)
        list->prev[a] = b;
    else
        list->prev[a] = prevb;
    if(a == nextb)
        list->next[a] = b;
    else
        list->next[a] = nextb;

    if(b == preva)
        list->prev[b] = a;
    else
        list->prev[b] = preva;
    if(b == nexta)
        list->next[b] = a;
    else
        list->next[b] = nexta;
    return -ERROR_NO_ERROR;
}

int indexed_list_move_after(indexed_list_t* list, int index, int prev) {
    when_false_ret(index >= 0 && index < list->size, -ERROR_INVALID_PARAM2);
    when_false_ret(prev < list->size, -ERROR_INVALID_PARAM3);
    int next = prev >= 0 ? list->next[prev] : list->begin;
    int oldprev = list->prev[index];
    int oldnext = list->next[index];
    if(oldprev == prev || index == prev)
        return -ERROR_NO_ERROR;
    if(oldprev >= 0)
        list->next[oldprev] = oldnext;
    else
        list->begin = oldnext;
    if(oldnext >= 0)
        list->prev[oldnext] = oldprev;
    else
        list->end = oldprev;
    
    if(prev >= 0)
        list->next[prev] = index;
    else
        list->begin = index;
    if(next >= 0)
        list->prev[next] = index;
    else
        list->end = index;
    list->prev[index] = prev;
    list->next[index] = next;
    return -ERROR_NO_ERROR;
}

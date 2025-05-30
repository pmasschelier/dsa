#include "array_list.h"
#include "indexed_list.h"
#include "list_ref/linked_list.h"
#include "b63/register.h"
#include <b63/b63.h>
#include <b63/counters/perf_events.h>
#include <stdlib.h>
#include <assert.h>

#define ARRAY_TYPE int
#define ARRAY_LEN 4096

B63_BASELINE(c_array, n) {
    int* array;
    srand(b63_seed);
    for (unsigned i = 0; i < n; i++) {
        B63_SUSPEND {
            array = malloc(ARRAY_LEN * sizeof(ARRAY_TYPE));
            for (unsigned i = 0; i < ARRAY_LEN; i++) {
                array[i] = rand();
            }
        }
        for (unsigned i = 1; i < ARRAY_LEN; i++) {
            int value = array[i];
            int j;
            for(j = i; j > 0 && array[j - 1] > value; j--) {
                array[j] = array[j-1];
            }
            array[j] = value;
        }
        B63_SUSPEND {
            for (unsigned i = 1; i < ARRAY_LEN; i++) {
                int a = array[i-1];
                int b = array[i];
                assert(a <= b);
            }
        }
        free(array);
    }
}

B63_BENCHMARK(linked_list, n) {
    linked_list_t list = LINKED_LIST_INIT(int);
    srand(b63_seed);
    for (unsigned i = 0; i < n; i++) {
        B63_SUSPEND {
            for (unsigned i = 0; i < ARRAY_LEN; i++) {
                int value = rand();
                linked_list_push_back(&list, &value);
            }
        }
        linked_list_node_t* next;
        for(linked_list_node_t* node = list.begin; node != NULL; node = next) {
            next = node->next;
            int value = *get_node_ref(node, int);
            linked_list_node_t* prev;
            for(prev = node; prev != NULL; prev = prev->prev) {
                int x = *get_node_ref(prev, int);
                if(x < value)
                    break;
            }
            linked_list_extract(&list, node);
            linked_list_insert_node(&list, prev, node);
        }
        B63_SUSPEND {
            for(linked_list_node_t* node = list.begin; node->next != NULL; node = node->next) {
                int a = *get_node_ref(node, int);
                int b = *get_node_ref(node->next, int);
                assert(a <= b);
            }
        }
        linked_list_clean(&list);
    }
}

B63_BENCHMARK(indexed_list, n) {
    indexed_list_t list = INDEXED_LIST_INIT(int);
    srand(b63_seed);
    for (unsigned i = 0; i < n; i++) {
        B63_SUSPEND {
            for (unsigned i = 0; i < ARRAY_LEN; i++) {
                int value = rand();
                indexed_list_insert(&list, list.end, &value);
            }
        }
        int next;
        for(int index = list.next[list.begin]; index >= 0; index = next) {
            next = list.next[index];
            int value = *get_array_list_ref(list, index, int);
            int other;
            for(other = list.prev[index]; other >= 0 && *get_array_list_ref(list, other, int) >= value; other = list.prev[other]);
            indexed_list_move_after(&list, index, other);
        }
        B63_SUSPEND {
            for(int index = list.begin; list.next[index] >= 0; index = list.next[index]) {
                int a = *get_array_list_ref(list, index, int);
                int b = *get_array_list_ref(list, list.next[index], int);
                assert(a <= b);
            }
        }
        indexed_list_clean(&list);
    }
}

B63_BENCHMARK(array_list, n) {
    array_list_t array = ARRAY_LIST_INIT(ARRAY_TYPE);
    srand(b63_seed);
    for (unsigned i = 0; i < n; i++) {
        B63_SUSPEND {
            for (unsigned i = 0; i < ARRAY_LEN; i++) {
                int value = rand();
                array_list_push_back(&array, &value);
            }
        }
        for (unsigned i = 0; i < ARRAY_LEN; i++) {
            int value = *get_array_list_ref(array, i, int);
            unsigned j;
            for(j = i; j > 0; j--) {
                int x = *get_array_list_ref(array, j-1, int);
                if(x > value)
                    *get_array_list_ref(array, j, int) = x;
                else
                    break;
            }
            *get_array_list_ref(array, j, int) = value;
        }
        B63_SUSPEND {
            for (unsigned i = 1; i < ARRAY_LEN; i++) {
                int a = *get_array_list_ref(array, i - 1, int);
                int b = *get_array_list_ref(array, i, int);
                assert(a <= b);
            }
        }
        array_list_deinit(&array);
  }
}

int main(int argc, char *argv[]) {
  B63_RUN(argc, argv);
  return EXIT_SUCCESS;
}

#include "list_ref/linked_list.h"
#include <assert.h>

#define ARRAY_TYPE int
#define ARRAY_LEN 20

const ARRAY_TYPE VALUES[ARRAY_LEN] = { -12, 0, 3, 4, 2, -1, 7, 65, -3, 8, 1, -9, 6, 5, 9, -6, -8, -4, -2, -5};

int main(void)
{
    linked_list_t list = LINKED_LIST_INIT(ARRAY_TYPE);
    for (unsigned i = 0; i < ARRAY_LEN; i++)
        linked_list_push_back(&list, &VALUES[i]);
    linked_list_foreach(&list, node) {
        int min = *get_node_ref(node, int);
        linked_list_node_t* node_min = node;
        for(linked_list_node_t* other = node->next; other != NULL; other = other->next) {
            int x = *get_node_ref(other, int);
            if(x < min) {
                min = x;
                node_min = other;
            }
        }
        linked_list_swap(&list, &node, &node_min);
    }
    for(linked_list_node_t* node = list.begin; node->next != NULL; node = node->next) {
        int a = *get_node_ref(node, int);
        int b = *get_node_ref(node->next, int);
        assert(a <= b);
    }
    linked_list_clean(&list);
    return 0;
}

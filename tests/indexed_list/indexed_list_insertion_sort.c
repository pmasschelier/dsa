#include <indexed_list.h>
#include <tap.h>

#define ARRAY_TYPE int
#define ARRAY_LEN 20

const ARRAY_TYPE VALUES[ARRAY_LEN] = { -12,  0,  3,  4,  2, -1,  7, 65, -3, 8, 1, -9, 6, 5, 9, -6, -8, -4, -2, -5 };

int main(void)
{
    indexed_list_t list = INDEXED_LIST_INIT(ARRAY_TYPE);
    for (unsigned i = 0; i < ARRAY_LEN; i++)
        indexed_list_insert(&list, list.end, &VALUES[i]);
    int next;
    for(int index = list.next[list.begin]; index >= 0; index = next) {
        next = list.next[index];
        int value = *get_array_ref(list, index, int);
        int other;
        for(other = list.prev[index]; other >= 0 && *get_array_ref(list, other, int) >= value; other = list.prev[other]);
        indexed_list_move_after(&list, index, other);
    }
    for(int index = list.begin; list.next[index] >= 0; index = list.next[index]) {
        int a = *get_array_ref(list, index, int);
        int b = *get_array_ref(list, list.next[index], int);
        cmp_ok(a, "<=", b);
    }
    indexed_list_clean(&list);
    return 0;
}

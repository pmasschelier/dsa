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
    indexed_list_foreach(list, index) {
        int min = *get_array_ref(list, index, int);
        int index_min = index;
        for(int other = list.next[index]; other >= 0; other = list.next[other]) {
            int x = *get_array_ref(list, other, int);
            if(x < min) {
                min = x;
                index_min = other;
            }
        }
        indexed_list_swap(&list, index, index_min);
        index = index_min;
    }
    for(int index = list.begin; list.next[index] >= 0; index = list.next[index]) {
        int a = *get_array_ref(list, index, int);
        int b = *get_array_ref(list, list.next[index], int);
        cmp_ok(a, "<=", b);
    }
    indexed_list_clean(&list);
    return 0;
}

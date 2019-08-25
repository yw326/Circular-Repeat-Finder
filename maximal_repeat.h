



#include <stdio.h>
#include "sfxtree.h"
#include "edit_distance.h"
#include "str_helper.h"
#include "debug_helper.h"

#ifndef maximal_repeat_h
#define maximal_repeat_h

typedef struct {
    int *arr;
    short size;
} arr_with_size;

typedef struct {
    arr_with_size *arrs;
    short size;
} arr_list;

typedef struct {
    int p1;
    int p2;
    short length;
}triple;

typedef struct {
    triple *result;
    short size;
} triple_list;

typedef struct {
    triple_list *result;
    int size;
} result_list;

#endif /* maximal_repeat_h */

result_list* outputRepeatedPairs(treenode_t *root, char *str, int threshold, int reversed, int cat, int pound_idx);
result_list* outputRepeatedPairs_debug(treenode_t *root, char *str, int threshold, int reversed, int cat, int pound_idx);
void free_results(result_list *results);



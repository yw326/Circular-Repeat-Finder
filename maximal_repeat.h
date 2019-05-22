

#ifndef maximal_repeat_h
#define maximal_repeat_h

#include <stdio.h>
#include "sfxtree.h"
#include "edit_distance.h"
#include "KMP.h"
#include "str_helper.h"
#include "debug_helper.h"

typedef struct {
    int p1;
    int p2;
    short length;
}triple;


typedef struct {
    triple *result;
    short size;
} tuple;

typedef struct {
    tuple *result;
    int size;
} result_list;

#endif /* maximal_repeat_h */

result_list* outputRepeatedPairs(treenode_t *root, char *str, int threshold, int reversed, int cat);
void free_results(result_list *results);



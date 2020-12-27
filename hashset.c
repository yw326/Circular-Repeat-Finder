
#include "hashset.h"

// hashset_t hashset_create()
// {
//     hashset_t set = calloc(1, sizeof(struct hashset_st));

//     if (set == NULL) {
//         return NULL;
//     }
//     set->nbits = 3;
//     set->capacity = (size_t)(1 << set->nbits);
//     set->mask = set->capacity - 1;
//     set->items = calloc(set->capacity, sizeof(mrp));
//     // printf("test %d,%d,%d\n", set->items[0].p1, set->items[0].p2, set->items[0].length);
//     if (set->items == NULL) {
//         hashset_destroy(set);
//         return NULL;
//     }
//     set->nitems = 0;
//     set->n_deleted_items = 0;
//     return set;
// }

// size_t hashset_num_items(hashset_t set)
// {
//     return set->nitems;
// }

// void hashset_destroy(hashset_t set)
// {
//     if (set) {
//         free(set->items);
//     }
//     free(set);
// }

// static int hashset_add_member(hashset_t set, mrp newMrp)
// {   
//     // mrp* mrpPtr = (mrp *) item;
//     // if (mrpPtr->length == 0) {
//     //     return 1;
//     // }
//     size_t value = mrp_hashcode(newMrp);
//     size_t ii;

//     if (value == 0 || value == 1) {
//         return -1;
//     }

//     ii = first_hashcode(set, value);

//     while (set->items[ii].length != 0 && set->items[ii].length != 1) {
//         if (set->items[ii].p1 == newMrp.p1 && set->items[ii].p2 == newMrp.p2 && set->items[ii].length == newMrp.length) {
//             return 0;
//         } else {
//             /* search free slot */
//             ii = new_hashcode(set, ii);
//         }
//     }

//     set->nitems++;
//     if (set->items[ii].p1 == 1) {
//         set->n_deleted_items--;
//     }
//     set->items[ii].p1 = newMrp.p1;
//     set->items[ii].p2 = newMrp.p2;
//     set->items[ii].length = newMrp.length;
//     return 1;

// }

// static void maybe_rehash(hashset_t set)
// {
//     printf("rehash\n");
//     printf("current size: %d\n", set->capacity);
//     mrp *old_items;
//     size_t old_capacity, ii;

//     if (set->nitems + set->n_deleted_items >= (double)set->capacity * rehash_threshold) {
//         printf("************\n");
//         old_items = set->items;
//         printf("a\n");
//         old_capacity = set->capacity;
//         printf("b\n");
//         set->nbits++;
//         set->capacity = (size_t)(1 << set->nbits);
//         printf("c\n");
//         set->mask = set->capacity - 1;
//         set->items = calloc(set->capacity, sizeof(size_t));
//         printf("d\n");
//         set->nitems = 0;
//         set->n_deleted_items = 0;
//         printf("e\n");
//         assert(set->items);
//         printf("f\n");
//         for (ii = 0; ii < old_capacity; ii++) {
//             hashset_add_member(set, old_items[ii]);
//         }
//         printf("g\n");
//         free(old_items);
//         printf("h\n");
//     }
//     printf("size after: %d\n", set->capacity);
// }

// int hashset_add(hashset_t set, mrp newMrp)
// {
//     int rv = hashset_add_member(set, newMrp);
//     maybe_rehash(set);
//     return rv;
// } 

// // int hashset_remove(hashset_t set, void *item)
// // {
// //     size_t value = (size_t)item;
// //     size_t ii = first_hashcode(set, value);

// //     // while (set->items[ii] != 0) {
// //     //     if (set->items[ii] == value) {
// //     //         set->items[ii] = 1;
// //     //         set->nitems--;
// //     //         set->n_deleted_items++;
// //     //         return 1;
// //     //     } else {
// //     //         ii = new_hashcode(set, ii);
// //     //     }
// //     // }
// //     return 0;
// // }



static const unsigned int prime_1 = 73;
static const unsigned int prime_2 = 5009;
static const double rehash_threshold = 0.75;

int first_hashcode(hashset_t set, size_t value) {
    return set->mask & (prime_1 * value);
}

int new_hashcode(hashset_t set, size_t ii) {
    return set->mask & (ii + prime_2);
}

int mrp_hashcode(mrp m) {
    return (m.p1 + m.p2 + m.length) * 43;
}

hashset_t hashset_create() {
    hashset_t set = calloc(1, sizeof(struct hashset_st));

    if (set == NULL) {
        return NULL;
    }
    set->nbits = 3;
    set->capacity = (size_t)(1 << set->nbits);
    set->mask = set->capacity - 1;
    set->items = calloc(set->capacity, sizeof(mrp));
    if (set->items == NULL) {
        hashset_destroy(set);
        return NULL;
    }
    return set;
}

void hashset_destroy(hashset_t set) {
    if (set) {
        free(set->items);
    }
    free(set);
}

static int hashset_add_member(hashset_t set, mrp newMrp)
{   
    size_t value = mrp_hashcode(newMrp);
    size_t ii;

    // if (value == 0 || value == 1) {
    //     return -1;
    // }

    ii = first_hashcode(set, value);

    while (set->items[ii].length != 0) {
        if (set->items[ii].p1 == newMrp.p1 && set->items[ii].p2 == newMrp.p2 && set->items[ii].length == newMrp.length) {
            return 0;
        } else {
            /* search free slot */
            ii = new_hashcode(set, ii);
        }
    }

    set->numberOfItems++;
    set->items[ii].p1 = newMrp.p1;
    set->items[ii].p2 = newMrp.p2;
    set->items[ii].length = newMrp.length;
    return 1;
}

static void maybe_rehash(hashset_t set) {
    mrp *old_items;
    size_t old_capacity, ii;

    if (set->numberOfItems >= (double)set->capacity * rehash_threshold) {
        old_items = set->items;
        old_capacity = set->capacity;
        set->nbits++;
        set->capacity = (size_t)(1 << set->nbits);
        set->mask = set->capacity - 1;
        set->items = calloc(set->capacity, sizeof(mrp));
        set->numberOfItems = 0;
        for (ii = 0; ii < old_capacity; ii++) {
            hashset_add_member(set, old_items[ii]);
        }
        free(old_items);
    }
}

int hashset_add(hashset_t set, mrp newMrp)
{
    int rv = hashset_add_member(set, newMrp);
    maybe_rehash(set);
    return rv;
} 

int hashset_is_member(hashset_t set, mrp newMrp)
{
    size_t value = mrp_hashcode(newMrp);
    size_t ii = first_hashcode(set, value);
    
    while (set->items[ii].length != 0) {
        if (set->items[ii].p1 == newMrp.p1 && set->items[ii].p2 == newMrp.p2 && set->items[ii].length == newMrp.length) {
            return 1;
        } else {
            ii = new_hashcode(set, ii);
        }
    }
    return 0;
}

mrpList* removeDuplicates(mrpList* resultWithDuplicates) {
    hashset_t set = hashset_create();
    mrpList* ml = malloc(sizeof(mrpList));
    mrp* resultMRPs = malloc(sizeof(mrp)*resultWithDuplicates->size);
    int size = 0;
    for (int i = 0; i < resultWithDuplicates->size; i++) {
        mrp m = resultWithDuplicates->mrps[i];
        if (!hashset_is_member(set, m)) {
            resultMRPs[size].p1 = m.p1;
            resultMRPs[size].p2 = m.p2;
            resultMRPs[size].length = m.length;
            size++;
            hashset_add(set,m);
        }
    }
    ml->mrps = resultMRPs;
    ml->size = size;
    hashset_destroy(set);
    return ml;
}
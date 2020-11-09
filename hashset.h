
#ifndef HASHSET_H
#define HASHSET_H 


#include <stdlib.h>
#include "MyStructs.h"


    // struct hashset_st {
    //     size_t nbits;
    //     size_t mask;

    //     size_t capacity;
    //     mrp *items;
    //     size_t nitems;
    //     size_t n_deleted_items;
    // };

    struct hashset_st {
        size_t nbits;
        size_t mask;

        size_t capacity;
        mrp *items;
        int numberOfItems;
    };

    typedef struct hashset_st *hashset_t;

    /* create hashset instance */
    hashset_t hashset_create(void);

    /* destroy hashset instance */
    void hashset_destroy(hashset_t set);

    /* add item into the hashset.
     *
     * @note 0 and 1 is special values, meaning nil and deleted items. the
     *       function will return -1 indicating error.
     *
     * returns zero if the item already in the set and non-zero otherwise
     */
    int hashset_add(hashset_t set, mrp newMrp);

    /* check if existence of the item
     *
     * returns non-zero if the item exists and zero otherwise
     */
    int hashset_is_member(hashset_t set, mrp newMrp);

#endif
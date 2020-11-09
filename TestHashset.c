#include "TestHashset.h"


int testHashset() {
    // arrange
    // mrp *mrp1 = malloc(sizeof(mrp));
    // mrp1->p1 = 2;
    // mrp1->p2 = 3;
    // mrp1->length = 4;

    // mrp *mrp2 = malloc(sizeof(mrp));
    // mrp2->p1 = 2;
    // mrp2->p2 = 3;
    // mrp2->length = 4;

    mrp mrp1 = {2,3,4};
    mrp mrp2 = {2,3,4};
    hashset_t set = hashset_create();
    

    // act
    hashset_add(set, mrp1);

    // assert
    if (!hashset_is_member(set, mrp2)) {
        printf("Failed: testHashset, failed to identify added element \n");
        return 1;
    }

    printf("Passed: testHashset\n");
    hashset_destroy(set);
    return 0;
}

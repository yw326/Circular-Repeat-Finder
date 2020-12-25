#include "TestHashset.h"


int testHashset() {
    // arrange
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

int testRemoveDuplicates() {
    // arrange
    int numberOfItems = 4;
    mrpList* ml = malloc(sizeof(mrpList));
    mrp* mrps = malloc(sizeof(mrp)*numberOfItems);
    mrp mrp1 = {2, 10, 3};
    mrp mrp2 = {95, 1456, 40};
    mrp mrp3 = {96, 1456, 40};
    mrp mrp4 = {2, 10, 3};
    mrps[0] = mrp1;
    mrps[1] = mrp2;
    mrps[2] = mrp3;
    mrps[3] = mrp4;
    ml->size = numberOfItems;
    ml->mrps = mrps;

    // act 
    mrpList* resultMl = removeDuplicates(ml);
    freeMRPList(ml);

    // assert
    if (resultMl->size != 3) {
        printf("Failed: testRemoveDuplicates, number of elements incorrect. Expected 3, found %d \n", resultMl->size);
        return 1;
    }

    return 0;
}

#include "TestList.h"

int testSingleNodeList() {
    // arrange
    List *lp = initList();
    int val = 0;
    
    // act
    add(lp, val);

    // assert
    int failed = 0;
    if (lp->head->val == val && !lp->head->next) {
        printf("Passed: singleNodeList test\n");
    } else {
        failed = 1;
        printf("Failed: singleNodeList test\n");
    }
    freeList(lp);

    return failed;
}

int testMultipleNodeList() {
    // arrange
    List *lp = initList();
    int numberOfNodes = 3;
        
    // act
    for(int i = 0; i < numberOfNodes; i++) { add(lp, i); }

    // assert
    int failed = 0;
    Node *curr = lp->head;
    for(int i = 0; i < numberOfNodes; i++) {
        if (curr->val != i) {
            failed = 1;
            printf("Failed: multipleNodeList test, value does not match\n");
            break;
        }
        curr = curr->next;
    }

    if (curr) {
        printf("Failed: multipleNodeList test, number of nodes wrong\n");
        return 1;
    }

    if (!failed) {
        printf("Passed: multipleNodeList test\n");
    }
    
    freeList(lp);

    return failed;
}

int testConcatenateNonEmptyList1AndEmptyList2() {
    // arrange
    List *lp1 = initList();
    List *lp2 = initList();

    // act
    add(lp1,0);
    concatenateTwoList(lp1, lp2);

    // assert
    int failed = 0;

    if (lp1->head->val == 0 && !lp1->tail->next) {
        printf("Passed: ConcatenateNonEmptyList1AndEmptyList2 test\n");
    } else {
        failed = 1;
        printf("Failed: ConcatenateNonEmptyList1AndEmptyList2 test\n");
    }
    freeList(lp1);
    free(lp2);

    return failed;
}

int testConcatenateEmptyList1AndNonEmptyList2() {
    // arrange
    List *lp1 = initList();
    List *lp2 = initList();

    // act
    add(lp2,0);
    concatenateTwoList(lp1, lp2);

    // assert
    int failed = 0;

    if (lp1->head->val == 0 && !lp1->tail->next) {
        printf("Passed: concatenateEmptyList1AndNonEmptyList2 test\n");
    } else {
        failed = 1;
        printf("Failed: concatenateEmptyList1AndNonEmptyList2 test\n");
    }
    freeList(lp1);
    free(lp2);

    return failed;
}

int testConcatenateNonEmptyLists() {
    // arrange
    List *lp1 = initList();
    List *lp2 = initList();

    // act
    add(lp1,0);
    add(lp1,1);
    add(lp2,2);
    add(lp2,3);
    concatenateTwoList(lp1, lp2);

    // assert
    int failed = 0;
    Node *curr = lp1->head;
    for (int i = 0; i < 4; i++) {
        if (curr->val != i) {
            printf("Failed: ConcatenateNonEmptyLists test\n");
            return 1;
        }
        curr = curr->next;
    }

    if (curr) {
        printf("Failed: ConcatenateNonEmptyLists test\n");
        failed = 1;
    }

    freeList(lp1);
    free(lp2);

    return failed;
}
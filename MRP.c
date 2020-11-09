#include "MRP.h"

int numberOfChildren(interval* it) {
    child *curr = it->childrenList;
    int count = 0;
    while(curr) {
        curr = curr->next_sibling;
        count += 1;
    }
    return count;
}

void printSA(uint_t *SA, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", SA[i]);
    }
    printf("\n");
}

void printPositionSet(List *positionSet[NUMBER_OF_POSSIBLE_LEFT_CHARACTERS]) {
    for (int i = 0; i < NUMBER_OF_POSSIBLE_LEFT_CHARACTERS; i++) {
        List *l = positionSet[i];
        if(l) {
            int size = 0;
            Node* curr = l->head;
            if (curr) {
                printf("%d: ", i);
            }
            while(curr) {
                printf("%d,", curr->val);
                curr = curr->next;
                size++;
            }
            if (size != 0) {
                printf(" size: %d\n", size);
            }
        }
        

    }
}

void printChild(child* c) {
    printf("child(lb=%d, rb=%d)\n", c->lb, c->rb);
    printf("child posistion set\n");
    printPositionSet(c->positionSet);
}

void printInterval(interval* it, int printChildrenFlag) {
    if (!it) {
        printf("interval is NULL\n");
        return;
    }

    int numChildren = numberOfChildren(it);
    printf("interval(lb=%d, rb=%d, lcp=%d), number of children: %d\n", it->lb, it->rb, it->lcp, numChildren);
    printf("position set:\n");
    printPositionSet(it->positionSet);
    if (printChildrenFlag) {
        child* curr = it->childrenList;
        for(int i = 0; i < numChildren; i++) {
            printf("child %d\n", i);
            printChild(curr);
            curr = curr->next_sibling;
        }
    }
} 

void printStack(interval **stack, int length) {
    printf("printing stack...\n");
    printf("number of element in stack %d\n", length);
    for(int i = 0; i < length; i++) {
        printf("stack element %d\n", i);
        printInterval(stack[i],1);
    }
}

void printList(List *l) {
    Node* curr = l->head;
    while(curr) {
        printf("%d->", curr->val);
        curr = curr->next;
    }
    printf("null\n");
}

child* initChildAllArgs(int lb, int rb, List *positionSet[NUMBER_OF_POSSIBLE_LEFT_CHARACTERS]) {

    child *c = malloc(sizeof(child));
    c->lb = lb;
    c->rb = rb;
    for (int i = 0; i < NUMBER_OF_POSSIBLE_LEFT_CHARACTERS; i++) {
        c->positionSet[i] = positionSet[i];
    }
    c->next_sibling = NULL;
    return c;
}

void addChild(interval* interval, child* newChild) {
    if (interval->childrenList) {
        child *curr = interval->childrenList;
        while(curr->next_sibling) {
            curr = curr->next_sibling;
        }
        curr->next_sibling = newChild;
    } else {
        interval->childrenList = newChild;
    }
}

interval* initInterval() {
    interval* it = malloc(sizeof(interval));
    it->lcp = 0;
    it->lb = 0;
    it->rb = -1;
    it->childrenList = NULL;
    for (int i = 0; i < NUMBER_OF_POSSIBLE_LEFT_CHARACTERS; i++) {
        it->positionSet[i] = initList();
    }
    return it;
}



void freeChild(child *c, int freePositionSetFlag) {

    for (int i = 0; i < NUMBER_OF_POSSIBLE_LEFT_CHARACTERS; i++) {
        if (freePositionSetFlag) {
            freeList(c->positionSet[i]);
        } else {
            free(c->positionSet[i]);
        }
    }
    free(c);
}

void freeInterval(interval* it, int freePositionSetFlag) {
    if (freePositionSetFlag) {
        for (int i = 0; i < NUMBER_OF_POSSIBLE_LEFT_CHARACTERS; i++) {
            freeList(it->positionSet[i]);
        } 
    }
    
    child *curr = it->childrenList;
    while(curr) {
        child *temp = curr;            
        curr = curr->next_sibling;
        freeChild(temp, freePositionSetFlag);
    }

    free(it);
}

void freeMRPList(mrpList *ml) {
    free(ml->mrps);
    free(ml);
}

void addToPosistionSet(int indexInSequence, interval* lastInterval, uint_t *SA, char* BWT) {
    if (SA[indexInSequence] == 0) {
        add(lastInterval->positionSet[0], 0);
    } else {                    
        add(lastInterval->positionSet[charToIndex(BWT[indexInSequence])], SA[indexInSequence]);
    }
}

void mrpsFromTwoLists(List *l1, List *l2, mrp* result, int* mrpCount, int_t lcp, int option, unsigned long n, 
                      unsigned long middleSeparatorIdx) {
    Node *curr1 = l1->head;
    while(curr1) {
        uint_t temp1 = curr1->val;
        Node *curr2 = l2->head;
        while(curr2) {
            uint_t temp2 = curr2->val;
                    
            uint_t val1 = temp1 < temp2 ? temp1 : temp2;
            uint_t val2 = temp1 < temp2 ? temp2 : temp1;

            if (option == 0) {
                mrp newMRP = {val1, val2, lcp};
                result[mrpCount[0]] = newMRP;
                mrpCount[0]++;
            } else if (option == 1) {
                if (val1 < middleSeparatorIdx && val2 > middleSeparatorIdx) {
                    mrp newMRP = {val1, val2, lcp};
                    result[mrpCount[0]] = newMRP;
                    mrpCount[0]++;
                }
            } else if (option == 2) {
                if (val1 < middleSeparatorIdx && val2 > middleSeparatorIdx) {
                    val2 = getCorresondingIndexFromConcatenatedInvertedSequenceLeft(val2, lcp, middleSeparatorIdx);
                    uint_t newVal1 = val1 < val2 ? val1 : val2;
                    uint_t newVal2 = val1 < val2 ? val2 : val1;
                    mrp newMRP = {newVal1, newVal2, lcp};
                    result[mrpCount[0]] = newMRP;
                    mrpCount[0]++;
                }   
            } else if (option == 3) {
                if (val1 < middleSeparatorIdx && val2 > middleSeparatorIdx) {
                    val2 = getCorresondingIndexFromConcatenatedInvertedSequenceRight(val2, lcp, n-1);
                    mrp newMRP = {val1, val2, lcp};
                    result[mrpCount[0]] = newMRP;
                    mrpCount[0]++;
                }
            }
            curr2 = curr2->next;
        }
        curr1 = curr1->next;
    }
}

void freeChildrenList(interval* itv) {
    child* curr = itv->childrenList;
    while(curr) {
        child* next = curr->next_sibling;
        free(curr);
        curr = next;
    }
}

void outputMRPWithOnlyIntervalPositionSet(interval* lastInterval, mrp* result, int* mrpCount, int option, unsigned long n,
                                          unsigned long middleSeparatorIdx) {
    int_t lcp = lastInterval->lcp;
    for (int i = 0; i < NUMBER_OF_POSSIBLE_LEFT_CHARACTERS - 1; i++) {
        for (int j = i+1; j < NUMBER_OF_POSSIBLE_LEFT_CHARACTERS; j++) {
            mrpsFromTwoLists(lastInterval->positionSet[i], lastInterval->positionSet[j], result, mrpCount, lcp,
                             option, n, middleSeparatorIdx);
        }
    }
}

void mergeChildPositionsIntoLastIntervalAndOutputMRP(interval* lastInterval, mrp* result, int* mrpCount, int option, 
                                                     unsigned long n, unsigned long middleSeparatorIdx) {
    child* currChild = lastInterval->childrenList;
    int_t lcp = lastInterval->lcp;
    while(currChild) {
        for (int i = 0; i < NUMBER_OF_POSSIBLE_LEFT_CHARACTERS; i++) {
            for (int j = 0; j < NUMBER_OF_POSSIBLE_LEFT_CHARACTERS; j++) {
                if (i == j) { continue; }
                mrpsFromTwoLists(lastInterval->positionSet[i], currChild->positionSet[j], result, mrpCount, lcp, 
                                 option, n, middleSeparatorIdx);
            }
        }

        for (int i = 0; i < NUMBER_OF_POSSIBLE_LEFT_CHARACTERS; i++) {
            concatenateTwoList(lastInterval->positionSet[i], currChild->positionSet[i]);
        }

        currChild = currChild->next_sibling;
    }
}


void process(uint_t *SA, char* BWT, interval* lastInterval, mrp* result, int* mrpCount, int option, unsigned long n, 
             unsigned long middleSeparatorIdx) {
    int lb = lastInterval->lb;
    int rb = lastInterval->rb;
    int_t lcp = lastInterval->lcp;

    int numberOfNoneLeafChildren = numberOfChildren(lastInterval);

    if (numberOfNoneLeafChildren == 0) {
        // all children are leaves in [lb, rb] need to be added to position set
        for (int i = lb; i <= rb; i++) {
            addToPosistionSet(i, lastInterval, SA, BWT);
        }
    } else {
        // some non-leaf child exists => add indices in [lb,rb] not covered by non-leaf child into the position set
        int childBoundaries[numberOfNoneLeafChildren][2];
        int itr = 0;
        child *tempChild = lastInterval->childrenList;
        while(tempChild) {
            childBoundaries[itr][0] = tempChild->lb;
            childBoundaries[itr][1] = tempChild->rb;
            itr++;
            tempChild = tempChild->next_sibling;
        }

        sortTuples(childBoundaries, numberOfNoneLeafChildren);

        // add leaves in [lb, first_child_lb) to position set
        for (int i = lb; i < childBoundaries[0][0]; i++) { 
            addToPosistionSet(i, lastInterval, SA, BWT); 
        }
        // add leaves in (last_child_rb, rb], first_child_lb) to posistion set
        for (int i = childBoundaries[numberOfNoneLeafChildren-1][1]+1; i <= rb; i++) { 
            addToPosistionSet(i, lastInterval, SA, BWT); 
        }

        // add the rest to position set
        if (numberOfNoneLeafChildren >= 2) {
            for (int i = 1; i < numberOfNoneLeafChildren; i++) {
                int prevEnd = childBoundaries[i-1][1];
                int currStart = childBoundaries[i][0];
                for (int j = prevEnd+1; j < currStart; j++) {
                    addToPosistionSet(j, lastInterval, SA, BWT);
                }
            }
        }
    }

    // output MRP and merge 
    outputMRPWithOnlyIntervalPositionSet(lastInterval, result, mrpCount, option, n, middleSeparatorIdx);
    mergeChildPositionsIntoLastIntervalAndOutputMRP(lastInterval, result, mrpCount, option, n, middleSeparatorIdx); 
}

/**
 *  Note: n should be strlen(seq) + 1
 * 
 *  option = 0: search all mrps in sequence
 *  option = 1: search all mrps (p1,p2,l) in seqeunce=s1#s2, so that p1 in s1 and p2 in s2
 *  option = 2: search all inverted mrps in s given sequence = s#s'
 *  option = 4: search all inverted mrps in seqeunce=s1#s2, so that p1 in s1 and p2 in s2
 */
mrpList* searchMRPWithOption(const char* sequence, unsigned long n, uint_t threshold, int option, unsigned long middleSeparatorIdx) {

    int MAX_MRP_ALLOWED = n;
    mrp* mrps = malloc(sizeof(mrp)*MAX_MRP_ALLOWED);
    int mrpCount[1];
    mrpCount[0] = 0;

    uint_t *SA = malloc(n * sizeof(uint_t));
    int_t *LCP = malloc(n * sizeof(int_t));
    char *BWT = malloc(n * sizeof(char));

    reformatSuffixArrays(sequence, SA, LCP, BWT, n);

    // mrpList *dummy = malloc(sizeof(mrpList));
    // dummy->size = 0;
    // return dummy;

    // bottom up traversal
    // use an array of interval pointers to simulate stack
    interval **stack = malloc(sizeof(interval*)*2*n); 
    // for(int i = 0; i < 2*n; i++) {
    //     stack[i] = NULL;
    // }

    interval* lastInterval = NULL;

    // add the first interval
    stack[0] = initInterval();
    int topElementIndex = 0;
    
    for (int i = 0; i < n; i++) {
        int lb = i-1;

        while (LCP[i] < stack[topElementIndex]->lcp) {

            stack[topElementIndex]->rb = i - 1;
            lastInterval = stack[topElementIndex];
            stack[topElementIndex] = NULL;
            topElementIndex--;

            if (lastInterval->lcp < threshold) {
                freeInterval(lastInterval, 1);
                lastInterval = NULL;
                continue;
            }

            if (lastInterval->lcp >= threshold) {
                process(SA, BWT, lastInterval, mrps, mrpCount, option, n, middleSeparatorIdx);
            } 

            lb = lastInterval->lb;

            if (LCP[i] <= stack[topElementIndex]->lcp) {
                child* newChild = initChildAllArgs(lastInterval->lb, lastInterval->rb, lastInterval->positionSet);
                addChild(stack[topElementIndex], newChild);
                freeInterval(lastInterval,0);
                lastInterval = NULL;
            }
        }

        if (LCP[i] > stack[topElementIndex]->lcp) {
            if (lastInterval) {
                if (lastInterval->lcp < threshold) {
                    freeInterval(lastInterval,1);
                    lastInterval = NULL;
                    continue;
                }

                child* newChild = initChildAllArgs(lastInterval->lb, lastInterval->rb, lastInterval->positionSet);
                
                interval* newInterval = initInterval();
                newInterval->lcp = LCP[i];
                newInterval->lb = lb;
                newInterval->childrenList = newChild;

                topElementIndex++;
                stack[topElementIndex] = newInterval;

                freeInterval(lastInterval, lastInterval->lcp < threshold);
                lastInterval = NULL;
            } else {

                interval* newInterval = initInterval();
                newInterval->lcp = LCP[i];
                newInterval->lb = lb;

                topElementIndex++;
                stack[topElementIndex] = newInterval;
            }
        }
        
    }

    for(int i = topElementIndex; i >= 0; i--) {
        freeInterval(stack[i],1);
    }

    free(SA);
    free(LCP);
    free(BWT);
    free(stack);

    mrpList *result = malloc(sizeof(mrpList));
    // mrp* resultMRPs = malloc(sizeof(mrp)*mrpCount[0]);
    // result->size = mrpCount[0];
    // for (int i = 0; i < mrpCount[0]; i++) {
    //     resultMRPs[i].p1 = mrps[i].p1;
    //     resultMRPs[i].p2 = mrps[i].p2;
    //     resultMRPs[i].length = mrps[i].length;
    // }
    result->mrps = mrps;
    result->size = mrpCount[0];
    // free(mrps);

    return result;
}


mrpList* searchMRPInSingleSequence(const char* sequence, unsigned long n, uint_t threshold) {
    return searchMRPWithOption(sequence, n, threshold, 0, -1);
}

mrpList* searchMRPInTwoSequences(const char* sequence, unsigned long n, uint_t threshold, unsigned long firstSequenceLength) {
    mrpList* ml = searchMRPWithOption(sequence, n, threshold, 1, firstSequenceLength);
    return ml;
}

mrpList* searchInvertedMRPInSingleSequence(const char* sequence, unsigned long n, uint_t threshold) {
    char* concatenatedInvertedSequence = malloc(sizeof(char)*(2*n+2));
    getConcantenatedInvertedSequence(sequence, n-1, concatenatedInvertedSequence);
    unsigned long n2 = strlen(concatenatedInvertedSequence) + 1;
    mrpList* resultWithDuplicates = searchMRPWithOption(concatenatedInvertedSequence, n2, threshold, 2, n-1);
    free(concatenatedInvertedSequence);

    hashset_t set = hashset_create();
    mrpList* ml = malloc(sizeof(mrpList));
    mrp* resultMRPs = malloc(sizeof(mrp)*resultWithDuplicates->size);
    int size = 0;
    for (int i = 0; i < resultWithDuplicates->size; i++) {
        mrp m = resultWithDuplicates->mrps[i];
        if (!hashset_is_member(set, m)) {
            resultMRPs[size] = m;
            size++;
            hashset_add(set,m);
        }
    }
    ml->mrps = resultMRPs;
    ml->size = size;
    freeMRPList(resultWithDuplicates);
    hashset_destroy(set);
    return ml;
}

mrpList* searchInvertedMRPInTwoSequences(const char* seq1, const char* seq2, unsigned long n1, unsigned long n2, 
            uint_t threshold) { 
    char* seq = malloc(sizeof(char)*(n1+n2+2));
    getConcantenatedInvertedSequenceDiff(seq1, seq2, n1, n2, seq);
    mrpList* ml = searchMRPWithOption(seq, n1+n2+2, threshold, 3, n1);

    free(seq);
    return ml;
}

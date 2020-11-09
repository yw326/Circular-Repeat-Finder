#include "TestInvertedCircleRepeatFinder.h"

int testFindInvertedCircleRepeatedPairs() {
    // arrange
    long sequenceLength = getDNASequenceLengthFromFile("NC_021868.txt");
    const char* seq = malloc(sizeof(char)*(sequenceLength+1));
    getDNASequenceFromFile("NC_021868.txt", seq);
    long n = sequenceLength+1;
    int l1 = 10;
    int l2 = 5;
    int L = 30;
    double alpha = 0.1;

    // act
    findInvertedCircleRepeatedPairs(seq, n, l1, l2, L, alpha, "result/invertedCircleRepeatIndices.txt", 0);

    // assert
    int failed = 0;
    free(seq);

    return failed;
}
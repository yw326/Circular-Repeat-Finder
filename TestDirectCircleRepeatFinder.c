#include "TestDirectCircleRepeatFinder.h"

int testFindDirectCircleRepeatedPairs() {
    // arrange
    long sequenceLength = getDNASequenceLengthFromFile("NC_021868.txt");
    const char* seq = malloc(sizeof(char)*(sequenceLength+1));
    getDNASequenceFromFile("NC_021868.txt", seq);
    long n = sequenceLength+1;
    int l1 = 10;
    int l2 = 5;
    int L = 30;
    float alpha = 0.1;

    // act
    findDirectCircleRepeatedPairs(seq, NULL, n, 0, l1, l2, L, alpha, "result/DirectCircleRepeatIndices.txt", 0, NULL);

    // assert
    int failed = 0;
    free(seq);

    return failed;
}

int testFindDirectCircleRepeatedPairsSubstrings1() {
    // arrange

    // first level mrp ATTCGGCTTT
    // second level mrp CGGT

    // check printed results have
    // firstRightExtension = ACCGGT, secondLeftExtension = CGGTTC
    // A1 = CG, A2 = AC, A3 = AC 
    // B1 = TT, B2 = TC, B3 = TC

    const char* seq = "ACCTTTCGATTCGGCTTTACCGGTACGAATCCTAATGGCAGTTCGGTTCATTCGGCTTTTCATCGATT";
    long n = strlen(seq)+1;
    int l1 = 10;
    int l2 = 4;
    int L = 6;
    float alpha = 0.1;

    // act
    findDirectCircleRepeatedPairs(seq, NULL, n, 0, l1, l2, L, alpha, "result/DirectCircleRepeatIndices.txt", 1, NULL);

    // assert
    int failed = 0;

    return failed;
}

int testFindDirectCircleRepeatedPairsSubstrings2() {
    // arrange

    // first level mrp ATTCGGCTTT
    // second level mrp CGGT

    // check printed results have
    // firstLeftExtension = CGGTCC, secondRightExtension = TACGGT
    // A1 = CG, A2 = CC, A3 = AC 
    // B1 = CA, B2 = TA, B3 = GG

    const char* seq = "TCGCGGTCCATTCGGCTTTACTGGTTCCTCCATACAATTCGGCTTTTACGGTGG";
    long n = strlen(seq)+1;
    int l1 = 10;
    int l2 = 4;
    int L = 6;
    float alpha = 0.1;

    // act
    findDirectCircleRepeatedPairs(seq, NULL, n, 0, l1, l2, L, alpha, "result/DirectCircleRepeatIndices.txt", 1, NULL);

    // assert
    int failed = 0;

    return failed;
}
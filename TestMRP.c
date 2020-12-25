#include "TestMRP.h"

int twoMRPEqual(mrp m1, mrp m2) {
    return m1.p1 == m2.p1 && m1.p2 == m2.p2 && m1.length == m2.length;
}

int isMRPInResult(mrp m, mrpList *ml) {
    for (int i = 0; i < ml->size; i++) {
        if (twoMRPEqual(m, ml->mrps[i]) ) {
            return 1;
        }
    }
    return 0;
}

int testSearchMRP1() {
    // arrange
    const char* testStr = "ACAAACATAT";
    int threshold = 3;
    long n = strlen(testStr);

    // act
    mrpList *ml = searchMRPInSingleSequence(testStr, n, threshold);

    // assert
    int failed = 0;
    int expectedSize = 1;
    mrp expectedMRP = {0,4,3};

    if (ml->size != expectedSize) {
        printf("Failed: testSearchMRP1, expected the number of MRPs to be %d, but found %d\n", expectedSize, ml->size);
        return 1;
    }

    if (!isMRPInResult(expectedMRP, ml) ) {
        printf("Failed: testSearchMRP1, mrp (%d,%d,%d) is not found\n", expectedMRP.p1, expectedMRP.p2, expectedMRP.length);
        return 1;
    }
    freeMRPList(ml);
    printf("Passed: testSearchMRP1\n");

    return failed;
}

int testSearchMRP2() {
    // arrange
    const char* testStr = "AGAAGACTATGAACCTGACGATTACGACGA";
    int threshold = 4;
    long n = strlen(testStr);

    // act
    mrpList *ml = searchMRPInSingleSequence(testStr, n, threshold);

    // assert
    int failed = 0;
    int expectedSize = 3;
    mrp expectedMRP1 = {23,26,4};
    mrp expectedMRP2 = {17,23,4};
    mrp expectedMRP3 = {16,25,5};

    if (ml->size != expectedSize) {
        printf("Failed: testSearchMRP2, expected the number of MRPs to be %d, but found %d\n", expectedSize, ml->size);
        return 1;
    }

    if (!isMRPInResult(expectedMRP1, ml) ) {
        printf("Failed: testSearchMRP2, mrp (%d,%d,%d) is not found\n", expectedMRP1.p1, expectedMRP1.p2, expectedMRP1.length);
        return 1;
    }

    if (!isMRPInResult(expectedMRP2, ml) ) {
        printf("Failed: testSearchMRP2, mrp (%d,%d,%d) is not found\n", expectedMRP2.p1, expectedMRP2.p2, expectedMRP2.length);
        return 1;
    }

    if (!isMRPInResult(expectedMRP3, ml) ) {
        printf("Failed: testSearchMRP2, mrp (%d,%d,%d) is not found\n", expectedMRP3.p1, expectedMRP3.p2, expectedMRP3.length);
        return 1;
    }
    freeMRPList(ml);

    printf("Passed: testSearchMRP2\n");
    return failed;
}

int testSearchMRP3() {
    // arrange
    long sequenceLength = getDNASequenceLengthFromFile("NC_021868.txt");
    const char* seq = malloc(sizeof(char)*(sequenceLength+1));
    getDNASequenceFromFile("NC_021868.txt", seq);
    int threshold = 10;
    long n = sequenceLength;

    // act
    mrpList *ml = searchMRPInSingleSequence(seq, n, threshold);

    FILE *fp;
    fp = fopen("3481.txt", "w+");
    for (int i = 0; i < ml->size; i++) {
        fprintf(fp, "%d %d %d\n", ml->mrps[i].p1, ml->mrps[i].p2, ml->mrps[i].length);
    }
    fclose(fp);

    // assert
    int failed = 0;
    int expectedSize = 3640;

    if (ml->size != expectedSize) {
        printf("Failed: testSearchMRP3, expected the number of MRPs to be %d, but found %d\n", expectedSize, ml->size);
        return 1;
    }
    freeMRPList(ml);
    free(seq);

    printf("Passed: testSearchMRP3\n");
    return failed;
}

int testSearchMRP4() {
    // arrange
    long sequenceLength = getDNASequenceLengthFromFile("chrY_prefiltered.mask");
    const char* seq = malloc(sizeof(char)*(sequenceLength+1));
    getDNASequenceFromFile("chrY_prefiltered.mask", seq);
    int threshold = 40;
    long n = sequenceLength;

    // act
    mrpList *ml = searchMRPInSingleSequence(seq, n, threshold);

    // assert
    int failed = 0;
    int expectedSize = 544030;

    if (ml->size != expectedSize) {
        printf("Failed: testSearchMRP4, expected the number of MRPs to be %d, but found %d\n", expectedSize, ml->size);
        return 1;
    }
    freeMRPList(ml);
    free(seq);

    printf("Passed: testSearchMRP4\n");
    return failed;
}

int testSearchMRPInTwoSequences() {
    // arrange
    const char* sequence1 = "AGAAGAC";
    const char* sequence2 = "ATTGACCT";
    int threshold = 3;
    long n1 = strlen(sequence1);
    long n2 = strlen(sequence2);

    // act
    mrpList *ml = searchMRPInTwoSequences(sequence1, sequence2, n1, n2, threshold);

    // assert
    int failed = 0;
    int expectedSize = 1;
    mrp expectedMRP = {4,11,3};

    if (ml->size != expectedSize) {
        printf("Failed: testSearchMRPOption1, expected the number of MRPs to be %d, but found %d\n", expectedSize, ml->size);
        return 1;
    }

    if (!isMRPInResult(expectedMRP, ml) ) {
        printf("Failed: testSearchMRPOption1, mrp (%d,%d,%d) is not found\n", expectedMRP.p1, expectedMRP.p2, expectedMRP.length);
        return 1;
    }

    freeMRPList(ml);

    printf("Passed: testSearchMRPOption1\n");
    return failed;
}

// GAAATCACTTCCCTAGTAACTAAGACAGAT#CTCATTAACCTACCGTTATAAAGGTGAAAT

int testSearchMRPInTwoSequences2() {
    // arrange
    const char* sequence1 = "GAAATCACTTCCCTAGTAACTAAGACAGAT";
    const char* sequence2 = "CTCATTAACCTACCGTTATAAAGGTGAAAT";
    int threshold = 5;
    long n1 = strlen(sequence1);
    long n2 = strlen(sequence2);

    // act
    mrpList *ml = searchMRPInTwoSequences(sequence1, sequence2, n1, n2, threshold);

    // assert
    int failed = 0;
    int expectedSize = 1;
    // mrp expectedMRP = {4,11,3};

    if (ml->size != expectedSize) {
        printf("Failed: testSearchMRPInTwoSequences2, expected the number of MRPs to be %d, but found %d\n", expectedSize, ml->size);
        return 1;
    }
    // printf("%d,%d,%d\n",ml->mrps[0].p1,ml->mrps[0].p2,ml->mrps[0].length);

    // if (!isMRPInResult(expectedMRP, ml) ) {
    //     printf("Failed: testSearchMRPOption1, mrp (%d,%d,%d) is not found\n", expectedMRP.p1, expectedMRP.p2, expectedMRP.length);
    //     return 1;
    // }

    freeMRPList(ml);

    printf("Passed: testSearchMRPInTwoSequences2\n");
    return failed;
}

int testSearchInvertedMRPInSingleSequence() {
    // arrange

    // inverse of AAGAC is GTCTT
    const char* testStr = "AGAAGACTATGCGTCTTA";
    // const char* testStr = "AGAAGACTATGCGTCTTA#TAAGACGCATAGTCTTCT";
    int threshold = 5;
    long n = strlen(testStr)+1;

    // act
    mrpList *ml = searchInvertedMRPInSingleSequence(testStr, n, threshold);

    // printf("%d,%d,%d\n", ml->mrps[0].p1, ml->mrps[0].p2, ml->mrps[0].length);
    // printf("%d,%d,%d\n", ml->mrps[1].p1, ml->mrps[1].p2, ml->mrps[1].length);

    // assert
    int failed = 0;
    int expectedSize = 1;
    mrp expectedMRP1 = {2,12,5};

    if (ml->size != expectedSize) {
        printf("Failed: testSearchInvertedMRPInSingleSequence, expected the number of MRPs to be %d, but found %d\n", expectedSize, ml->size);
        return 1;
    }

    if (!isMRPInResult(expectedMRP1, ml) ) {
        printf("Failed: testSearchInvertedMRPInSingleSequence, mrp (%d,%d,%d) is not found\n", expectedMRP1.p1, expectedMRP1.p2, expectedMRP1.length);
        return 1;
    }

    freeMRPList(ml);

    printf("Passed: testSearchInvertedMRPInSingleSequence\n");
    return failed;
}

int testSearchInvertedMRPInTwoSequences() {
    // arrange

    // inverse of AAGAC is GTCTT
    const char* seq1 = "AAAGACA";
    const char* seq2 = "AGTCTTC";
    int threshold = 5;
    long n1 = strlen(seq1);
    long n2 = strlen(seq2);

    // act
    mrpList *ml = searchInvertedMRPInTwoSequences(seq1, seq2, n1, n2, threshold);

    // assert
    int failed = 0;
    int expectedSize = 1;
    mrp expectedMRP1 = {1,1,5};

    if (ml->size != expectedSize) {
        printf("Failed: searchInvertedMRPInTwoSequences, expected the number of MRPs to be %d, but found %d\n", expectedSize, ml->size);
        return 1;
    }

    if (!isMRPInResult(expectedMRP1, ml) ) {
        printf("Failed: searchInvertedMRPInTwoSequences, mrp (%d,%d,%d) is not found\n", expectedMRP1.p1, expectedMRP1.p2, expectedMRP1.length);
        return 1;
    }

    freeMRPList(ml);

    printf("Passed: searchInvertedMRPInTwoSequences\n");
    return failed;
}


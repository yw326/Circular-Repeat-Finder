#include "TestDirectCircleRepeatFinder.h"

int testFindDirectCircleRepeatedPairs() {
    // arrange
    long sequenceLength = getDNASequenceLengthFromFile("TestData/NC_021868.txt");
    const char* seq = malloc(sizeof(char)*(sequenceLength+1));
    getDNASequenceFromFile("TestData/NC_021868.txt", seq);
    const char* outputFileName = "result/DirectCircleRepeatIndices.txt";
    long n = sequenceLength;
    int l1 = 10;
    int l2 = 5;
    int L = 30;
    float alpha = 0.1;

    // act
    findDirectCircleRepeatedPairs(seq, NULL, n, 0, l1, l2, L, alpha, outputFileName, 0, NULL);

    // assert
    int failed = 1;
    int numberOfTokenExtracting = 6;
    const char* expectedResultPath = "TestData/NC_021868_direct_result.txt";

    int numberCircleRpeats = countLinesInFile(outputFileName);
    int expectedCount = countLinesInFile(expectedResultPath);

    if (numberCircleRpeats != expectedCount) {
        printf("testFindDirectCircleRepeatedPairs failed. Number of circular repeats incorrect. Expected %d, got %d\n", expectedCount, numberCircleRpeats);
        return failed;
    }

    char*** result = readResultIdxFile(outputFileName, numberOfTokenExtracting);
    char*** expectedResult = readResultIdxFile(expectedResultPath, numberOfTokenExtracting);
    
    int missedElementsCount = resultDifference(result, expectedResult, numberOfTokenExtracting, numberCircleRpeats, expectedCount);
    int moreElementsCount = resultDifference(expectedResult, result, numberOfTokenExtracting, expectedCount, numberCircleRpeats);
    
    if (missedElementsCount != 0) {
        printf("testFindDirectCircleRepeatedPairs failed. Missed %d \n", missedElementsCount);
    }

    if (moreElementsCount != 0) {
        printf("testFindDirectCircleRepeatedPairs failed. %d more than expected\n", moreElementsCount);
    }

    if (missedElementsCount != 0 || moreElementsCount != 0) {
        freeReadFileResult(result, numberCircleRpeats, numberOfTokenExtracting);
        freeReadFileResult(expectedResult, expectedCount, numberOfTokenExtracting);
        return failed;
    }

    freeReadFileResult(result, numberCircleRpeats, numberOfTokenExtracting);
    freeReadFileResult(expectedResult, expectedCount, numberOfTokenExtracting);
    free(seq);

    printf("Passed: testFindDirectCircleRepeatedPairs\n");

    return 0;
}

int testFindDirectCircleRepeatedPairsPartition() {
    // arrange & act
    // To obtain data, run ./CRPFinder direct --partition TestData/NC_021868_split_5 1-15 --l1 10 --l2 5 --L 30

    // assert
    int failed = 1;
    int numberOfTokenExtracting = 6;
    const char* expectedResultPath = "TestData/NC_021868_direct_result.txt";
    const char* outputDir = "TestData/NC_021868_split_5/1-15-direct";

    int numberCircleRpeats = countLinesInDir(outputDir);
    int expectedCount = countLinesInFile(expectedResultPath);

    if (numberCircleRpeats != expectedCount) {
        printf("testFindDirectCircleRepeatedPairsPartition failed. Number of circular repeats incorrect. Expected %d, got %d\n", expectedCount, numberCircleRpeats);
        return failed;
    }

    char*** result = readResultDir(outputDir, numberOfTokenExtracting);
    char*** expectedResult = readResultIdxFile(expectedResultPath, numberOfTokenExtracting);
    
    int missedElementsCount = resultDifference(result, expectedResult, numberOfTokenExtracting, numberCircleRpeats, expectedCount);
    int moreElementsCount = resultDifference(expectedResult, result, numberOfTokenExtracting, expectedCount, numberCircleRpeats);
    
    if (missedElementsCount != 0) {
        printf("testFindDirectCircleRepeatedPairsPartition failed. Missed %d \n", missedElementsCount);
    }

    if (moreElementsCount != 0) {
        printf("testFindDirectCircleRepeatedPairsPartition failed. %d more than expected\n", moreElementsCount);
    }

    if (missedElementsCount != 0 || moreElementsCount != 0) {
        freeReadFileResult(result, numberCircleRpeats, numberOfTokenExtracting);
        freeReadFileResult(expectedResult, expectedCount, numberOfTokenExtracting);
        return failed;
    }

    freeReadFileResult(result, numberCircleRpeats, numberOfTokenExtracting);
    freeReadFileResult(expectedResult, expectedCount, numberOfTokenExtracting);

    printf("Passed: testFindDirectCircleRepeatedPairsPartition\n");
    return 0;
}






// int testFindDirectCircleRepeatedPairsSubstrings1() {
//     // arrange

//     // first level mrp ATTCGGCTTT
//     // second level mrp CGGT

//     // check printed results have
//     // firstRightExtension = ACCGGT, secondLeftExtension = CGGTTC
//     // A1 = CG, A2 = AC, A3 = AC 
//     // B1 = TT, B2 = TC, B3 = TC

//     const char* seq = "ACCTTTCGATTCGGCTTTACCGGTACGAATCCTAATGGCAGTTCGGTTCATTCGGCTTTTCATCGATT";
//     long n = strlen(seq)+1;
//     int l1 = 10;
//     int l2 = 4;
//     int L = 6;
//     float alpha = 0.1;

//     // act
//     findDirectCircleRepeatedPairs(seq, NULL, n, 0, l1, l2, L, alpha, "result/DirectCircleRepeatIndices.txt", 1, NULL);

//     // assert
//     int failed = 0;

//     return failed;
// }

// int testFindDirectCircleRepeatedPairsSubstrings2() {
//     // arrange

//     // first level mrp ATTCGGCTTT
//     // second level mrp CGGT

//     // check printed results have
//     // firstLeftExtension = CGGTCC, secondRightExtension = TACGGT
//     // A1 = CG, A2 = CC, A3 = AC 
//     // B1 = CA, B2 = TA, B3 = GG

//     const char* seq = "TCGCGGTCCATTCGGCTTTACTGGTTCCTCCATACAATTCGGCTTTTACGGTGG";
//     long n = strlen(seq)+1;
//     int l1 = 10;
//     int l2 = 4;
//     int L = 6;
//     float alpha = 0.1;

//     // act
//     findDirectCircleRepeatedPairs(seq, NULL, n, 0, l1, l2, L, alpha, "result/DirectCircleRepeatIndices.txt", 1, NULL);

//     // assert
//     int failed = 0;

//     return failed;
// }
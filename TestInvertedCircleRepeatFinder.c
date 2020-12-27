#include "TestInvertedCircleRepeatFinder.h"

int testFindInvertedCircleRepeatedPairs() {
    // arrange
    long sequenceLength = getDNASequenceLengthFromFile("TestData/NC_021868.txt");
    const char* seq = malloc(sizeof(char)*(sequenceLength+1));
    getDNASequenceFromFile("TestData/NC_021868.txt", seq);
    const char* outputFileName = "result/InvertedCircleRepeatIndices.txt";
    long n = sequenceLength;
    int l1 = 10;
    int l2 = 5;
    int L = 30;
    float alpha = 0.1;

    // act
    findInvertedCircleRepeatedPairs(seq, NULL, n, 0, l1, l2, L, alpha, outputFileName, 0, NULL);

    // assert
    int failed = 1;
    int numberOfTokenExtracting = 6;
    const char* expectedResultPath = "TestData/NC_021868_inverted_result.txt";

    int numberCircleRpeats = countLinesInFile(outputFileName);
    int expectedCount = countLinesInFile(expectedResultPath);

    if (numberCircleRpeats != expectedCount) {
        printf("testFindInvertedCircleRepeatedPairs failed. Number of circular repeats incorrect. Expected %d, got %d\n", expectedCount, numberCircleRpeats);
        return failed;
    }

    char*** result = readResultIdxFile(outputFileName, numberOfTokenExtracting);
    char*** expectedResult = readResultIdxFile(expectedResultPath, numberOfTokenExtracting);
    
    int missedElementsCount = resultDifference(result, expectedResult, numberOfTokenExtracting, numberCircleRpeats, expectedCount);
    int moreElementsCount = resultDifference(expectedResult, result, numberOfTokenExtracting, expectedCount, numberCircleRpeats);
    
    if (missedElementsCount != 0) {
        printf("testFindInvertedCircleRepeatedPairs failed. Missed %d \n", missedElementsCount);
    }

    if (moreElementsCount != 0) {
        printf("testFindInvertedCircleRepeatedPairs failed. %d more than expected\n", moreElementsCount);
    }

    if (missedElementsCount != 0 || moreElementsCount != 0) {
        freeReadFileResult(result, numberCircleRpeats, numberOfTokenExtracting);
        freeReadFileResult(expectedResult, expectedCount, numberOfTokenExtracting);
        return failed;
    }

    freeReadFileResult(result, numberCircleRpeats, numberOfTokenExtracting);
    freeReadFileResult(expectedResult, expectedCount, numberOfTokenExtracting);
    free(seq);

    printf("Passed: testFindInvertedCircleRepeatedPairs\n");

    return 0;
}

int testFindInvertedCircleRepeatedPairsPartition() {
    // arrange & act
    // To obtain data, run ./CRPFinder inverted --partition TestData/NC_021868_split_5 1-15 --l1 10 --l2 5 --L 30

    // assert
    int failed = 1;
    int numberOfTokenExtracting = 6;
    const char* expectedResultPath = "TestData/NC_021868_inverted_result.txt";
    const char* outputDir = "TestData/NC_021868_split_5/1-15-inverted";

    int numberCircleRpeats = countLinesInDir(outputDir);
    int expectedCount = countLinesInFile(expectedResultPath);

    if (numberCircleRpeats != expectedCount) {
        printf("testFindDirectCircleRepeatedPairsPartition failed. Number of circular repeats incorrect. Expected %d, got %d\n", expectedCount, numberCircleRpeats);
        return failed;
    }

    char*** result = readResultDir(outputDir, numberOfTokenExtracting);
    char*** expectedResult = readResultIdxFile(expectedResultPath, numberOfTokenExtracting);
    
    int missedElementsCount = resultDifference(result, expectedResult, numberOfTokenExtracting, numberCircleRpeats, expectedCount);
    if (missedElementsCount != 0) {
        printf("testFindDirectCircleRepeatedPairsPartition failed. Missed %d \n", missedElementsCount);
    }
    
    int moreElementsCount = resultDifference(expectedResult, result, numberOfTokenExtracting, expectedCount, numberCircleRpeats);

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
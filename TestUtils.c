//
//  TestUtils.c
//  mdf-2.0
//
//  Created by Yiming Wang on 2020/7/5.
//

#include "TestUtils.h"

char testString[] = "ATCGTTCG";
char testLoadFileName[] = "test_LoadFile.txt";

int testGetDNASequenceFromFile() {
    // arrange
    int n = getDNASequenceLengthFromFile(testLoadFileName);
    char *seq = malloc(sizeof(char)*(n+1));

    // act
    getDNASequenceFromFile(testLoadFileName, seq);

    // assert
    int failed = 0;
    if (strlen(seq) == 6) {
        printf("Passed: getDNASequenceFromFile length test\n");
    } else {
        printf("Failed: getDNASequenceFromFile length test, expected 6, got %lu \n", strlen(seq));
        failed++;
    }

    if (strcmp(seq, "ATTCCG") == 0) {
        printf("Passed: getDNASequenceFromFile equal test \n");
    } else {
        printf("Failed: getDNASequenceFromFile equal test, expected ATTCCG, got %s \n", seq);
        failed++;
    }

    free(seq);
    
    return failed;
}

int testGetConcantenatedInvertedSequence() {
    // arrange
    const char *givenSeq = "ATTCCG";
    unsigned long n = strlen(givenSeq);
    char *seq = malloc(sizeof(char)*(2*n+2));

    // act 
    getConcantenatedInvertedSequence(givenSeq, n, seq);

    // assert
    int failed = 0;
    if (strlen(seq) == 13) {
        printf("Passed: testGetConcantenatedInvertedSequence length test\n");
    } else {
        printf("Failed: testGetConcantenatedInvertedSequence length test, expected 13, got %lu \n", strlen(seq));
        failed++;
    }

    if (strcmp(seq, "ATTCCG#CGGAAT") == 0) {
        printf("Passed: testGetConcantenatedInvertedSequence equal test \n");
    } else {
        printf("Failed: testGetConcantenatedInvertedSequence equal test, expected ATTCCG, got %s \n", seq);
        failed++;
    }

    free(seq);
    
    return failed;
}

int testGetConcatenatedSequence() {
    // arrange
    const char* seq1 = "ATCCG";
    const char* seq2 = "CGGAATT";
    unsigned long n1 = strlen(seq1);
    unsigned long n2 = strlen(seq2);
    char* seq = malloc(sizeof(char)*(n1+n2+2));

    // act
    getConcatenatedSequence(seq, seq1, seq2, n1, n2);

    // assert
    int failed = 0;
    if (strlen(seq) != n1+n2+1) {
        printf("Failed: testGetConcatenatedSequence length test, expected %d, got %lu \n", n1+n2+1, strlen(seq));
        return 1;
    }

    if (strcmp(seq, "ATCCG#CGGAATT") != 0) {
        printf("Failed: testGetConcatenatedSequence equal test, got %s \n", n1+n2+1, seq);
        return 1;
    }

    printf("Passed: testGetConcatenatedSequence \n");
    free(seq);
    return 0;
}

int testGetReversedSequence() {
    // arrange
    const char *givenSeq = "ATTCCG";
    unsigned long n = strlen(givenSeq);
    char *seq = malloc(sizeof(char)*(n+1));

    // act 
    getReversedSequence(givenSeq, n, seq);

    // assert
    int failed = 0;
    if (strcmp(seq, "GCCTTA") == 0) {
        printf("Passed: testGetReversedSequence equal test \n");
    } else {
        printf("Failed: testGetReversedSequence equal test, expected GCCTTA, got %s \n", seq);
        failed++;
    }

    free(seq);
    
    return failed;
}

int testSubstring() {
    // arrange
    int substringLength = 2;
    char *substr = malloc(sizeof(char)*(substringLength+1));
    
    // act
    substring(testString,1,2, substr); // TC

    // assert
    int failed = 0;
    if (strlen(substr) == substringLength) {
        printf("Passed: substring length test\n");
    } else {
        printf("Failed: substring length test, expected 2, got %lu \n", strlen(substr));
        failed++;
    }

    if (strcmp(substr, "TC") == 0) {
        printf("Passed: substring equal test \n");
    } else {
        printf("Failed: substring equal test, expected TC, got %s \n", substr);
        failed++;
    }

    free(substr);
    
    return failed;
}

int testInvertedSubstring() {
    // arrange
    int substringLength = 3;
    char *rcSubstr = malloc(sizeof(char)*(substringLength+1));
    
    // act
    invertedSubstring(testString,1,3, rcSubstr); // (TCG)' = CGA

    // assert
    int failed = 0;
    if (strlen(rcSubstr) == 3) {
        printf("Passed: testInvertedSubstring length test\n");
    } else {
        printf("Failed: testInvertedSubstring length test, expected 13, got %lu \n", strlen(rcSubstr));
        failed++;
    }

    if (strcmp(rcSubstr, "CGA") == 0) {
        printf("Passed: testInvertedSubstring equal test \n");
    } else {
        printf("Failed: testInvertedSubstring equal test, expected TC, got %s \n", rcSubstr);
        failed++;
    }

    free(rcSubstr);
    
    return failed;
}


int testSortTuples() {
    // arrange

    // 8 12
    // 6 11
    // 9 10
    // 3 8 
    int tuples[4][2];
    int firstCol[4] = {8,6,9,3};
    int secondCol[4] = {12,11,10,8};
    for(int i = 0; i < 4; i++) {
        tuples[i][0] = firstCol[i];
        tuples[i][1] = secondCol[i];
    }

    // act
    sortTuples(tuples, 4);

    // assert

    // expect
    // 3 8
    // 6 11
    // 8 12
    // 9 10
    int expectedFirstCol[4] = {3,6,8,9};
    int expectedSecondCol[4] = {8,11,12,10};
    int failed = 0;

    for(int i = 0; i < 4; i++) { 
        if (expectedFirstCol[i] != tuples[i][0] || expectedSecondCol[i] != tuples[i][1]) {
            printf("Failed: testSortTuples, row %d not correct\n", i);
            return 1;
        }
    }
    printf("Passed: testSortTuples\n");
    return 0;
}

int testGetCorresondingIndexFromConcatenatedInvertedSequenceLeft() {
    // arrange

    // ATTTCGG#CCGAAAT
    // n = 7
    // GAAA => should find TTTC from 1 to 4
    // start = 10
    // length = 4
    // (7*2+1) - 10 - 4

    const char *givenSeq = "ATTTCGG";
    unsigned long n = strlen(givenSeq);
    char *seq = malloc(sizeof(char)*(2*n+2));
    getConcantenatedInvertedSequence(givenSeq, n, seq);
    long startIndex = 10;
    int length = 4;
    long expectedStartIndex = 1;

    // act 
    long resultIndex = getCorresondingIndexFromConcatenatedInvertedSequenceLeft(startIndex, length, n);
    
    // assert
    if (resultIndex != 1) {
        printf("Failed: testGetCorresondingIndexFromConcatenatedInvertedSequence\n");
        return 1;
    }
    printf("Passed: testGetCorresondingIndexFromConcatenatedInvertedSequence\n");
    free(seq);
    return 0;
}

int testGetConcantenatedInvertedSequenceDiff() {
    // arrange
    const char *seq1 = "ATTCCG";
    const char *seq2 = "CGTAA";
    unsigned long n1 = strlen(seq1);
    unsigned long n2 = strlen(seq2);
    char *seq = malloc(sizeof(char)*(n1+n2+2));

    // act 
    getConcantenatedInvertedSequenceDiff(seq1, seq2, n1, n2, seq);

    // assert
    int failed = 0;

    if (strcmp(seq, "ATTCCG#TTACG") == 0) {
        printf("Passed: testGetConcantenatedInvertedSequenceDiff equal test \n");
    } else {
        printf("Failed: testGetConcantenatedInvertedSequenceDiff equal test, expected ATTCCG#TTACG, got %s \n", seq);
        failed++;
    }

    free(seq);
    
    return failed;
}

int testGetSplittedSeqFilePath() {
    // arrange
    const char *dirName = "dir_name";
    int seqNum = 10;

    // act 
    char* path = getSplittedSeqFilePath(dirName, seqNum);

    // assert
    int failed = 0;

    if (strcmp(path, "dir_name/partition-10.txt") == 0) {
        printf("Passed: testGetSplittedSeqFilePath test \n");
    } else {
        printf("Failed: testGetSplittedSeqFilePath dir_name/partition-10, got %s \n", path);
        failed++;
    }

    free(path);
    
    return failed;
}

int testPartitionSequence1() {
    // arrange
    const char *seq = "123456789";
    int numPartition = 5;

    // act
    char** resultSeqs = partitionSequence(seq, numPartition);

    // assert
    // for (int i = 0; i < numPartition; i++) {
    //     printf("%s, %ld\n", resultSeqs[i], strlen(resultSeqs[i]));
    // } 

    int failed = 0;
    if (strcmp(resultSeqs[0], "12") == 0 && strcmp(resultSeqs[1], "34") == 0 && strcmp(resultSeqs[2], "56") == 0 && strcmp(resultSeqs[3], "78") == 0  && strcmp(resultSeqs[4], "9") == 0) {
        printf("Passed: testPartitionSequence1 test \n");
    } else {
        printf("Falied: testPartitionSequence1\n");
        failed++;
    }

    freePartitions(resultSeqs, numPartition);

    return failed;
 
}

int testPartitionSequence2() {
    // arrange
    const char *seq = "123456789";
    int numPartition = 3;

    // act
    char** resultSeqs = partitionSequence(seq, numPartition);

    // assert
    // for (int i = 0; i < numPartition; i++) {
    //     printf("%s, %ld\n", resultSeqs[i], strlen(resultSeqs[i]));
    // } 

    int failed = 0;
    if (strcmp(resultSeqs[0], "123") == 0 && strcmp(resultSeqs[1], "456") == 0 && strcmp(resultSeqs[2], "789") == 0 ) {
        printf("Passed: testPartitionSequence2 test \n");
    } else {
        printf("Falied: testPartitionSequence2\n");
        failed++;
    }

    freePartitions(resultSeqs, numPartition);

    return failed;
 
}

int testCountNumPartitionFilesInDir() {
    
    // act
    int numPartitionFiles = countNumPartitionFilesInDir("test_partition_dir");

    // assert
    int failed = 0;
    if (numPartitionFiles == 3) {
        printf("Passed: testCountNumPartitionFilesInDir  \n");
    } else {
        printf("Falied: testCountNumPartitionFilesInDir, expected 3, got %d\n", numPartitionFiles);
        failed++;
    }

    return failed;
}

int testGetPartitionSize() {
    // arrange
    const char *dir = "test_partition_dir";

    // act
    unsigned long l = getPartitionSize(dir);

    // assert
    int failed = 0;
    if (l == 4) {
        printf("Passed: testGetPartitionSize  \n");
    } else {
        printf("Falied: testGetPartitionSize, expected 4, got %ld\n", l);
        failed++;
    }

    return failed;
}

int testPartitionIdxToSeqIdx() {
    
}
//
//  Test.c
//  mdf-2.0
//
//  Created by Yiming Wang on 2020/7/5.
//

#include <stdio.h>
#include "TestUtils.h"
#include "TestSuffixArray.h"
#include "TestList.h"
#include "TestMRP.h"
#include "TestHashset.h"
#include "TestDirectCircleRepeatFinder.h"
#include "TestInvertedCircleRepeatFinder.h"
#include "TestPartitionTask.h"

int main(int argc, char *argv[]) {
    int failedCount = 0;

    failedCount += testGetDNASequenceFromFile();
    failedCount += testGetConcantenatedInvertedSequence();
    failedCount += testGetConcatenatedSequence();
    failedCount += testGetConcantenatedInvertedSequenceDiff();
    failedCount += testGetReversedSequence();
    failedCount += testSubstring();
    failedCount += testInvertedSubstring();
    failedCount += testReformatSuffixArrays();
    failedCount += testSingleNodeList();
    failedCount += testMultipleNodeList();
    failedCount += testConcatenateNonEmptyList1AndEmptyList2();
    failedCount += testConcatenateEmptyList1AndNonEmptyList2();
    failedCount += testConcatenateNonEmptyLists();
    failedCount += testSortTuples();
    failedCount += testSearchMRP1();
    failedCount += testSearchMRP2();
    failedCount += testSearchMRP3();
    failedCount += testSearchMRPInTwoSequences();
    failedCount += testSearchMRPInTwoSequences2();
    failedCount += testHashset();
    failedCount += testGetCorresondingIndexFromConcatenatedInvertedSequenceLeft();
    failedCount += testSearchInvertedMRPInSingleSequence();
    failedCount += testSearchInvertedMRPInTwoSequences();
    failedCount += testGetSplittedSeqFilePath();
    failedCount += testPartitionSequence1();
    failedCount += testPartitionSequence2();
    failedCount += testCountNumPartitionFilesInDir();
    failedCount += testParseTaskArg();
    failedCount += testGetPartitionSize();
    failedCount += testGetTaskNum();

    /**
     * compare results with the original suffix tree implementation
     */ 
    // failedCount += testFindDirectCircleRepeatedPairs();
    // failedCount += testFindInvertedCircleRepeatedPairs();
    
    

    /**
     * print out substrings and compare with desired results in the test's comments
     */ 
    // failedCount += testFindDirectCircleRepeatedPairsSubstrings1();
    // failedCount += testFindDirectCircleRepeatedPairsSubstrings2();
    // failedCount += testFindInvertedCircleRepeatedPairsSubstrings1();
    

    /**
     * test this one with AWS, cannot test locally
     */ 
    // failedCount += testSearchMRP4(); 

    printf("total number of failed cases: %d\n", failedCount);

    return 0;
}

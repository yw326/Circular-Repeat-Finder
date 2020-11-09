

#ifndef TestUtils_h
#define TestUtils_h

#include <stdio.h>
#include <string.h>
#include "Utils.h"

int testGetDNASequenceFromFile();
int testGetConcantenatedInvertedSequence();
int testGetConcatenatedSequence();
int testGetConcantenatedInvertedSequenceDiff();
int testGetReversedSequence();

int testSubstring();
int testInvertedSubstring();

int testSortTuples();
int testGetCorresondingIndexFromConcatenatedInvertedSequenceLeft();

int testGetSplittedSeqFilePath();
int testPartitionSequence1();
int testPartitionSequence2();

int testCountNumPartitionFilesInDir();
int testGetPartitionSize();

#endif /* TestUtils_h */



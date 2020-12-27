//
//  Utils.h
//  mdf-2.0
//
//  Created by Yiming Wang on 2020/7/5.
//

#ifndef Utils_h
#define Utils_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "MyStructs.h"

#endif /* Utils_h */

unsigned long getDNASequenceLengthFromFile (char *fileName);

/**
 * return DNA sequence from the file: characters other than A,T,C,G are removed
 */
void getDNASequenceFromFile(char *fileName, char* seq);

/**
 * given s1, s2, return s1#s2
 */
char* getConcatenatedSequence(char* seq, char* seq1, char* seq2, unsigned long n1, unsigned long n2);

/**
 * get reversed sequence of seq
 */
void getReversedSequence(char* seq, unsigned long n, char* resultSeq);

/**
 * given s, output s#s'
 */
void getConcantenatedInvertedSequence(char* givenSeq, unsigned long n, char* resultSeq);

/**
 * given s1,s2, output s1#s2'
 */
void getConcantenatedInvertedSequenceDiff(char* s1, char* s2, unsigned long n1, unsigned long n2, char* resultSeq);

/**
 * in a string s#s', given some substring in s', find its corresponding start index in s
 */
unsigned long getCorresondingIndexFromConcatenatedInvertedSequenceLeft(unsigned long startIndex, int stringLength, 
                                                                   unsigned long n);

/**
 * in a string s1#s2', given some substring in s2', find its corresponding start index in s2
 */
unsigned long getCorresondingIndexFromConcatenatedInvertedSequenceRight(unsigned long startIndex, int stringLength, 
                                                                        unsigned long n);

/**
 * in a string s1#s2', given some substring in s2', find its corresponding start index in s1#s2
 */
unsigned long getCorresondingIndexFromConcatenatedInvertedSequenceRightAbsolute(unsigned long startIndex, int subStringLength, 
                unsigned long middleSeparatorIdx, unsigned long totalLength);


void substring(char* s, int start, int length, char* substr);
char* returnSubstring(char* s, int start, int length);
void invertedSubstring(char *str, int start, int length, char* substr);

void sortTuples(int **tuples, int n);
int charToIndex(char c);

/**
 * return "dir_name/partition-seq_num.txt"
 */ 
char *getSplittedSeqFilePath(char* dir_name, int seq_num);

char** partitionSequence(char* seq, int numberOfPartitions);
void freePartitions(char** partitions, int numberOfPartitions);

int countNumPartitionFilesInDir(char* dir);
unsigned long getPartitionSize(const char* dir);
unsigned long partitionIdxToSeqIdx(unsigned long partitionIdx, int partitionNum, unsigned long partitionSize);


// -------------------------------- for testing --------------------------------

// return a list of fixed size array of strings
// each array contains a fixed number of tokens, specifying a circular repeat
char*** readResultIdxFile(char* fileName, int numberOfTokenExtracting);

// similar to readResultIdxFile, but reads a dir of result files for partition option
char*** readResultDir(char* dir, int numberOfTokenExtracting);

int countLinesInFile(char* fileName);
int countLinesInDir(char* dir);

void freeReadFileResult(char*** result, int numberOfLines, int tokensExtractedPerLine);

// print the elements in result1 but not in result2, and return the count
int resultDifference(char*** rst1, char*** rst2, int numTokens, int size1, int size2);


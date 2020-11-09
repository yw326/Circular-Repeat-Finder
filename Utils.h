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


#endif /* Utils_h */

int getDNASequenceLengthFromFile (char *fileName);

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
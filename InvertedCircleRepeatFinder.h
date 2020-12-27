#ifndef InvertedCircleRepeatFinder_h
#define InvertedCircleRepeatFinder_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MRP.h"
#include "EditDistance.h"
#include "PartitionTask.h"

void findInvertedCircleRepeatedPairs(char *seq, char* seq2, unsigned long n, unsigned long n2, int l1, int l2, int L, 
        double alpha, char* outputFilePath, int verbose, task *t) ;

#endif 


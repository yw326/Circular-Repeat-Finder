#ifndef InvertedCircleRepeatFinder_h
#define InvertedCircleRepeatFinder_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MRP.h"
#include "EditDistance.h"

void findInvertedCircleRepeatedPairs(char *seq, unsigned long n, int l1, int l2, int L, double alpha, 
        char* outputFilePath, int verbose);

#endif 


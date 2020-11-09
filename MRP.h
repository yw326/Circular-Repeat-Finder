


#ifndef MRP_h
#define MRP_h

#include <stdio.h>
#include <string.h>
#include "SuffixArray.h"
#include "List.h"
#include "Utils.h"
#include "hashset.h"
#include "MyStructs.h"

/**
 * output all mrps in sequence
 */
mrpList* searchMRPInSingleSequence(const char* sequence, unsigned long n, uint_t threshold);

/**
 * given sequence = s1#s2, output all mrps (p1,p2,l) with p1 in s1 and p2 in s2
 */
mrpList* searchMRPInTwoSequences(const char* sequence, unsigned long n, uint_t threshold, unsigned long firstSequenceLength);

/**
 * output all maximal inverted pairs in sequence
 */
mrpList* searchInvertedMRPInSingleSequence(const char* sequence, unsigned long n, uint_t threshold);

/**
 * given sequence = s1#s2'
 * output all maximal inverted pairs (p1,p2,l) wrt string s1#s2 with p1 in s1 and p2 in s2
 */
mrpList* searchInvertedMRPInTwoSequences(const char* seq1, const char* seq2, unsigned long n1, unsigned long n2, 
            uint_t threshold);


void freeMRPList(mrpList *ml);

#endif 

// next hash set for inverted
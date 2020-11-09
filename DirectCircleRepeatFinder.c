#include "DirectCircleRepeatFinder.h"

void getMiminizationVector(char* s1, char* s2, char* s3, unsigned long l, int* result) {

    if (l == 0) {
        result[0] = 0;
        result[1] = 0;
        result[2] = 0;
        return;
    }

    // get d(s1,s2)
    int* v1 = levenshtein(s1,s2,l,l);

    // get d(rev_s1, rev_s2)
    char* s1Reverse = malloc(sizeof(char)*(l+1));
    char* s3Reverse = malloc(sizeof(char)*(l+1));
    getReversedSequence(s1,l,s1Reverse);
    getReversedSequence(s3,l,s3Reverse);
    int* v2Reverse = levenshtein(s1Reverse,s3Reverse,l,l);
    free(s1Reverse); 
    free(s3Reverse);

    // get rev_d(s1,s2)
    int v2[l];
    for (int i = 0; i < l; i++) {
        v2[i] = v2Reverse[l-1-i];
    }

    // find mim distance and corresponding index
    int idx = -1;
    int min = l*3;
    
    for (int i = 0; i < l; i++) {
        if (v1[i] + v2[i] < min) {
            min = v1[i] + v2[i];
            idx = i;
        }
    }

    // construct result
    result[0] = idx;
    result[1] = v1[idx];
    result[2] = v2[idx];

    free(v1);
    free(v2Reverse);
}

void findDirectCircleRepeatedPairs(char *seq, unsigned long n, int l1, int l2, int L, double alpha, 
        char* outputFilePath, int verbose, task *t) {
    
    
    FILE* outputFile = fopen(outputFilePath, "w");
    int count = 0;

    mrpList *firstLevelMrps;
    if (t && t->partitionNum1 != t->partitionNum2) {
        firstLevelMrps = searchMRPInTwoSequences(seq, n, l1, t->partitionSize);
    } else {
        firstLevelMrps = searchMRPInSingleSequence(seq, n, l1);
    }
    
    printf("the number of first level mrps is %d\n", firstLevelMrps->size);

    // for (int i = 0; i < 0; i++) {

    for (int i = 0; i < firstLevelMrps->size; i++) {
        mrp currMrp1 = firstLevelMrps->mrps[i];
        uint_t repeat1Length = currMrp1.length;
        uint_t repeat1p1 = currMrp1.p1;
        uint_t repeat1p2 = currMrp1.p2;

        int firstRightExtensionStart = repeat1p1 + repeat1Length;
        int secondLeftExtensionStart = repeat1p2 - L;

        if (firstRightExtensionStart > secondLeftExtensionStart || repeat1p1 < L) {
            continue;
        }

        char* firstRightExtension = returnSubstring(seq, firstRightExtensionStart, L);
        char* secondLeftExtension = returnSubstring(seq, secondLeftExtensionStart, L);

        if (verbose) {
            printf("firstRightExtension: %s\n", firstRightExtension);
            printf("secondLeftExtension: %s\n", secondLeftExtension);
        }

        char* concatenatedSequence1 = malloc(sizeof(char)*(2*L+2));
        getConcatenatedSequence(concatenatedSequence1, firstRightExtension, secondLeftExtension, L, L);
        
        mrpList* secondLevelMrps = searchMRPInTwoSequences(concatenatedSequence1, 2*L+2, l2, L);

        free(firstRightExtension); free(secondLeftExtension); free(concatenatedSequence1);

        for (int j = 0; j < secondLevelMrps->size; j++) {
            mrp currMrp2 = secondLevelMrps->mrps[j];
            uint_t repeat2Length = currMrp2.length;
            uint_t repeat2p1 = currMrp2.p1;
            uint_t repeat2p2 = currMrp2.p2;
            repeat2p2 = repeat2p2 - L - 1;

            // we have ... A1 s1 A2 s2 A3 ...B1 s2 B2 s1 B3...
            // distance(A2,B3) + rev_distance(A2,B1) (elementwise), take minimum index i1
            // distance(B2,A3) + rev_distance(B2,A1) (elementwise), taken minimum index i2
            // |A2| = |B3| = |B1| = x1
            // |A1| = |A3| = |B2| = x2

            int x1 = repeat2p1;
            int x2 = repeat1p2 - (secondLeftExtensionStart + repeat2p2 + repeat2Length);
            int exactLength = repeat1Length + repeat2Length;
            int totalLength = exactLength*2 + x1*3 + x2*3;

            if (x1 - x2 > alpha*totalLength || x1 - x2 < -alpha*totalLength) {
                continue;
            }

            // get first minimizaion result
            char* A2 = returnSubstring(seq, firstRightExtensionStart, x1);
            char* B3 = returnSubstring(seq, repeat1p2 + repeat1Length, x1);
            char* B1 = returnSubstring(seq, secondLeftExtensionStart + repeat2p2 - x1, x1);

            if (verbose) {
                printf("A2: %s\n", A2);
                printf("B3: %s\n", B3);
                printf("B1: %s\n", B1);
            }

            int* result1 = malloc(sizeof(int)*3);
            getMiminizationVector(A2, B3, B1, x1, result1);
            free(A2); free(B3); free(B1);
            // result1[0] = 0; result1[1] = 0; result1[2] = 0;
            int i1 = result1[0]; int s1Min1 = result1[1]; int s2Min1 = result1[2];
            free(result1);
            
            int s1s2Len = x1 + x2 + exactLength;
            if (s1Min1 > s1s2Len*alpha || s2Min1 > s1s2Len*alpha) {
                continue;
            } 

            // get second minimizaion result

            char* A1 = returnSubstring(seq, repeat1p1-x2, x2);
            char* A3 = returnSubstring(seq, firstRightExtensionStart + repeat2p1 + repeat2Length, x2);
            char* B2 = returnSubstring(seq, repeat1p2 - x2, x2);

            if (verbose) {
                printf("A1: %s\n", A1);
                printf("A3: %s\n", A3);
                printf("B2: %s\n", B2);
            }

            int* result2 = malloc(sizeof(int)*3);
            getMiminizationVector(B2, A3, A1, x2, result2);
            // result2[0] = 0; result2[1] = 0; result2[2] = 0;
            free(A1); free(A3); free(B2);
            int i2 = result2[0]; int s2Min2 = result2[1]; int s1Min2 = result2[2];
            free(result2);

            // output result to file
            int firstStart = repeat1p1 - (x2 - i2);
            int firstS1Length = x2 - i2 + repeat1Length + i1;
            int firstS2Length = x1 - i1 + repeat2Length + i2;

            int secondStart = secondLeftExtensionStart + repeat2p2- x1 + i1;
            int secondS2Length = x1 - i1 + repeat2Length + i2;
            int secondS1Length = x2 - i2 + repeat1Length + i1;

            double totalMismatchRatio = (s1Min1 + s1Min2 + s2Min1 + s2Min2) / ((double) x1 + x2 + exactLength);
            double s1Mismatch = (s1Min1 + s1Min2) / ((double) firstS1Length); // use both instead of one
            double s2Mismatch = (s2Min1 + s2Min2) / ((double) firstS2Length);

            if (s1Mismatch > alpha || s2Mismatch > alpha) {
                continue;
            }
            
            if (t) {
                firstStart = partitionIdxToSeqIdx(firstStart, t->partitionNum1, t->partitionSize);
                if (t->partitionNum1 == t->partitionNum2) {
                    secondStart = partitionIdxToSeqIdx(secondStart, t->partitionNum2, t->partitionSize);
                } else {
                    secondStart = partitionIdxToSeqIdx(secondStart - t->partitionSize - 1, t->partitionNum2, t->partitionSize);
                }
            } 

            fprintf(outputFile, "(%d,%d,%d,%d,%d,%d,%f,%f,%f,%d)\n", firstStart, secondStart, firstS1Length, 
            firstS2Length, secondS1Length, secondS2Length, totalMismatchRatio, s1Mismatch, s2Mismatch, x1+x2+exactLength);

            // printf("a (%d,%d,%d,%d,%d,%d,%f,%f,%f,%d)\n", firstStart, secondStart, firstS1Length, 
            // firstS2Length, secondS1Length, secondS2Length, totalMismatchRatio, s1Mismatch, s2Mismatch, x1+x2+exactLength);
            // printf("%d,%d,%d,%d\n", repeat1p1, repeat1p2, repeat2p1, repeat2p2);

            count++;
        }

        freeMRPList(secondLevelMrps);
    
    
        int firstLeftExtensionStart = repeat1p1 - L;
        int secondRightExtensionStart = repeat1p2 + repeat1Length;

        if (firstLeftExtensionStart < 0 || secondRightExtensionStart + L >= n) {
            continue;
        }

        char* firstLeftExtension = returnSubstring(seq, firstLeftExtensionStart, L);
        char* secondRightExtension = returnSubstring(seq, secondRightExtensionStart, L);

        if (verbose) {
            printf("firstLeftExtension: %s\n", firstLeftExtension);
            printf("secondRightExtension: %s\n", secondRightExtension);
        }

        char* concatenatedSequence2 = malloc(sizeof(char)*(2*L+2));
        getConcatenatedSequence(concatenatedSequence2, firstLeftExtension, secondRightExtension, L, L);
        
        mrpList* secondLevelMrps2 = searchMRPInTwoSequences(concatenatedSequence2, 2*L+2, l2, L);

        free(firstLeftExtension); free(secondRightExtension); free(concatenatedSequence2);

        for (int j = 0; j < secondLevelMrps2->size; j++) {
            mrp currMrp2 = secondLevelMrps2->mrps[j];
            uint_t repeat2Length = currMrp2.length;
            uint_t repeat2p1 = currMrp2.p1;
            uint_t repeat2p2 = currMrp2.p2;
            repeat2p2 = repeat2p2 - L - 1;

            int x1 = L - repeat2p1 - repeat2Length;
            int x2 = repeat2p2;
            int exactLength = repeat1Length + repeat2Length;
            int totalLength = exactLength*2 + x1*3 + x2*3;

            if (x1 - x2 > alpha*totalLength || x1 - x2 < -alpha*totalLength) {
                continue;
            }

            // get first minimizaion result
            char* A2 = returnSubstring(seq, firstLeftExtensionStart + repeat2p1 + repeat2Length, x1);
            char* B3 = returnSubstring(seq, secondRightExtensionStart + repeat2p2 + repeat2Length, x1);
            char* B1 = returnSubstring(seq, repeat1p2 - x1, x1);
            
            if (verbose) {
                printf("A2: %s\n", A2);
                printf("B3: %s\n", B3);
                printf("B1: %s\n", B1);
            }
            
            int* result1 = malloc(sizeof(int)*3);
            getMiminizationVector(A2, B3, B1, x1, result1);
            free(A2); free(B3); free(B1);
            // result1[0] = 0; result1[1] = 0; result1[2] = 0;
            int i1 = result1[0]; int s1Min1 = result1[1]; int s2Min1 = result1[2];
            free(result1);

            // get second minimizaion result
            char* A1 = returnSubstring(seq, firstLeftExtensionStart + repeat2p1 - x2, x2);
            char* A3 = returnSubstring(seq, repeat1p1 + repeat1Length, x2);
            char* B2 = returnSubstring(seq, repeat1p2 + repeat1Length, x2);

            if (verbose) {
                printf("A1: %s\n", A1);
                printf("A3: %s\n", A3);
                printf("B2: %s\n", B2);
            }

            int* result2 = malloc(sizeof(int)*3);
            getMiminizationVector(B2, A3, A1, x2, result2);
            free(A1); free(A3); free(B2);
            // result2[0] = 0; result2[1] = 0; result2[2] = 0;
            int i2 = result2[0]; int s2Min2 = result2[1]; int s1Min2 = result2[2];
            free(result2);

            // output result to file
            int firstStart = firstLeftExtensionStart + repeat2p1 - (x2-i2);
            int firstS1Length = x2 - i2 + repeat2Length + i1;
            int firstS2Length = x1 - i1 + repeat1Length + i2;

            int secondStart = repeat1p2 - (x1 - i1);
            int secondS2Length = x1 - i1 + repeat1Length + i2;
            int secondS1Length = x2 - i2 + repeat2Length + i1;

            double totalMismatchRatio = (s1Min1 + s1Min2 + s2Min1 + s2Min2) / ((double) x1 + x2 + exactLength);
            double s1Mismatch = (s1Min1 + s1Min2) / ((double) firstS1Length); // use both instead of one
            double s2Mismatch = (s2Min1 + s2Min2) / ((double) firstS2Length);

            if (s1Mismatch > alpha || s2Mismatch > alpha) {
                continue;
            }

            if (t) {
                firstStart = partitionIdxToSeqIdx(firstStart, t->partitionNum1, t->partitionSize);
                if (t->partitionNum1 == t->partitionNum2) {
                    secondStart = partitionIdxToSeqIdx(secondStart, t->partitionNum2, t->partitionSize);
                } else {
                    secondStart = partitionIdxToSeqIdx(secondStart - t->partitionSize - 1, t->partitionNum2, t->partitionSize);
                }
            } 


            fprintf(outputFile, "(%d,%d,%d,%d,%d,%d,%f,%f,%f,%d)\n", firstStart, secondStart, firstS1Length, 
            firstS2Length, secondS1Length, secondS2Length, totalMismatchRatio, s1Mismatch, s2Mismatch, x1+x2+exactLength);

            // printf("b (%d,%d,%d,%d,%d,%d,%f,%f,%f,%d)\n", firstStart, secondStart, firstS1Length, 
            // firstS2Length, secondS1Length, secondS2Length, totalMismatchRatio, s1Mismatch, s2Mismatch, x1+x2+exactLength);
            
            count++;
        }

        freeMRPList(secondLevelMrps2);
    }
    
    freeMRPList(firstLevelMrps);
    fclose(outputFile);
    printf("the number of cirlce repeat found: %d \n", count);
}
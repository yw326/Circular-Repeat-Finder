#include "InvertedCircleRepeatFinder.h"

/**
 * ......A1 s1 A2 s2 A3 ...... B1 s1' B2 s2' B3 ......
 * f(A2, B1, B3) and f(B2, A3, A1)  
 * 
 */ 


void getMiminizationVectorInv(char* s1, char* s2, char* s3, unsigned long l, int* result) {

    if (l == 0) {
        result[0] = 0;
        result[1] = 0;
        result[2] = 0;
        return;
    }

    // get v1
    char* s2Inverted = malloc(sizeof(char)*(l+1));
    invertedSubstring(s2, 0, l, s2Inverted);
    int* v1 = levenshtein(s1, s2Inverted,l,l);
    free(s2Inverted);

    // get v2
    char* s1Inverted = malloc(sizeof(char)*(l+1));
    invertedSubstring(s1, 0, l, s1Inverted);
    int* v2Inverted = levenshtein(s1Inverted, s3, l, l);
    free(s1Inverted); 

    int v2[l];
    for (int i = 0; i < l; i++) {
        v2[i] = v2Inverted[l-1-i];
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
    free(v2Inverted);
}

void findInvertedCircleRepeatedPairs(char *seq, unsigned long n, int l1, int l2, int L, double alpha, 
        char* outputFilePath, int verbose) {
    
    FILE* outputFile = fopen(outputFilePath, "w");
    int count = 0;


    mrpList *firstLevelMrps = searchInvertedMRPInSingleSequence(seq, n, l1);
    printf("the number of first level inverted mrps is %d\n", firstLevelMrps->size);

    for (int i = 0; i < firstLevelMrps->size; i++) {
        mrp currMrp1 = firstLevelMrps->mrps[i];
        uint_t repeat1Length = currMrp1.length;
        uint_t repeat1p1 = currMrp1.p1;
        uint_t repeat1p2 = currMrp1.p2;

        int rightExtensionStart1 = repeat1p1 + repeat1Length;
        int rightExtensionStart2 = repeat1p2 + repeat1Length;

        int rightLimit = n; // it's (n // 2) if parallel 
        if (rightExtensionStart2 + L >= rightLimit || rightExtensionStart1 + L >= repeat1p2) {
            continue;
        }

        char* rightExtension1 = malloc(sizeof(char)*(L+1));
        char* rightExtension2 = malloc(sizeof(char)*(L+1));
        substring(seq, rightExtensionStart1, L, rightExtension1);
        substring(seq, rightExtensionStart2, L, rightExtension2);

        if (verbose) {
            printf("rightExtension1: %s\n", rightExtension1);
            printf("rightExtension2Inverted: %s\n", rightExtension2);
        }
        
        mrpList* secondLevelMrps = searchInvertedMRPInTwoSequences(rightExtension1, rightExtension2, L, L, l2);

        free(rightExtension1); free(rightExtension2); 

        
        for (int j = 0; j < secondLevelMrps->size; j++) {
            mrp currMrp2 = secondLevelMrps->mrps[j];
            uint_t repeat2Length = currMrp2.length;
            uint_t repeat2p1 = currMrp2.p1;
            uint_t repeat2p2 = currMrp2.p2;

            
            // repeat2p2 = repeat2p2 - L - 1;

            // we have ... A1 s1 A2 s2 A3 ...B1 s1' B2 s2' B3...
            // rev_distance(A2,B3) + rev_distance(A2,B1) (elementwise), take minimum index i1
            // rev_distance(B2,A3) + rev_distance(B2,A1) (elementwise), taken minimum index i2
            // len(A2) = len(B3) = len(B1) = l1
            // len(A1) = len(B2) = len(A3) = l2
            int x1 = repeat2p1;
            int x2 = repeat2p2;
            int exactLength = repeat1Length + repeat2Length;                
            int totalLength = exactLength*2 + x1*3 + x2*3;

            if (x1 - x2 > alpha*totalLength || x1 - x2 < -alpha*totalLength) {
                continue;
            }

            // get first minimizaion result
            char* A2 = malloc(sizeof(char)*(x1+1));
            char* B3 = malloc(sizeof(char)*(x1+1));
            char* B1 = malloc(sizeof(char)*(x1+1));
            substring(seq, rightExtensionStart1, x1, A2);
            substring(seq, rightExtensionStart2 + repeat2p2 + repeat2Length, x1, B3);
            substring(seq, repeat1p2 - x1, x1, B1);

            if (verbose) {
                printf("A2: %s\n", A2);
                printf("B3: %s\n", B3);
                printf("B1: %s\n", B1);
            }
            
            int* result1 = malloc(sizeof(int)*3);
            getMiminizationVectorInv(A2, B1, B3, x1, result1);
            free(A2); free(B3); free(B1);
            int i1 = result1[0]; int s1Min1 = result1[1]; int s2Min1 = result1[2];
            free(result1);

            int s1s2Length = x1 + x2 + exactLength;
            if (s1Min1 > s1s2Length * alpha || s2Min1 > s1s2Length * alpha) {
                continue;
            }

            // get second minimizaion result
            char* A1 = malloc(sizeof(char)*(x2+1));
            char* A3 = malloc(sizeof(char)*(x2+1));
            char* B2 = malloc(sizeof(char)*(x2+1));
            substring(seq, repeat1p1 - x2, x2, A1);
            substring(seq, rightExtensionStart1 + repeat2p1 + repeat2Length, x2, A3);
            substring(seq, rightExtensionStart2, x2, B2);

            if (verbose) {
                printf("A1: %s\n", A1);
                printf("A3: %s\n", A3);
                printf("B2: %s\n", B2);
            }

            int* result2 = malloc(sizeof(int)*3);
            getMiminizationVectorInv(B2, A1, A3, x2, result2);
            free(A1); free(A3); free(B2);
            int i2 = result2[0]; int s1Min2 = result2[1]; int s2Min2 = result2[2];
            free(result2);

            // output result to file
            unsigned long firstStart = repeat1p1 - i2;
            int firstS1Length = i2 + repeat1Length + i1;
            int firstS2Length = x1 - i1 + repeat2Length + (x2 - i2);

            unsigned long secondStart = repeat1p2 - i1;
            int secondS1Length = firstS1Length;
            int secondS2Length = firstS2Length;

            double totalMismatchRatio = (s1Min1 + s1Min2 + s2Min1 + s2Min2) / ((double) x1 + x2 + exactLength);
            double s1Mismatch = (s1Min1 + s1Min2) / ((double) firstS1Length); // use both instead of one
            double s2Mismatch = (s2Min1 + s2Min2) / ((double) firstS2Length);

            if (s1Mismatch > alpha || s2Mismatch > alpha) {
                continue;
            }

            fprintf(outputFile, "(%ld,%ld,%d,%d,%d,%d,%f,%f,%f,%d)\n", firstStart, secondStart, firstS1Length, 
            firstS2Length, secondS1Length, secondS2Length, totalMismatchRatio, s1Mismatch, s2Mismatch, x1+x2+exactLength);

            // printf("(%d,%d,%d,%d,%d,%d,%f,%f,%f,%d)\n", firstStart, secondStart, firstS1Length, 
            // firstS2Length, secondS1Length, secondS2Length, totalMismatchRatio, s1Mismatch, s2Mismatch, x1+x2+exactLength);

            count++;
        }

        freeMRPList(secondLevelMrps);

        int leftExtensionStart1 = repeat1p1 - L;
        int leftExtensionStart2 = repeat1p2 - L;

        if (leftExtensionStart1 < 0 || leftExtensionStart2 <= repeat1p1 + repeat1Length) {
            continue;
        }
        // TO DO: another short-circuit for parallel

        char* leftExtension1 = malloc(sizeof(char)*(L+1));
        char* leftExtension2 = malloc(sizeof(char)*(L+1));
        substring(seq, leftExtensionStart1, L, leftExtension1);
        substring(seq, leftExtensionStart2, L, leftExtension2);

        if (verbose) {
            printf("leftExtension1: %s\n", leftExtension1);
            printf("leftExtension2: %s\n", leftExtension2);
        }

        mrpList* secondLevelMrps2 = searchInvertedMRPInTwoSequences(leftExtension1, leftExtension2, L, L, l2);
        
        free(leftExtension1); free(leftExtension2); 

        for (int j = 0; j < secondLevelMrps2->size; j++) {
            mrp currMrp2 = secondLevelMrps2->mrps[j];
            uint_t repeat2Length = currMrp2.length;
            uint_t repeat2p1 = currMrp2.p1;
            uint_t repeat2p2 = currMrp2.p2;

            int x1 = L - repeat2p1 - repeat2Length;
            int x2 = L - repeat2p2 - repeat2Length;
            int exactLength = repeat1Length + repeat2Length;
            int totalLength = exactLength*2 + x1*3 + x2*3;

            if (x1 - x2 > alpha*totalLength || x1 - x2 < -alpha*totalLength) {
                continue;
            }

            // get first minimizaion result
            char* A2 = malloc(sizeof(char)*(x1+1));
            char* B3 = malloc(sizeof(char)*(x1+1));
            char* B1 = malloc(sizeof(char)*(x1+1));
            substring(seq, leftExtensionStart1 + repeat2p1 + repeat2Length, x1, A2);
            substring(seq, repeat1p2 + repeat1Length, x1, B3);
            substring(seq, leftExtensionStart2 + repeat2p2 - x1, x1, B1);

            if (verbose) {
                printf("A2: %s\n", A2);
                printf("B3: %s\n", B3);
                printf("B1: %s\n", B1);
            }
            
            int* result1 = malloc(sizeof(int)*3);
            getMiminizationVectorInv(A2, B1, B3, x1, result1);
            free(A2); free(B3); free(B1);
            int i1 = result1[0]; int s1Min1 = result1[1]; int s2Min1 = result1[2];
            free(result1);

            int s1s2Length = x1 + x2 + exactLength;
            if (s1Min1 > s1s2Length * alpha || s2Min1 > s1s2Length * alpha) {
                continue;
            }

            // get second minimizaion result
            char* A1 = malloc(sizeof(char)*(x2+1));
            char* A3 = malloc(sizeof(char)*(x2+1));
            char* B2 = malloc(sizeof(char)*(x2+1));
            substring(seq, leftExtensionStart1 + repeat2p1 - x2, x2, A1);
            substring(seq, repeat1p1 + repeat1Length, x2, A3);
            substring(seq, leftExtensionStart2 + repeat2p2 + repeat2Length, x2, B2);

            if (verbose) {
                printf("A1: %s\n", A1);
                printf("A3: %s\n", A3);
                printf("B2: %s\n", B2);
            }

            int* result2 = malloc(sizeof(int)*3);
            getMiminizationVectorInv(B2, A1, A3, x2, result2);
            free(A1); free(A3); free(B2);
            int i2 = result2[0]; int s1Min2 = result2[1]; int s2Min2 = result2[2];
            free(result2);

            // output result to file
            unsigned long  firstStart = leftExtensionStart1 + repeat2p1 - i2;
            int firstS1Length = i2 + repeat2Length + i1;
            int firstS2Length = x1 - i1 + repeat1Length + (x2 - i2);

            unsigned long  secondStart = leftExtensionStart2 + repeat2p2 - i1;
            int secondS2Length = firstS1Length;
            int secondS1Length = firstS2Length;

            double totalMismatchRatio = (s1Min1 + s1Min2 + s2Min1 + s2Min2) / ((double) x1 + x2 + exactLength);
            double s1Mismatch = (s1Min1 + s1Min2) / ((double) firstS1Length); // use both instead of one
            double s2Mismatch = (s2Min1 + s2Min2) / ((double) firstS2Length);

            if (s1Mismatch > alpha || s2Mismatch > alpha) {
                continue;
            }

            fprintf(outputFile, "(%ld,%ld,%d,%d,%d,%d,%f,%f,%f,%d)\n", firstStart, secondStart, firstS1Length, 
            firstS2Length, secondS1Length, secondS2Length, totalMismatchRatio, s1Mismatch, s2Mismatch, x1+x2+exactLength);

            // printf("(%d,%d,%d,%d,%d,%d,%f,%f,%f,%d)\n", firstStart, secondStart, firstS1Length, 
            // firstS2Length, secondS1Length, secondS2Length, totalMismatchRatio, s1Mismatch, s2Mismatch, x1+x2+exactLength);

            count++;
        }

        freeMRPList(secondLevelMrps2);
    }
    
    freeMRPList(firstLevelMrps);
    fclose(outputFile);
    printf("the number of cirlce repeat found: %d \n", count);
}




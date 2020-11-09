
#include "TestSuffixArray.h"

char TEST_STR[] = "acaaacatat";

int x(){return 1;}


int testReformatSuffixArrays() {
    // arrange
    unsigned long n = strlen(TEST_STR) + 1;
    uint_t *SA = (uint_t *)malloc(n * sizeof(uint_t));
    int_t *LCP = (int_t *)malloc(n * sizeof(int_t));
    char *BWT = (char *)malloc(n * sizeof(char));

    uint_t expectedSA[] = {2,3,0,4,8,6,1,5,9,7,10};
    int_t expectedLCP[] = {0,2,1,3,1,2,0,2,0,1,0};
    char expectedBWT[] = {'c', 'a', '?', 'a', 't', 'c', 'a', 'a', 'a', 'a', 't'};

    // act
    reformatSuffixArrays(TEST_STR, SA, LCP, BWT, n);

    // assert
    int failedSA = 0;
    int failedLCP = 0;
    int failedBWT = 0;
    for(int i = 0; i < n; i++) {
        if (SA[i] != expectedSA[i] && failedSA == 0) {
            failedSA = 1;
            printf("Failed: testReformattedSuffixArrays SA test\n");
        } 

        if (LCP[i] != expectedLCP[i] && failedLCP == 0) {
            failedLCP = 1;
            printf("Failed: testReformattedSuffixArrays LCP test\n");
        }

        if (BWT[i] != expectedBWT[i] && failedBWT == 0) {
            failedBWT = 1;
            printf("Failed: testReformattedSuffixArrays BWT test\n");
        }
    }

    int failed = failedSA || failedLCP || failedBWT;
    if (!failed) {
        printf("Passed: testReformattedSuffixArrays\n");
    }

    free(SA);
    free(LCP);
    free(BWT);

    return failed;
}










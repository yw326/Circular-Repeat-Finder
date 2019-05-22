//
//  edit_distance.c
//  GeneSeq
//
//  Created by Yiming Wang on 2019/3/11.
//  Copyright © 2019 Yiming Wang. All rights reserved.
//

#include "edit_distance.h"

// MARK: edit distance
/* Returns an unsigned integer, depicting
 * the difference between `a` and `b`.
 * See http://en.wikipedia.org/wiki/Levenshtein_distance
 * for more information. */

#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

// returns the diagonal
int *levenshtein(char *s1, char *s2, int s1len, int s2len) {
    unsigned int x, y, lastdiag, olddiag;
    unsigned int column[s1len+1];
    
    int *result = malloc(sizeof(int)*s1len);
    
    for (y = 1; y <= s1len; y++)
        column[y] = y;
    for (x = 1; x <= s2len; x++) {
        column[0] = x;
        for (y = 1, lastdiag = x-1; y <= s1len; y++) {
            olddiag = column[y];
            column[y] = MIN3(column[y] + 1, column[y-1] + 1, lastdiag + (s1[y-1] == s2[x-1] ? 0 : 1));
            
            if (x==y) {
                result[x-1] = column[x];
            }
            
            lastdiag = olddiag;
        }
        

    }
    
    return result;
}

int levenshtein_val(char *s1, char *s2, int s1len, int s2len) {
    unsigned int x, y, lastdiag, olddiag;
    unsigned int column[s1len+1];
    
    for (y = 1; y <= s1len; y++)
        column[y] = y;
    for (x = 1; x <= s2len; x++) {
        column[0] = x;
        for (y = 1, lastdiag = x-1; y <= s1len; y++) {
            olddiag = column[y];
            column[y] = MIN3(column[y] + 1, column[y-1] + 1, lastdiag + (s1[y-1] == s2[x-1] ? 0 : 1));
            
            lastdiag = olddiag;
        }
        
        
    }
    
    return (column[s1len]);
}

void test_edit_distance() {
    int *dist1 = levenshtein("sunday", "saturday", 6, 8);
    if (dist1[0] == 0 && dist1[1] == 1 && dist1[2] == 2 && dist1[3] == 3 && dist1[4] == 4 && dist1[5] == 5) {
        printf("test sunday saturday passed (array) \n");
    } else {
        printf("test sunday saturday failed (array) \n");
    }

    int *dist2 = levenshtein("kitten", "sitting", 6, 7);
    if (dist2[0] == 1 && dist2[1] == 1 && dist2[2] == 1 && dist2[3] == 1 && dist2[4] == 2 && dist2[5] == 2) {
        printf("test kitten sitting passed (array) \n");
    } else {
        printf("test kitten sitting failed (array) \n");
    }
    free(dist1);
    free(dist2);
    
    if (levenshtein_val("sunday", "saturday", 6, 8) == 3) {
        printf("test sunday saturday passed (val) \n");
    } else {
        printf("test sunday saturday failed (val) \n");
    }
    
    if (levenshtein_val("kitten", "sitting", 6, 7) == 3) {
        printf("test sunday saturday passed (val) \n");
    } else {
        printf("test sunday saturday failed (val) \n");
    }
}



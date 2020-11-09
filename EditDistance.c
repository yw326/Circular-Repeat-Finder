#include "EditDistance.h"

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

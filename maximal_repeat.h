

#ifndef maximal_repeat_h
#define maximal_repeat_h

#include <stdio.h>
#include "sfxtree.h"

#endif /* maximal_repeat_h */


typedef struct {
    int p1;
    int p2;
    short length;
}triple;


typedef struct {
    triple *result;
    short size;
} tuple;

typedef struct {
    int *arr;
    short size;
} arr_with_size;

arr_with_size* KMP(char *txt, char *pat, int N, int M);
void printDictionaryInfo(treenode_t *node);
void printArcLabel(char *str, treenode_t *node);
void printPathLabel(char *str, treenode_t *node);
int convertCharToInt(char c);
void outputRepeatedPairs(treenode_t *root, char *str, int threshold, int min_extension_len, float mis_perc, int reversed);
char *returnSubstring(char *str, int start, int length);
int levenshtein(char *s1, char *s2, int s1len, int s2len);


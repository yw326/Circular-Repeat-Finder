
#ifndef MYSTRUCTS_H 
#define MYSTRUCTS_H 

#include "SuffixArray.h"
#include "List.h"

#ifndef NUMBER_OF_POSSIBLE_LEFT_CHARACTERS
        #define NUMBER_OF_POSSIBLE_LEFT_CHARACTERS 6 // A, T, C, G, #, ?
#endif

typedef struct{
    uint_t p1;
    uint_t p2;
    uint_t length;
} mrp ;

typedef struct {
    int size;
    mrp* mrps;
} mrpList;

//maybe add left char?
typedef struct temp_child {
    int lb;
    int rb;
    List *positionSet[NUMBER_OF_POSSIBLE_LEFT_CHARACTERS];
    struct temp_child *next_sibling;
} child;

typedef struct  {
    int_t lcp;
    uint_t lb;
    uint_t rb;
    child *childrenList;
    List *positionSet[NUMBER_OF_POSSIBLE_LEFT_CHARACTERS];
} interval;



#endif
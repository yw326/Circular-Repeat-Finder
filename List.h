


#ifndef List_h
#define List_h

#include <stdio.h>
#include <string.h>
#include "SuffixArray.h"

typedef struct tmp {
	uint_t val;
	struct tmp *next;
} Node;

typedef struct {
	Node *head;
	Node *tail;
} List;


List* initList();
Node* createNode(uint_t val);
void add(List *lp, uint_t val);
void freeList(List *lp);
void concatenateTwoList(List *lp1, List *lp2); 


#endif 


#include "List.h"

List* initList() {
    List *lp = malloc(sizeof(List));
	lp->head = NULL;
	lp->tail = NULL;
    return lp;
}

Node * createNode(uint_t val) {
	Node * nNode;

	nNode = (Node *) malloc(sizeof(Node));

	nNode->val = val;
	nNode->next = NULL;

	return nNode;
}

void add(List * lp, uint_t item) {
	Node * node;
	node = createNode(item);

	if(lp->head == NULL) {
		lp->head = node;
		lp->tail = node;
	} else {
		lp->tail->next = node;
		lp->tail = lp->tail->next;
	}
    
}

// change l1 to become the concatenated list, and dealloc l2
void concatenateTwoList(List *lp1, List *lp2) {
    if (!lp1 && lp2) {
        lp1 = initList();
        lp1->head = lp2->head;
        lp1->tail = lp2->tail;
        return;
    } else if ( (lp1 && !lp2) || (!lp1 && !lp2) ) {
        return;
    } 

    if (!lp1->head) {
        lp1->head = lp2->head;
        lp1->tail = lp2->tail;
        return;
    } else if (!lp2->head) {
        return;
    } else {
        lp1->tail->next = lp2->head;
        lp1->tail = lp2->tail;
    }
}

void freeList(List *lp) {
    if (!lp) { return; }  

    if (lp->head) {
        Node *curr = lp->head;
        while (curr) {
            Node *next = curr->next;
            // printf("%d\n",curr->val);
            free(curr);
            // printf("haha\n");
            curr = next;
        }
    }
    // printf("lol\n");
    free(lp);
    // printf("lol2\n");
}

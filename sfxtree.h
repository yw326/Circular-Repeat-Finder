#include <limits.h>


#ifndef __SFXTREE_H__
#define __SFXTREE_H__


typedef struct {
    int start, end;   /* end not included */
    /* end == INT_MAX    range from start to the end of string
     * end <= start      empty string */
} range_t;


typedef struct Node{
    int data;
    struct Node *next;
}Node;

typedef struct {
    Node *start;
    Node *end;
}linked_list;

typedef struct treenode_s {
    linked_list node_dic[6];
    range_t arc_val;    /* substr of *arc* which led here. start==end==0 for root */
    range_t node_val;   /* substr of *path* which led here. start==end==0 for root */
    struct treenode_s *suffix_link;
    struct treenode_s *parent;
    struct treenode_s *next_sibling;
    struct treenode_s *prev_sibling;
    struct treenode_s *first_child;
    //int done;
} treenode_t;

/* Point in tree. Can be on a node or an arc. Equivalent to a canonical
 * reference pair in Ukkonen's terminology. */
typedef struct {
    treenode_t *parent;
    treenode_t *child;  /* can be NULL if arcpos == 0 */
    int arcpos;         /* must be a valid index in the arc from parent to child */
} treepoint_t;


/* length of a range_t */
#define RANGE_LEN(r) (((r)).end - ((r)).start)
/* length of a treepoint_t */
#define POINT_LEN(r) (((r)).arcpos + RANGE_LEN(((r)).parent->node_val))

void printRange(const char *str, range_t sfx);
void printTree(const char *str, const treenode_t *root);
void freeTree(treenode_t *root);


treepoint_t fastScan(const char *str, const range_t *ss, treenode_t *tree);
treepoint_t slowScan(const char *str, const range_t *ss, const treepoint_t *start);
treenode_t *splitAtPoint(const treepoint_t *pos);
treenode_t *newChild(treenode_t *parent, const range_t *node_val);



#endif




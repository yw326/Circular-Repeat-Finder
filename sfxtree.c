#include "sfxtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void printRange(const char *str, range_t sfx)
{
    for (int i=sfx.start; i<sfx.end; i++) {
        putchar(str[i] == '\0' ? '$' : str[i]);
        if (str[i] == '\0')
            return;
    }
}


void _printtree(const char *str, const treenode_t *root)
{
    printf("L%p [label=\"\" shape=point];\n", root);
    if (root->parent) {
        printf("L%p -> L%p [label=\"'", root->parent, root);
        printRange(str, root->arc_val);
        printf("'\"];\n");
    }
    if (root->suffix_link) {
        printf("L%p -> L%p [style=dotted];\n", root, root->suffix_link);
    }
    if (root->first_child) {
        _printtree(str, root->first_child);
    }
    if (root->next_sibling) {
        _printtree(str, root->next_sibling);
    }
}


void printTree(const char *str, const treenode_t *root)
{
    printf("digraph G {\n");
    _printtree(str, root);
    printf("}\n");
}


void freeTree(treenode_t *root) {
    if (root->first_child)
        freeTree(root->first_child);
    if (root->next_sibling)
        freeTree(root->next_sibling);
    free(root);
}


/* Search where, in the tree, the string sfx appears. It must be
 * present in the tree in its entirety (not necessarily at a leaf node)
 * McCreight calls this function "rescanning" */
treepoint_t fastScan(const char *str, const range_t *ss, treenode_t *tree)
{
    range_t sfx = *ss;
    treepoint_t pos = {tree, tree, RANGE_LEN(sfx)};
    range_t arc = {0, 0};

    while (sfx.end > sfx.start) {
        pos.parent = pos.child;

        pos.child = pos.parent->first_child;
        while (pos.child) {
            arc = pos.child->arc_val;
            if (str[sfx.start] == str[arc.start])
                break;
            pos.child = pos.child->next_sibling;
        }

        if (!pos.child)
            break;

        int d = pos.arcpos - RANGE_LEN(arc);
        if (d >= 0) {
            pos.arcpos = d;
            if (d == 0) {
                pos.parent = pos.child;
                pos.child = NULL;
                break;
            }
        } else
            break;

        sfx.start += RANGE_LEN(arc);
    }

    return pos;
}


/* Search where the longest prefix of sfx (head(sfx)) is located in the tree.
 * McCreight calls this function "scanning" */
treepoint_t slowScan(const char *str, const range_t *ss, const treepoint_t *start)
{
    range_t sfx = *ss;
    treepoint_t pos = *start;

    while (sfx.end > sfx.start) {
        if (pos.arcpos == 0) {
            /* on an edge */
            pos.child = pos.parent->first_child;
            while (pos.child) {
                if (str[sfx.start] == str[pos.child->arc_val.start])
                    break;
                pos.child = pos.child->next_sibling;
            }

            if (!pos.child)
                break;

        } else {
            /* on an arc */
            if (str[pos.child->arc_val.start + pos.arcpos] != str[sfx.start])
                break;
        }

        pos.arcpos++;
        sfx.start++;

        if (pos.arcpos == RANGE_LEN(pos.child->arc_val)) {
            /* from an arc to the next edge */
            pos.parent = pos.child;
            pos.arcpos = 0;
        }
    }

    return pos;
}


treenode_t *splitAtPoint(const treepoint_t *pos)
{
    if (pos->arcpos == 0)
        return pos->parent;

    treenode_t *new = malloc(sizeof(treenode_t));

    if (pos->child->prev_sibling == NULL)
        pos->parent->first_child = new;
    else
        pos->child->prev_sibling->next_sibling = new;

    if (pos->child->next_sibling)
        pos->child->next_sibling->prev_sibling = new;
    new->prev_sibling = pos->child->prev_sibling;
    new->next_sibling = pos->child->next_sibling;

    new->first_child = pos->child;
    pos->child->next_sibling = NULL;
    pos->child->prev_sibling = NULL;

    new->parent = pos->parent;
    pos->child->parent = new;

    new->arc_val.start = pos->child->arc_val.start;
    new->arc_val.end = new->arc_val.start + pos->arcpos;
    pos->child->arc_val.start = new->arc_val.end;
    new->node_val.start = pos->child->node_val.start;
    new->node_val.end = new->arc_val.end;

    new->suffix_link = NULL;

    return new;
}


treenode_t *newChild(treenode_t *parent, const range_t *node_val)
{
    treenode_t *new = malloc(sizeof(treenode_t));

    new->next_sibling = parent->first_child;
    new->prev_sibling = NULL;
    new->first_child = NULL;
    if (parent->first_child)
        parent->first_child->prev_sibling = new;
    parent->first_child = new;
    new->parent = parent;
    new->suffix_link = NULL;

    new->node_val = *node_val;
    new->arc_val.end = node_val->end;
    new->arc_val.start = node_val->start + RANGE_LEN(parent->node_val);

    return new;
}

treenode_t *suffixTree_mcCreight(const char *str)
{
    range_t s = {0, strlen(str)+1};
    
    treenode_t *root = calloc(1, sizeof(treenode_t));
    root->suffix_link = root;
    
    newChild(root, &s);
    
    /* Loop invariant: active_node must represent a prefix of the current
     * suffix */
    treenode_t *active_node = root;
    
    /* Suffixes are inserted in order from the longest to the shortest. */
    for (int i=1; i<s.end; i++) {
        range_t suf = {i, s.end};  /* suffix that will be inserted */
        
        /* Search for the current 'head' by scanning until we reach the
         * first character that does not match or we "fall out" of the tree.
         * The 'head' is defined as the longest prefix of the current suffix
         * that is already present in the tree as a prefix of another suffix.
         * 'tail' is the rest of the suffix (the part after the head).
         * gamma is the last part of 'head' that has to be scanned. */
        range_t partial_head = active_node->node_val;
        int gamma_start = RANGE_LEN(partial_head) + i;
        range_t gammatail = {gamma_start, s.end};
        treepoint_t start = {active_node, NULL, 0};
        treepoint_t head = slowScan(str, &gammatail, &start);
        
        /* Split where 'head' ends and insert the new suffix */
        active_node = splitAtPoint(&head);
        newChild(active_node, &suf);
        
        /* Set the suffix link of the head node, if needed */
        if (active_node->suffix_link == NULL) {
            /* Search the location in the tree of the current head without the
             * first character. Use the suffix link of the parent to speed it up. */
            range_t csialphabeta = active_node->node_val;
            /* alphabeta: current head except the first character (csi).
             * The case where csi is empty is implicitly handled. */
            range_t alphabeta = {csialphabeta.start+1, csialphabeta.end};
            /* beta: portion of head on the last arc in the path to head
             * alpha: alphabeta without beta */
            treenode_t *alpha_node = active_node->parent->suffix_link;
            range_t alpha = alpha_node->node_val;
            range_t beta = {alphabeta.start + RANGE_LEN(alpha), alphabeta.end};
            treepoint_t alphabeta_point = fastScan(str, &beta, alpha_node);
            
            treenode_t *linkt = splitAtPoint(&alphabeta_point);
            active_node->suffix_link = linkt;
        }
        
        /* Lemma 1 in McCreight's paper states that by removing the 1st character
         * from the current prefix's head, we get a prefix of the next suffix's
         * head. Thus we can use the suffix link to speed up the search for
         * the next head. */
        active_node = active_node->suffix_link;
    }
    
    return root;
}


























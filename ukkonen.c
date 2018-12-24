#include "sfxtree.h"
#include "benchmark.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* 
 * Primary reference: Ukkonen, Esko. "On-line construction of suffix trees."
 * Algorithmica 14.3 (1995): 249-260.
 *
 * Ukkonen's original exposition of his algorithm seems pretty unpopular; most
 * resources (and this implementation) actually refer to the explanation by
 * D. Gusfield in "Algorithms on Strings, Trees and Sequences: Computer 
 * Science and Computational Biology", Cambridge University Press, 1997.
 *
 * Another criticality point in Ukkonen's paper is that it introduces "new"
 * tree-manipulation procedures that are actually recombinations of those
 * used by McCreight. This relationship has been proven in a rigorous way
 * by R. Giegerich and S. Kurtz in "From Ukkonen to McCreight and Weiner: A 
 * Unifying View of Linear-Time Suffix Tree Construction".
 */


treenode_t *suffixTree_ukkonen(const char *str)
{
  treenode_t *root = calloc(1, sizeof(treenode_t));
  root->suffix_link = root;
  
  /* Loop invariant: active_pos must be the position in the tree where the
   * preexisting suffix we want to extend ends */
  treepoint_t active_pos = {root, NULL, 0};
  
  int j=0;
  /* i: character read at this stage + 1 */
  for (int i=1; i<2 || str[i-2] != '\0'; i++) {
    range_t cs = {0, i};   /* string that has been read so far */
    
    /* j: starting character of the suffix we are appending str[i-1] to */
    for (; j<i; j++) {
      range_t csfx = {j, cs.end};  /* current suffix */
    
      /* Rule 3: is {j, i} in the tree? */
      range_t rest = {i-1, i};
      active_pos = slowScan(str, &rest, &active_pos);
      
      if (POINT_LEN(active_pos) == RANGE_LEN(csfx)) {
        /* Yes: fast forward rule 3 for all other j's and increment i.
         * 
         * Since all prefixes of cs longer than csfx have already been inserted,
         * we're sure that we'll not be able to insert any suffix shorter than
         * this one. Thus we read the next character and try again to apply
         * rule 3; if it fails, then we've found the branch point for this
         * suffix (rule 2).
         *
         * We will restart at this same suffix (all other longer suffixes - 
         * rule 1 - are already inserted because of the unbounded ranges trick),
         * but it will be one character longer; thus the active_pos invariant
         * is already taken care of. */
        break;
      }
              
      /* Rule 2: {j, i} not in the tree; do the split and add it. This
       * implicitly adds the whole suffix starting from j, thus we can ignore
       * it from now on by incrementing j. */
      active_pos.parent = splitAtPoint(&active_pos);
      range_t suffix = {j, INT_MAX};
      newChild(active_pos.parent, &suffix);
      
      /* Set the suffix link of the new internal node produced by applying 
       * rule 2 (which is currently in active_pos.parent). */
      if (active_pos.parent->suffix_link == NULL) {
        /* Set the suffix link from the new internal node {j, i-1} to 
         * {j+1, i-1}. Use the suffix link of the parent (which points to
         * {j+1, alpha} where alpha < i-1) to speed it up. */
         
        /* v = parent of active_pos.parent
         * s(v) = node linked to v by a suffix link 
         * gamma = arc from v to active_pos.parent */
        range_t s_vgamma = {j+1, i-1};
        treenode_t *s_v_node = active_pos.parent->parent->suffix_link;
        range_t s_v = s_v_node->node_val;
        range_t gamma = {s_vgamma.start + RANGE_LEN(s_v), s_vgamma.end};
        treepoint_t s_vgamma_point = fastScan(str, &gamma, s_v_node);
      
        treenode_t *linkt = splitAtPoint(&s_vgamma_point);
        active_pos.parent->suffix_link = linkt;
      }
        
      /* Follow the suffix link to go to the next suffix to be extended. */
      active_pos.parent = active_pos.parent->suffix_link;
      active_pos.child = NULL;
      active_pos.arcpos = 0;
    }
  }

  return root;
}


int main(int argc, char *argv[])
{
  if (argc < 2) {
    benchmark(suffixTree_ukkonen);
  } else {
    char *str = argv[1];
    treenode_t *t = suffixTree_ukkonen(str);
    printTree(str, t);
  }
  return 0;
}


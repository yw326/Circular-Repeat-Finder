#include "sfxtree.h"
#include "benchmark.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


treenode_t *suffixTree_naive(const char *str)
{
  range_t s = {0, strlen(str)+1};
  
  treenode_t *root = calloc(1, sizeof(treenode_t));
  newChild(root, &s);
  
  for (int i=1; i<s.end; i++) {
    range_t thispfx = {i, s.end};
    treepoint_t start = {root, NULL, 0};
    treepoint_t split = slowScan(str, &thispfx, &start);
    
    treenode_t *n = splitAtPoint(&split);
    newChild(n, &thispfx);
  }
  
  return root;
}


int main(int argc, char *argv[])
{
  if (argc < 2) {
    benchmark(suffixTree_naive);
  } else {
    char *str = argv[1];
    treenode_t *t = suffixTree_naive(str);
    printTree(str, t);
  }
  return 0;
}



//
//  debug_helper.h
//  GeneSeq
//
//  Created by Yiming Wang on 2019/3/11.
//  Copyright © 2019 Yiming Wang. All rights reserved.
//

#ifndef debug_helper_h
#define debug_helper_h

#include <stdio.h>
#include <string.h>
#include "sfxtree.h"
#include "edit_distance.h"
#include "str_helper.h"

#endif /* debug_helper_h */

void printDictionaryInfo(treenode_t *node, int linked_list_num);
void printArcLabel(char *str, treenode_t *node);
void printPathLabel(char *str, treenode_t *node);
void printNodeInfo(treenode_t *node, char *str);
int convertCharToInt(char c);
char convertIntToChar(int i);

void check_direct_pair_distance(char* seq, int start1, int start2, int first_s1_len, int second_s2_len, int s1s2_len);
void check_rc_pair_distance(char* seq, int start1, int start2, int first_s1_len, int second_s2_len, int s1s2_len);

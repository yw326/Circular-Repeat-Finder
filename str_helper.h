//
//  str_helper.h
//  GeneSeq
//
//  Created by Yiming Wang on 2019/3/11.
//  Copyright © 2019 Yiming Wang. All rights reserved.
//

#ifndef str_helper_h
#define str_helper_h

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#endif /* str_helper_h */

typedef struct {
    unsigned long length;
    char* str;
} my_string;

char *returnSubstring(char *str, int start, int length);
char *returnReversedSubstring(char *str, int start, int length);
char *returnReverseComplementSubstring(char *str, int start, int length);
char *returnComplementSubstring(char *str, int start, int length);

int checkTwoStringEqual(char *str1, char *str2, int len);
char *concatenate_two_str(char *s1, char *s2);
char *reverse_str(char* str);

my_string* partition_long_str(char* seq, int num_partitions);
void free_my_strings(my_string *my_strings, int num_my_strings);
void print_my_string(my_string *my_strings, int num_my_strings);
unsigned long to_seq_idx(unsigned long partition_idx, int partition_num, unsigned long partition_size);

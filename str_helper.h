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

char *returnSubstring(char *str, int start, int length);
char *returnReversedSubstring(char *str, int start, int length);
char *returnReverseComplementSubstring(char *str, int start, int length);
char *returnComplementSubstring(char *str, int start, int length);

int checkTwoStringEqual(char *str1, char *str2, int len);
char *concatenate_two_str(char *s1, char *s2);
char *reverse_str(char* str);

//
//  KMP.h
//  GeneSeq
//
//  Created by Yiming Wang on 2019/3/11.
//  Copyright © 2019 Yiming Wang. All rights reserved.
//

#ifndef KMP_h
#define KMP_h

#include <stdio.h>
#include <stdlib.h>

#endif /* KMP_h */

typedef struct {
    int *arr;
    short size;
} arr_with_size;

void computeLPSarray(char *pat, int M, int *lps);
arr_with_size* KMP(char *txt, char *pat, int N, int M);

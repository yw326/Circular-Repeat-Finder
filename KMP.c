//
//  KMP.c
//  GeneSeq
//
//  Created by Yiming Wang on 2019/3/11.
//  Copyright © 2019 Yiming Wang. All rights reserved.
//

#include "KMP.h"

// MARK: KMP algorithm
void computeLPSarray(char *pat, int M, int *lps)
{
    int len = 0;    //lenght of previous longest prefix suffix
    int i ;
    
    lps[0] = 0 ;    //lps[0] is always 0
    i = 1;
    
    while(i <  M)    //loop calculates lps[i] for i = 1 to M-1
    {
        if(pat[i] == pat[len])
        {
            len++;
            lps[i] = len;
            i++;
        }
        else    //pat[i] != pat[len]
        {
            if(len != 0)
            {
                //this is tricky
                //consider the example AAACAAAA and i =7
                len = lps[len - 1];
                //we donot increment i here
            }
            else    //if len == 0
            {
                lps[i] = 0;
                i++;
            }
        }
    }
}

arr_with_size* KMP(char *txt, char *pat, int N, int M) {
    int arr[N];
    int size = 0;
    
    int *lps = (int*)malloc(M*sizeof(int));
    int j = 0;
    
    computeLPSarray(pat, M, lps);
    
    int i = 0;
    while(i < N)
    {
        if(pat[j] == txt[i])
        {
            j++;
            i++;
        }
        if (j == M) {
            arr[size] = i-j;
            size++;
            //printf("Found pattern at index %d\n",i - j);
            j = lps[j-1];
        }
        else if(pat[j] != txt[i])
        {
            if(j != 0)
                j = lps[j-1];
            else
                i++;
        }
    }
    free(lps);
    
    int *result = malloc(sizeof(int)*size);
    for (int i = 0; i < size; i++) {
        result[i] = arr[i];
        //printf("arr %d\n", result[i]);
    }
    arr_with_size *arr_ws = malloc(sizeof(arr_with_size));
    arr_ws->arr = result;
    arr_ws->size = size;
    
    return arr_ws;
    
}

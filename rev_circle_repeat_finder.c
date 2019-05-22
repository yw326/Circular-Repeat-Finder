//
//  rev_circle_repeat_finder.c
//  GeneSeq
//
//  Created by Yiming Wang on 2019/3/18.
//  Copyright © 2019 Yiming Wang. All rights reserved.
//

#include "rev_circle_repeat_finder.h"

/*
 given 3 strings of same len, s1, s2, s3
 get dist(s1, s2^-1), dist(s3, s1^-1)
 
 e.g. (A2,B1,B3)
 
 result[0] index, result[1] sum
 */
int* get_result_minimizing_dist2(char* s1, char* s2, char* s3, int len){
    if (len == 0) {
        int *result = malloc(sizeof(int)*2);
        result[0] = 0;
        result[1] = 0;
        return result;
    }
    char* s2_rev = reverse_str(s2);
    int* v1 = levenshtein(s1, s2_rev, len, len);
    free(s2_rev);
    
    char* s1_rev = reverse_str(s1);
    int* v2_rev = levenshtein(s1_rev, s3, len, len);
    free(s1_rev);
    
    int v2[len];
    for (int i = 0; i < len; i++) {
        v2[i] = v2_rev[len-1-i];
    }
    
    int idx = -1;
    int min = len*3;
    
    for (int i = 0; i < len; i++) {
        if (v1[i] + v2[i] < min) {
            min = v1[i] + v2[i];
            idx = i;
        }
    }
    
    free(v1);
    free(v2_rev);
    
    int *result = malloc(sizeof(int)*2);
    result[0] = idx;
    result[1] = min;
    
    return result;
}



void findReverseApproximateCircleRepeat(tuple *my_result_list, int size, char *str, double mismatch_ratio, int min_check_length, int max_check_length) {
    
    int arr[size];
    unsigned long maximal_repeat_pair_count = 0;
    
    for (int i = 0; i < size; i++) {
        arr[i] = my_result_list[i].size;
        maximal_repeat_pair_count += arr[i];
        
    }
    
    printf("-------------------\n");
    printf("The total number of maximal reverse pair count is %lu\n", maximal_repeat_pair_count);
    
    FILE* index_file = fopen("result/rev_index.txt", "w");
    FILE* seq_file = fopen("result/rev_seq.txt", "w");

    long count = 0;
    long strlength = strlen(str);

    
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < arr[i]; j++) {
            
            triple t = my_result_list[i].result[j];
            int repeat_len = t.length;
            int p1 = t.p1 < t.p2 ? t.p1 : t.p2;
            int p2 = t.p1 > t.p2 ? t.p1 : t.p2;
            

            int right_extension_start1 = p1+repeat_len;
            int right_extension_start2 = p2+repeat_len;
            
            if (right_extension_start2 + max_check_length > strlength || right_extension_start1 + max_check_length > p2) {
                continue;
            }
            

            char* first_right_extension = returnSubstring(str, right_extension_start1, max_check_length);
//            char* second_right_extension_rev = returnReversedSubstring(str, right_extension_start2, max_check_length);

            char* second_right_extension_rev = returnReverseComplementSubstring(str, right_extension_start2, max_check_length);
            char *concatenated_str = concatenate_two_str(first_right_extension, second_right_extension_rev);

            long cat_strlength = strlen(concatenated_str);
            free(first_right_extension);
            free(second_right_extension_rev);

            result_list *cat_results = outputRepeatedPairs(suffixTree_mcCreight(concatenated_str), concatenated_str, min_check_length, 0, 1);

            free(concatenated_str);

            for (int m = 0; m < cat_results->size; m++) {
                for (int n = 0; n < cat_results->result[m].size; n++) {
                    triple cat_t = cat_results->result[m].result[n];
                    

                    int cat_repeat_len = cat_t.length;
                    int cat_p1 = cat_t.p1 < cat_t.p2 ? cat_t.p1 : cat_t.p2;
                    int cat_p2 = cat_t.p1 > cat_t.p2 ? cat_t.p1 : cat_t.p2;
                    
                    //if newly found pair both in one section, continue
                    if (cat_p1 >= max_check_length || cat_p2 <= max_check_length) {
                        continue;
                    }

                    cat_p2 = cat_strlength - 1 - cat_p2 - cat_repeat_len;

                    
                    // we have ... A1 s1 A2 s2 A3 ...B1 s1^(-1) B2 s2^(-1) B3...
                    // rev_distance(A2,B3) + rev_distance(A2,B1) (elementwise), take minimum index i1
                    // rev_distance(B2,A3) + rev_distance(B2,A1) (elementwise), taken minimum index i2
                    // len(A2) = len(B3) = len(B1) = l1
                    // len(A1) = len(B2) = len(A3) = l2
                    int l1 = cat_p1;
                    int l2 = cat_p2;
                    int exact_len = cat_repeat_len + repeat_len;
                    int total_len = exact_len*2 + l1*3 + l2*3;
                    
                    // if A2 and B2 length difference exceed threshold
                    // then edit distance must also exceed threshold
                    if (l1-l2 > mismatch_ratio*total_len || l1-l2 < -mismatch_ratio*total_len) {
                        continue;
                    }
                    
                    
                    
                    char* A2 = returnSubstring(str, right_extension_start1, l1);
                    char* B3 = returnComplementSubstring(str, right_extension_start2 + cat_p2 + cat_repeat_len, l1);
                    char* B1 = returnComplementSubstring(str, p2 - l1, l1);
                    int *result1 = get_result_minimizing_dist2(A2, B1, B3, l1);
                    free(A2); free(B3); free(B1);
                    int i1 = result1[0]; int min1 = result1[1];
                    free(result1);
                    if (min1 > mismatch_ratio*total_len) { continue; }
                    
                    
                    char* A1 = returnSubstring(str, p1 - l2, l2);
                    char* A3 = returnSubstring(str, right_extension_start2+cat_p1+cat_repeat_len, l2);
                    char* B2 = returnComplementSubstring(str, right_extension_start2, l2);
                    int *result2 = get_result_minimizing_dist2(B2, A1, A3, l2);
                    free(A1); free(A3); free(B2);
                    int i2 = result2[0]; int min2 = result2[1];
                    free(result2);

                    double actual_mismatch_ratio = (min1+min2) / ((double) (l1+l2+exact_len) );
                    if (actual_mismatch_ratio > mismatch_ratio) {
                        continue;
                    }
                    
                    // ouput results to file
                    int first_start = p1 - i2;
                    int first_s1_len = i2 + repeat_len + i1;
                    int first_s2_len = l1 - i1 + cat_repeat_len + (l2 - i2);
                    char* first_str = returnSubstring(str, first_start, l1+l2+exact_len);
                    char* first_s1 = returnSubstring(str, first_start, first_s1_len);
                    char* first_s2 = returnSubstring(str, first_start+first_s1_len, first_s2_len);
                    
                    int second_start = p2 - i1;
                    int second_s1_len = first_s1_len;
                    int second_s2_len = first_s2_len;
                    char* second_str = returnSubstring(str, second_start, l1+l2+exact_len);
                    char* second_s1 = returnSubstring(str, second_start, second_s1_len);
                    char* second_s2 = returnSubstring(str, second_start+second_s1_len, second_s2_len);
                    
//                    int dist1 = levenshtein_val(first_s1, returnReverseComplementSubstring(str, second_start, second_s1_len), first_s1_len, second_s1_len);
//                    int dist2 = levenshtein_val(first_s2, returnReverseComplementSubstring(str, second_start+second_s1_len, second_s2_len), first_s2_len, second_s2_len);
//                    printf("%f\n", (double) (dist1+dist2)/(l1+l2+exact_len));
                    
                    fprintf(index_file, "(%d,%d,%d,%d,%d,%d,%f,%d)\n", first_start, second_start, first_s1_len, first_s2_len, second_s1_len, second_s2_len, actual_mismatch_ratio, l1+l2+exact_len);
                    
                    fprintf(seq_file, ">seq%ld 1st section\n", count);
                    fprintf(seq_file, "%s\n", first_str);
                    fprintf(seq_file, ">seq%ld 1st section s1\n", count);
                    fprintf(seq_file, "%s\n", first_s1);
                    fprintf(seq_file, ">seq%ld 1st section s2\n", count);
                    fprintf(seq_file, "%s\n", first_s2);
                    free(first_str); free(first_s1); free(first_s2);
                    
                    
                    fprintf(seq_file, ">seq%ld 2nd section\n", count);
                    fprintf(seq_file, "%s\n", second_str);
                    fprintf(seq_file, ">seq%ld 2nd section s1\n", count);
                    fprintf(seq_file, "%s\n", second_s1);
                    fprintf(seq_file, ">seq%ld 2nd section s2\n", count);
                    fprintf(seq_file, "%s\n", second_s2);
                    free(second_str); free(second_s1); free(second_s2);
                    fprintf(seq_file, "\n");
                    
                    count++;
                    
                }
            }
            free_results(cat_results);
        }
    }

    fclose(seq_file);
    fclose(index_file);

    
    printf("the number of cirlce repeat is %ld \n", count);

    
}

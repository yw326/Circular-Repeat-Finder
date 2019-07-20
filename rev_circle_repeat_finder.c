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
    char* s1_rev = reverse_str(s1);
    char* s2_rev = reverse_str(s2);
//    char* s1_rev = returnReverseComplementSubstring(s1, 0, len);
//    char* s2_rev = returnReverseComplementSubstring(s2, 0, len);
    int* v1 = levenshtein(s1, s2_rev, len, len);
    free(s2_rev);
    
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
    
    int *result = malloc(sizeof(int)*2);
    result[0] = idx;
    result[1] = v1[idx];
    result[2] = v2[idx];
    
    free(v1);
    free(v2_rev);
    
    return result;
}



void findReverseApproximateCircleRepeat(triple_list *my_result_list, int size, char *str, double mismatch_ratio, int min_check_length, int max_check_length, char* output_file_path, int task, int pound_idx, int partition_num1, int partition_num2, unsigned long partition_size, int one_nbhd) {
    
    char* nc_seq = getStrFromFile("chrY_prefiltered.mask");
    
    int arr[size];
    unsigned long maximal_repeat_pair_count = 0;
    
    for (int i = 0; i < size; i++) {
        arr[i] = my_result_list[i].size;
        maximal_repeat_pair_count += arr[i];
    }
    
    printf("The total number of maximal reverse pair count is %lu\n", maximal_repeat_pair_count);
    
    FILE* index_file = fopen(output_file_path, "w");
    long count = 0;
    long strlength = strlen(str);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < arr[i]; j++) {
            triple t = my_result_list[i].result[j];
            int repeat_len = t.length;
            int p1 = t.p1; int p2 = t.p2;
            
            int right_extension_start1 = p1+repeat_len;
            int right_extension_start2 = p2+repeat_len;
            
            int right_limit = partition_num1 != partition_num2 ? strlength : strlength/2;
            if (right_extension_start2 + max_check_length > right_limit || right_extension_start1 + max_check_length > p2) {
                continue;
            }
            
            char* first_right_extension = returnSubstring(str, right_extension_start1, max_check_length);
            char* second_right_extension_rev = returnReverseComplementSubstring(str, right_extension_start2, max_check_length);
            char *concatenated_str = concatenate_two_str(first_right_extension, second_right_extension_rev);
            long first_right_ext_len = strlen(first_right_extension);
            treenode_t *root = suffixTree_mcCreight(concatenated_str);

            result_list *cat_results = outputRepeatedPairs(root, concatenated_str, min_check_length, 1, 1, first_right_ext_len);

            free(first_right_extension);
            free(second_right_extension_rev);
            freeTree(root);

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

                    //cat_p2 = strlen(concatenated_str) - cat_p2 - cat_repeat_len;
                    cat_p2 = cat_p2 - max_check_length - 1;
                    
                    
                    // we have ... A1 s1 A2 s2 A3 ...B1 s1' B2 s2' B3...
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
                    
                    int s1s2_len = l1+l2+exact_len;
                    
                    char* A2 = returnSubstring(str, right_extension_start1, l1);
                    char* B3 = returnComplementSubstring(str, right_extension_start2 + cat_p2 + cat_repeat_len, l1);
                    char* B1 = returnComplementSubstring(str, p2 - l1, l1);
                    int *result1 = get_result_minimizing_dist2(A2, B1, B3, l1);
                    free(A2); free(B3); free(B1);
                    int i1 = result1[0]; int s_1_min1 = result1[1]; int s_2_min1 = result1[2];
                    free(result1);
                    if (s_1_min1 > s1s2_len*mismatch_ratio || s_2_min1 > s1s2_len*mismatch_ratio) {
                        continue;
                    }
                    
                    
                    char* A1 = returnSubstring(str, p1 - l2, l2);
                    char* A3 = returnSubstring(str, right_extension_start1+cat_p1+cat_repeat_len, l2);
                    char* B2 = returnComplementSubstring(str, right_extension_start2, l2);
                    int *result2 = get_result_minimizing_dist2(B2, A1, A3, l2);
                    free(A1); free(A3); free(B2);
                    int i2 = result2[0]; int s_1_min2 = result2[1]; int s_2_min2 = result2[2];
                    free(result2);
                    
                    int first_start = p1 - i2;
                    int first_s1_len = i2 + repeat_len + i1;
                    int first_s2_len = l1 - i1 + cat_repeat_len + (l2 - i2);
                    
                    int second_start = p2 - i1;
                    int second_s1_len = first_s1_len;
                    int second_s2_len = first_s2_len;
                    
                    double total_mismatch_ratio = (s_1_min1+s_1_min2+s_2_min1+s_2_min2) / ((double) (l1+l2+exact_len) );
                    double s1_mismatch = (s_1_min1 + s_1_min2) / ((double) first_s1_len);
                    double s2_mismatch = (s_2_min1 + s_2_min2) / ((double) first_s2_len);
                    
                    if (s1_mismatch > mismatch_ratio || s2_mismatch > mismatch_ratio) {
                        continue;
                    }
                    
                    // ouput results to file
                    
//                    char* first_s1 = returnSubstring(str, first_start, first_s1_len);
//                    char* first_s2 = returnSubstring(str, first_start+first_s1_len, first_s2_len);
//                    char* second_s1 = returnReverseComplementSubstring(str, second_start, second_s1_len);
//                    char* second_s2 = returnReverseComplementSubstring(str, second_start+second_s1_len, second_s2_len);
//
//                    int dist1 = levenshtein_val(first_s1, second_s1, first_s1_len, second_s1_len);
//                    int dist2 = levenshtein_val(first_s2, second_s2, first_s2_len, second_s2_len);
//                    printf("%f\n", (double) (dist1+dist2)/(l1+l2+exact_len));
                    
                    if (task == 1) {
                        first_start = to_seq_idx(first_start, partition_num1, partition_size);
                        if (partition_num1 == partition_num2) {
                            second_start = to_seq_idx(second_start, partition_num2, partition_size);
                        } else {
                            second_start = to_seq_idx(second_start - pound_idx - 1, partition_num2, partition_size);
                        }
                    }
                    
//                    printf("--------------------1\n");
//                    
//                    printf("(%d,%d,%d,%d,%d,%d,%f,%f,%f,%d)\n", first_start, second_start, first_s1_len, first_s2_len, second_s1_len, second_s2_len, total_mismatch_ratio, s1_mismatch, s2_mismatch, l1+l2+exact_len);
//                    check_rc_pair_distance(nc_seq, first_start, second_start, first_s1_len, second_s2_len, s1s2_len);

                    fprintf(index_file, "(%d,%d,%d,%d,%d,%d,%f,%f,%f,%d)\n", first_start, second_start, first_s1_len, first_s2_len, second_s1_len, second_s2_len, total_mismatch_ratio, s1_mismatch, s2_mismatch, l1+l2+exact_len);
                    
                    count++;
                    
                }
            }
            free_results(cat_results);
            
            
            
            
            if (one_nbhd == 1) {
                continue;
            }
            
            
            int left_extension_start1 = p1-max_check_length;
            int left_extension_start2 = p2-max_check_length;
            
            if (left_extension_start1 < 0 || left_extension_start2 <= p1 + repeat_len) {
                continue;
            }
            
            char* first_left_extension = returnSubstring(str, left_extension_start1, max_check_length);
            char* second_left_extension_rev = returnReverseComplementSubstring(str, left_extension_start2, max_check_length);
            char *concatenated_str2 = concatenate_two_str(first_left_extension, second_left_extension_rev);
            treenode_t *root2 = suffixTree_mcCreight(concatenated_str2);
            
            result_list *cat_results2 = outputRepeatedPairs(root2, concatenated_str2, min_check_length, 1, 1, max_check_length);
            
            free(first_left_extension);
            free(second_left_extension_rev);
            freeTree(root2);
            
            free(concatenated_str2);
            
            for (int m = 0; m < cat_results2->size; m++) {
                for (int n = 0; n < cat_results2->result[m].size; n++) {
                    triple cat_t = cat_results2->result[m].result[n];
                    
                    
                    int cat_repeat_len = cat_t.length;
                    int cat_p1 = cat_t.p1 < cat_t.p2 ? cat_t.p1 : cat_t.p2;
                    int cat_p2 = cat_t.p1 > cat_t.p2 ? cat_t.p1 : cat_t.p2;
                    
                    
                    
                    //if newly found pair both in one section, continue
                    if (cat_p1 >= max_check_length || cat_p2 <= max_check_length) {
                        continue;
                    }
                    
                    //cat_p2 = strlen(concatenated_str) - cat_p2 - cat_repeat_len;
                    cat_p2 = cat_p2 - max_check_length - 1;
                    
                    
                    // we have ... A1 s1 A2 s2 A3 ...B1 s1' B2 s2' B3...
                    // rev_distance(A2,B3) + rev_distance(A2,B1) (elementwise), take minimum index i1
                    // rev_distance(B2,A3) + rev_distance(B2,A1) (elementwise), taken minimum index i2
                    // len(A2) = len(B3) = len(B1) = l1
                    // len(A1) = len(B2) = len(A3) = l2
                    int l1 = max_check_length - cat_p1 - cat_repeat_len;
                    int l2 = max_check_length - cat_p2 - cat_repeat_len;
                    int exact_len = cat_repeat_len + repeat_len;
                    int total_len = exact_len*2 + l1*3 + l2*3;
                    
                    
                    // if A2 and B2 length difference exceed threshold
                    // then edit distance must also exceed threshold
                    if (l1-l2 > mismatch_ratio*total_len || l1-l2 < -mismatch_ratio*total_len) {
                        continue;
                    }
                    
                    int s1s2_len = l1+l2+exact_len;
                    
                    char* A2 = returnSubstring(str, left_extension_start1+cat_p1+cat_repeat_len, l1);
                    char* B3 = returnComplementSubstring(str, p2 + repeat_len, l1);
                    char* B1 = returnComplementSubstring(str, left_extension_start2+cat_p2 - l1, l1);
                    int *result1 = get_result_minimizing_dist2(A2, B1, B3, l1);
                    int i1 = result1[0]; int s_1_min1 = result1[1]; int s_2_min1 = result1[2];
                    free(A2); free(B3); free(B1);
                    free(result1);
                    if (s_1_min1 > s1s2_len*mismatch_ratio || s_2_min1 > s1s2_len*mismatch_ratio) {
                        continue;
                    }
                    
                    char* A1 = returnSubstring(str, left_extension_start1+cat_p1-l2, l2);
                    char* A3 = returnSubstring(str, p1+repeat_len, l2);
                    char* B2 = returnComplementSubstring(str, left_extension_start2+cat_p2+cat_repeat_len, l2);
                    int *result2 = get_result_minimizing_dist2(B2, A1, A3, l2);
                    free(A1); free(A3); free(B2);
                    int i2 = result2[0]; int s_1_min2 = result2[1]; int s_2_min2 = result2[2];
                    free(result2);
                    
                    int first_start =  left_extension_start1+cat_p1 - i2;
                    int first_s1_len = i2 + cat_repeat_len + i1;
                    int first_s2_len = l1 - i1 + repeat_len + (l2 - i2);
                    
                    int second_start = left_extension_start2+cat_p2 - i1;
                    int second_s1_len = first_s1_len;
                    int second_s2_len = first_s2_len;
                    
                    double total_mismatch_ratio = (s_1_min1+s_1_min2+s_2_min1+s_2_min2) / ((double) (l1+l2+exact_len) );
                    double s1_mismatch = (s_1_min1 + s_1_min2) / ((double) first_s1_len);
                    double s2_mismatch = (s_2_min1 + s_2_min2) / ((double) first_s2_len);
                    
                    if (s1_mismatch > mismatch_ratio || s2_mismatch > mismatch_ratio) {
                        continue;
                    }
                    
                    // ouput results to file
                    
//                    char* first_s1 = returnSubstring(str, first_start, first_s1_len);
//                    char* first_s2 = returnSubstring(str, first_start+first_s1_len, first_s2_len);
//                    char* second_s1 = returnReverseComplementSubstring(str, second_start, second_s1_len);
//                    char* second_s2 = returnReverseComplementSubstring(str, second_start+second_s1_len, second_s2_len);
//
//                    int dist1 = levenshtein_val(first_s1, second_s1, first_s1_len, second_s1_len);
//                    int dist2 = levenshtein_val(first_s2, second_s2, first_s2_len, second_s2_len);
//                    printf("%f\n", (double) (dist1+dist2)/(l1+l2+exact_len));

                    
                    if (task == 1) {
                        first_start = to_seq_idx(first_start, partition_num1, partition_size);
                        if (partition_num1 == partition_num2) {
                            second_start = to_seq_idx(second_start, partition_num2, partition_size);
                        } else {
                            second_start = to_seq_idx(second_start - pound_idx - 1, partition_num2, partition_size);
                        }
                    }
                    
//                    printf("--------------------2\n");
//
//                    printf("(%d,%d,%d,%d,%d,%d,%f,%f,%f,%d)\n", first_start, second_start, first_s1_len, first_s2_len, second_s1_len, second_s2_len, total_mismatch_ratio, s1_mismatch, s2_mismatch, l1+l2+exact_len);
//                    check_rc_pair_distance(nc_seq, first_start, second_start, first_s1_len, second_s2_len, s1s2_len);
                    
                    fprintf(index_file, "(%d,%d,%d,%d,%d,%d,%f,%f,%f,%d)\n", first_start, second_start, first_s1_len, first_s2_len, second_s1_len, second_s2_len, total_mismatch_ratio, s1_mismatch, s2_mismatch, l1+l2+exact_len);
                    
                    count++;
                    
                }
            }
            free_results(cat_results2);
        }
    }
    fclose(index_file);
    
    printf("the number of cirlce repeat is %ld \n", count);

    
}

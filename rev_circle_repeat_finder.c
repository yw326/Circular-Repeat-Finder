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
    
    free(v1);
    free(v2_rev);
    
    int *result = malloc(sizeof(int)*2);
    result[0] = idx;
    result[1] = min;
    
    return result;
}



void findReverseApproximateCircleRepeat(triple_list *my_result_list, int size, char *str, double mismatch_ratio, int min_check_length, int max_check_length, char* output_file_path, int task, int pound_idx, int partition_num1, int partition_num2, unsigned long partition_size) {
    
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
            long cat_strlength = strlen(concatenated_str);
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
                    
                    

//                    cat_p2 = cat_strlength - 1 - cat_p2 - cat_repeat_len;
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
                    
                    
                    
                    char* A2 = returnSubstring(str, right_extension_start1, l1);
                    char* B3 = returnComplementSubstring(str, right_extension_start2 + cat_p2 + cat_repeat_len, l1);
                    char* B1 = returnComplementSubstring(str, p2 - l1, l1);
                    int *result1 = get_result_minimizing_dist2(A2, B1, B3, l1);
                    free(A2); free(B3); free(B1);
                    int i1 = result1[0]; int min1 = result1[1];
                    free(result1);
                    if (min1 > mismatch_ratio*total_len) { continue; }
                    
                    
                    char* A1 = returnSubstring(str, p1 - l2, l2);
                    char* A3 = returnSubstring(str, right_extension_start1+cat_p1+cat_repeat_len, l2);
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
                    
                    int second_start = p2 - i1;
                    int second_s1_len = first_s1_len;
                    int second_s2_len = first_s2_len;
                    
//                    char* first_s1 = returnSubstring(str, first_start, first_s1_len);
//                    char* first_s2 = returnSubstring(str, first_start+first_s1_len, first_s2_len);
//                    char* second_s1 = returnReverseComplementSubstring(str, second_start, second_s1_len);
//                    char* second_s2 = returnReverseComplementSubstring(str, second_start+second_s1_len, second_s2_len);
//                    printf("*****\n");
//                    printf("%s\n", returnSubstring(str, first_start, first_s1_len+first_s2_len));
//                    printf("%s\n", returnSubstring(str, second_start, second_s1_len+second_s2_len));
//                    printf("%s\n", first_s1);
//                    printf("%s\n", second_s1);
//                    printf("%s\n", first_s2);
//                    printf("%s\n", second_s2);
//                    printf("%d,%d\n",min1,min2);
//                    printf("%s,%s\n", returnSubstring(str, p1, t.length), returnSubstring(str, p2, t.length));
//                    printf("%s,%s\n", returnSubstring(str, right_extension_start1+cat_p1, cat_repeat_len), returnSubstring(str, right_extension_start2+cat_p2, cat_repeat_len));
//                    printf("A1: %s\n", A1);
//                    printf("A3: %s\n", A3);
//                    printf("B2: %s\n", B2);
//                    printf("l2: %d\n", l2);


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

                    fprintf(index_file, "(%d,%d,%d,%d,%d,%d,%f,%d)\n", first_start, second_start, first_s1_len, first_s2_len, second_s1_len, second_s2_len, actual_mismatch_ratio, l1+l2+exact_len);
                    
                    count++;
                    
                }
            }
            free_results(cat_results);
        }
    }
    fclose(index_file);
    
    printf("the number of cirlce repeat is %ld \n", count);

    
}

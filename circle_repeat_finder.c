//
//  circle_repeat_finder.c
//  GeneSeq
//
//  Created by Yiming Wang on 2019/3/11.
//  Copyright © 2019 Yiming Wang. All rights reserved.
//

#include "circle_repeat_finder.h"

int using_slurm = 0;

/*
 given 3 strings of same len, s1, s2, s3
 get edit distance vector v1=rev_d(s1,s2), v2=rev_d(s1,s3)
 return i that mimimizes v1[i]+v2[i] and the sum
 
 result[0] index, result[1] cost sum
 */



int* get_result_minimizing_dist(char* s1, char* s2, char* s3, int len){
    if (len == 0) {
        int *result = malloc(sizeof(int)*2);
        result[0] = 0;
        result[1] = 0;
        return result;
    }
    int* v1 = levenshtein(s1, s2, len, len);

    char* s1_rev = reverse_str(s1);
    char* s3_rev = reverse_str(s3);
    int* v2_rev = levenshtein(s1_rev, s3_rev, len, len);
    free(s1_rev); free(s3_rev);
    
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



void findApproximateCircleRepeat(triple_list *my_result_list, int size, char *str, double mismatch_ratio, int min_extension_len, int max_check_length, char* output_file_path, int task, int pound_idx, int partition_num1, int partition_num2, unsigned long partition_size) {

    int arr[size];
    unsigned long maximal_repeat_pair_count = 0;

    for (int i = 0; i < size; i++) {
        arr[i] = my_result_list[i].size;
        maximal_repeat_pair_count += arr[i];
    }

//    printf("-------------------\n");
    printf("The total number of maximal repeat pair count is %lu\n", maximal_repeat_pair_count);

    int min_check_length = min_extension_len;

    long count = 0;
    long strlength = strlen(str);

//    printf("-------------------\n");
    
    FILE* index_file = fopen(output_file_path, "w");
//    FILE* seq_file = fopen("result/seq.txt", "w");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < arr[i]; j++) {
            
            triple t = my_result_list[i].result[j];
            int repeat_len = t.length;
            int p1 = t.p1; int p2 = t.p2;
            
            /*
             first we look at right extension of first and left extension of second
             and try to maximal repeats
            */
            
            int right_extension_start = p1+repeat_len;
            int left_extension_start = p2-max_check_length;
            
            if (right_extension_start > left_extension_start) {
                // too close: continue?
                // or assign each extension half of the space in between?
                continue;
            }
            
            
            char* first_right_extension = returnSubstring(str, right_extension_start, max_check_length);
            char* second_left_extension = returnSubstring(str, left_extension_start, max_check_length);
            char *concatenated_str = concatenate_two_str(first_right_extension, second_left_extension);
            int first_right_ext_len = strlen(first_right_extension);
            int min_check_length = 5;
            
            treenode_t *root = suffixTree_mcCreight(concatenated_str);
            result_list *cat_results = outputRepeatedPairs(root, concatenated_str, min_check_length, 0, 1, first_right_ext_len);
            freeTree(root);
            
            free(first_right_extension); free(second_left_extension);
            free(concatenated_str);

            //iterate over repeated pairs in concatenated str
            for (int m = 0; m < cat_results->size; m++) {
                for (int n = 0; n < cat_results->result[m].size; n++) {
                    triple cat_t = cat_results->result[m].result[n];
                    int cat_repeat_len = cat_t.length;
                    int cat_p1 = cat_t.p1 < cat_t.p2 ? cat_t.p1 : cat_t.p2;
                    int cat_p2 = cat_t.p1 > cat_t.p2 ? cat_t.p1 : cat_t.p2;

                    cat_p2 = cat_p2 - max_check_length - 1;

                    
                    // we have ... A1 s1 A2 s2 A3 ...B1 s2 B2 s1 B3...
                    // distance(A2,B3) + rev_distance(A2,B1) (elementwise), take minimum index i1
                    // distance(B2,A3) + rev_distance(B2,A1) (elementwise), taken minimum index i2
                    // len(A2) = len(B3) = len(B1) = l1
                    // len(A1) = len(B2) = len(A3) = l2
                    int l1 = cat_p1;
                    int l2 = p2 - (left_extension_start + cat_p2 + cat_repeat_len);
                    int exact_len = cat_repeat_len + repeat_len;
                    int total_len = exact_len*2 + l1*3 + l2*3;
                    
                    // if A2 and B2 length difference exceed threshold
                    // then edit distance must also exceed threshold
                    if (l1-l2 > mismatch_ratio*total_len || l1-l2 < -mismatch_ratio*total_len) {
                        continue;
                    }
                    

                    char* A2 = returnSubstring(str, right_extension_start, l1);
                    char* B3 = returnSubstring(str, p2+repeat_len, l1);
                    char* B1 = returnSubstring(str, left_extension_start+cat_p2-l1, l1);
                    int *result1 = get_result_minimizing_dist(A2, B3, B1, l1);
                    free(A2); free(B3); free(B1);
                    int i1 = result1[0]; int min1 = result1[1];
                    free(result1);
                    if (min1 > mismatch_ratio*total_len) { continue; }


                    char* A1 = returnSubstring(str, p1 - l2, l2);
                    char* A3 = returnSubstring(str, right_extension_start+cat_p1+cat_repeat_len, l2);
                    char* B2 = returnSubstring(str, p2-l2, l2);
                    int *result2 = get_result_minimizing_dist(B2, A3, A1, l2);
                    free(A1); free(A3); free(B2);
                    int i2 = result2[0]; int min2 = result2[1];
                    free(result2);
                    if (min1 + min2 > mismatch_ratio*total_len) { continue; }
                    
                    // ouput results to file
                    int first_start = p1-(l2-i2);
                    int first_s1_len = l2-i2+repeat_len+i1;
                    int first_s2_len = l1 - i1 + cat_repeat_len + i2;
 
                    int second_start = left_extension_start+cat_p2-l1+i1;
                    int second_s2_len = l1 - i1 + cat_repeat_len + i2;
                    int second_s1_len = l2 - i2 + repeat_len + i1;
                    
                
                    double actual_mismatch_ratio = (min1+min2) / ((double) (l1+l2+exact_len) );
                    if (actual_mismatch_ratio > mismatch_ratio) {
                        continue;
                    }
                    
//                    char* first_s1 = returnSubstring(str, first_start, first_s1_len);
//                    char* first_s2 = returnSubstring(str, first_start+first_s1_len, first_s2_len);
//                    char* second_s2 = returnSubstring(str, second_start, second_s2_len);
//                    char* second_s1 = returnSubstring(str, second_start+second_s2_len, second_s1_len);
//
//                    int dist1 = levenshtein_val(first_s1, second_s1, first_s1_len, second_s1_len);
//                    int dist2 = levenshtein_val(first_s2, second_s2, first_s2_len, second_s2_len);
//                    //printf("------------------\n");
////                    printf("min1: %d\n", min1);
////                    printf("min2: %d\n", min2);
////                    printf("dist1: %d\n", dist1);
////                    printf("dist2: %d\n", dist2);
                    
                    
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






























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
        int *result = malloc(sizeof(int)*3);
        result[0] = 0;
        result[1] = 0;
        result[2] = 0;
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
    
//    for (int i = 0; i < len; i++) {
//        if (v2[i] < 0) {
//            printf("%d\n",v2[i]);
//        }
//    }
//
    int idx = -1;
    int min = len*3;
    
    for (int i = 0; i < len; i++) {
        if (v1[i] + v2[i] < min) {
            min = v1[i] + v2[i];
            idx = i;
        }
    }
    
    int *result = malloc(sizeof(int)*3);
    result[0] = idx;
    result[1] = v1[idx];
    result[2] = v2[idx];
//
//    if (min < 0) {
//        printf("min %d\n", min);
//    }
    
    free(v1);
    free(v2_rev);
    
    return result;
}



void findApproximateCircleRepeat(triple_list *my_result_list, int size, char *str, double mismatch_ratio, int min_extension_len, int max_check_length, char* output_file_path, int task, int pound_idx, int partition_num1, int partition_num2, unsigned long partition_size, int one_nbhd) {
    
//    char* nc_seq = getStrFromFile("NC_021868.txt");

    int arr[size];
    unsigned long maximal_repeat_pair_count = 0;

    for (int i = 0; i < size; i++) {
        arr[i] = my_result_list[i].size;
        maximal_repeat_pair_count += arr[i];
    }
    
//    FILE* maximal_repeat_file = fopen("maximal_repeat_output.txt", "w");
//    for (int i = 0; i < size; i++) {
//        for (int j = 0; j < arr[i]; j++) {
//            triple t = my_result_list[i].result[j];
//            fprintf(maximal_repeat_file, "(%d,%d,%d)\n", t.p1, t.p2, t.length);
//        }
//    }
//    fclose(maximal_repeat_file);
    
    printf("The total number of maximal repeat pair count is %lu\n", maximal_repeat_pair_count);
//    return;
    
    int min_check_length = min_extension_len;

    long count = 0;
    long strlength = strlen(str);
    
    FILE* index_file = fopen(output_file_path, "w");
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
                // TO DO:
                // too close: continue?
                // or assign each extension half of the space in between?
                continue;
            }
            
            if (partition_num1 != partition_num2 && (right_extension_start + max_check_length >= strlength/2 || left_extension_start - max_check_length <= strlength/2)) {
                continue;
            }
            
            
            
            char* first_right_extension = returnSubstring(str, right_extension_start, max_check_length);
            char* second_left_extension = returnSubstring(str, left_extension_start, max_check_length);
            char *concatenated_str = concatenate_two_str(first_right_extension, second_left_extension);
            treenode_t *root = suffixTree_mcCreight(concatenated_str);
            result_list *cat_results = outputRepeatedPairs(root, concatenated_str, min_check_length, 0, 1, max_check_length);
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
                    
                    
                    
                    int s1s2_len = l1+l2+exact_len;
                    
                    char* A2 = returnSubstring(str, right_extension_start, l1);
                    char* B3 = returnSubstring(str, p2+repeat_len, l1);
                    char* B1 = returnSubstring(str, left_extension_start+cat_p2-l1, l1);
                    int *result1 = get_result_minimizing_dist(A2, B3, B1, l1);
                    free(A2); free(B3); free(B1);
                    int i1 = result1[0]; int s_1_min1 = result1[1]; int s_2_min1 = result1[2];
                    free(result1);
                    
                    if (s_1_min1 > s1s2_len*mismatch_ratio || s_2_min1 > s1s2_len*mismatch_ratio) {
                        continue;
                    }
                    
                    char* A1 = returnSubstring(str, p1 - l2, l2);
                    char* A3 = returnSubstring(str, right_extension_start+cat_p1+cat_repeat_len, l2);
                    char* B2 = returnSubstring(str, p2-l2, l2);
                    int *result2 = get_result_minimizing_dist(B2, A3, A1, l2);
                    free(A1); free(A3); free(B2);
                    int i2 = result2[0]; int s_2_min2 = result2[1]; int s_1_min2 = result2[2];
                    free(result2);

                    // ouput results to file
                    int first_start = p1-(l2-i2);
                    int first_s1_len = l2-i2+repeat_len+i1;
                    int first_s2_len = l1 - i1 + cat_repeat_len + i2;
 
                    int second_start = left_extension_start+cat_p2-l1+i1;
                    int second_s2_len = l1 - i1 + cat_repeat_len + i2;
                    int second_s1_len = l2 - i2 + repeat_len + i1;
                    
                
                    double total_mismatch_ratio = (s_1_min1+s_1_min2+s_2_min1+s_2_min2) / ((double) (l1+l2+exact_len) );
                    double s1_mismatch = (s_1_min1 + s_1_min2) / ((double) first_s1_len);
                    double s2_mismatch = (s_2_min1 + s_2_min2) / ((double) first_s2_len);
                    
                    if (s1_mismatch > mismatch_ratio || s2_mismatch > mismatch_ratio) {
                        continue;
                    }
                    
                    
                    if (task == 1) {
                        first_start = to_seq_idx(first_start, partition_num1, partition_size);
                        if (partition_num1 == partition_num2) {
                            second_start = to_seq_idx(second_start, partition_num2, partition_size);
                        } else {
                            second_start = to_seq_idx(second_start - pound_idx - 1, partition_num2, partition_size);
                        }
                    }
                    
//                    printf("(%d,%d,%d,%d,%d,%d,%f,%f,%f,%d)\n", first_start, second_start, first_s1_len, first_s2_len, second_s1_len, second_s2_len, total_mismatch_ratio, s1_mismatch, s2_mismatch, l1+l2+exact_len);
                    
//                    printf("--------------------\n");
//                    printf("(%d,%d,%d,%d,%d,%d,%f,%f,%f,%d)\n", first_start, second_start, first_s1_len, first_s2_len, second_s1_len, second_s2_len, total_mismatch_ratio, s1_mismatch, s2_mismatch, l1+l2+exact_len);
//                    printf("(%d,%d,%d)\n",p1,p2,repeat_len);
//                    printf("(%d,%d,%d)\n",cat_p1,cat_p2,cat_repeat_len);
//                    check_direct_pair_distance(nc_seq, first_start, second_start, first_s1_len, second_s2_len, s1s2_len);

                    
                    fprintf(index_file, "(%d,%d,%d,%d,%d,%d,%f,%f,%f,%d)\n", first_start, second_start, first_s1_len, first_s2_len, second_s1_len, second_s2_len, total_mismatch_ratio, s1_mismatch, s2_mismatch, l1+l2+exact_len);
//                    fprintf(index_file, "(%d,%d,%d)\n", p1, p2,repeat_len);
//                    fprintf(index_file, "(%d,%d,%d)\n", right_extension_start+cat_p1, left_extension_start+cat_p2,cat_repeat_len);

                    count++;
                }
            }
            free_results(cat_results);
            
            
            
            
            
            
            if (one_nbhd == 1) {
                continue;
            }
            
            
            
            /*
             ---------------------------------------------------------------------
             then we look at left extension of first and right extension of second
             and try to maximal repeats
             */
            
            int first_left_ext_start = p1-max_check_length;
            int second_right_ext_start = p2+repeat_len;

            if (first_left_ext_start < 0 || second_right_ext_start+max_check_length >= strlength) {
                continue;
            }

            char* first_left_extension = returnSubstring(str, first_left_ext_start, max_check_length);
            char* second_right_extension = returnSubstring(str, second_right_ext_start, max_check_length);
            char *concatenated_str2 = concatenate_two_str(first_left_extension, second_right_extension);
            treenode_t *root2 = suffixTree_mcCreight(concatenated_str2);
            result_list *cat_results2 = outputRepeatedPairs(root2, concatenated_str2, min_check_length, 0, 1, max_check_length);
            freeTree(root2);

            free(first_left_extension); free(second_right_extension);
            free(concatenated_str2);

            //iterate over repeated pairs in concatenated str
            for (int m = 0; m < cat_results2->size; m++) {
                for (int n = 0; n < cat_results2->result[m].size; n++) {
//                    printf("1\n");
                    triple cat_t = cat_results2->result[m].result[n];
                    int cat_repeat_len = cat_t.length;
                    int cat_p1 = cat_t.p1 < cat_t.p2 ? cat_t.p1 : cat_t.p2;
                    int cat_p2 = cat_t.p1 > cat_t.p2 ? cat_t.p1 : cat_t.p2;
                    cat_p2 = cat_p2 - max_check_length - 1;

                    // we have ... A1 s1 A2 s2 A3 ...B1 s2 B2 s1 B3...
                    // distance(A2,B3) + rev_distance(A2,B1) (elementwise), take minimum index i1
                    // distance(B2,A3) + rev_distance(B2,A1) (elementwise), taken minimum index i2
                    // len(A2) = len(B3) = len(B1) = l1
                    // len(A1) = len(B2) = len(A3) = l2
                    int l1 = max_check_length - cat_p1 - cat_repeat_len;
                    int l2 = cat_p2;
                    int exact_len = cat_repeat_len + repeat_len;
                    int total_len = exact_len*2 + l1*3 + l2*3;

                    // if A2 and B2 length difference exceed threshold
                    // then edit distance must also exceed threshold
                    if (l1-l2 > mismatch_ratio*total_len || l1-l2 < -mismatch_ratio*total_len) {
                        continue;
                    }

                    int s1s2_len = l1+l2+exact_len;

                    char* A2 = returnSubstring(str, first_left_ext_start+cat_p1+cat_repeat_len, l1);
                    char* B3 = returnSubstring(str, second_right_ext_start+cat_p2+cat_repeat_len, l1);
                    char* B1 = returnSubstring(str, p2 - l1, l1);
                    int *result1 = get_result_minimizing_dist(A2, B3, B1, l1);
                    free(A2); free(B3); free(B1);
                    int i1 = result1[0]; int s_1_min1 = result1[1]; int s_2_min1 = result1[2];
                    free(result1);
                    if (s_1_min1 > s1s2_len*mismatch_ratio || s_2_min1 > s1s2_len*mismatch_ratio) {
                        continue;
                    }

                    char* A1 = returnSubstring(str, first_left_ext_start+cat_p1 - l2, l2);
                    char* A3 = returnSubstring(str, p1+repeat_len, l2);
                    char* B2 = returnSubstring(str, p2+repeat_len, l2);
                    int *result2 = get_result_minimizing_dist(B2, A3, A1, l2);
                    free(A1); free(A3); free(B2);
                    int i2 = result2[0]; int s_2_min2 = result2[1]; int s_1_min2 = result2[2];
                    free(result2);

                    // ouput results to file
                    int first_start = first_left_ext_start+cat_p1-(l2-i2);
                    int first_s1_len = l2-i2+cat_repeat_len+i1;
                    int first_s2_len = l1 - i1 + repeat_len + i2;

                    int second_start = p2-(l1-i1);
                    int second_s2_len = l1 - i1 + repeat_len + i2;
                    int second_s1_len = l2 - i2 + cat_repeat_len + i1;


                    double total_mismatch_ratio = (s_1_min1+s_1_min2+s_2_min1+s_2_min2) / ((double) (l1+l2+exact_len) );
                    double s1_mismatch = (s_1_min1 + s_1_min2) / ((double) first_s1_len);
                    double s2_mismatch = (s_2_min1 + s_2_min2) / ((double) first_s2_len);

                    if (s1_mismatch > mismatch_ratio || s2_mismatch > mismatch_ratio) {
                        continue;
                    }

                    if (task == 1) {
                        first_start = to_seq_idx(first_start, partition_num1, partition_size);
                        if (partition_num1 == partition_num2) {
                            second_start = to_seq_idx(second_start, partition_num2, partition_size);
                        } else {
                            second_start = to_seq_idx(second_start - pound_idx - 1, partition_num2, partition_size);
                        }
                    }

//                    printf("--------------------\n");
//
//                    printf("(%d,%d,%d,%d,%d,%d,%f,%f,%f,%d)\n", first_start, second_start, first_s1_len, first_s2_len, second_s1_len, second_s2_len, total_mismatch_ratio, s1_mismatch, s2_mismatch, l1+l2+exact_len);
//                    check_direct_pair_distance(nc_seq, first_start, second_start, first_s1_len, second_s2_len, s1s2_len);

                    fprintf(index_file, "(%d,%d,%d,%d,%d,%d,%f,%f,%f,%d)\n", first_start, second_start, first_s1_len, first_s2_len, second_s1_len, second_s2_len, total_mismatch_ratio, s1_mismatch, s2_mismatch, l1+l2+exact_len);
                    
//                    fprintf(index_file, "(%d,%d,%d)\n", p1, p2,repeat_len);
//                    fprintf(index_file, "(%d,%d,%d)\n", first_left_ext_start+cat_p1, second_right_ext_start+cat_p2,cat_repeat_len);
                    
                    count++;

                }
            }

            free_results(cat_results2);
        }
    }
    
    fclose(index_file);
    
    printf("the number of cirlce repeat is %ld \n", count);
}






























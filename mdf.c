#include "sfxtree.h"
#include "maximal_repeat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "file_helper.h"
#include "circle_repeat_finder.h"
#include "debug_helper.h"
#include "rev_circle_repeat_finder.h"
#include "task.h"
#include <sys/stat.h>


//MARK: tests


int is_in_result_list(result_list* r, triple t) {
    for (int i = 0; i < r->size; i++) {
        for (int j = 0; j < r->result[i].size; j++) {
            triple t2 = r->result[i].result[j];
            if (t.p1 == t2.p1 && t.p2 == t2.p2 && t.length == t2.length) {
                return 1;
            }
        }
    }
    return 0;
}

void write_maximal_repeated_pairs_to_file(result_list* r, char* file_name) {
    FILE *f = fopen(file_name, "w");
    for (int i = 0; i < r->size; i++) {
        for (int j = 0; j < r->result[i].size; j++) {
            triple t = r->result[i].result[j];
            fprintf(f, "(%d,%d,%d)\n",t.p1,t.p2,t.length);
        }
    }
    fclose(f);
}

int count_num_results(result_list *r) {
    int count = 0;
    for (int i = 0; i < r->size; i++) {
        count += r->result[i].size;
    }
    return count;
}

int is_complement_char(char a, char b) {
    if (a == 'A' && b == 'T') {
        return 1;
    } else if (a == 'T' && b == 'A') {
        return 1;
    } else if (a == 'C' && b == 'G') {
        return 1;
    } else if (a == 'G' && b == 'C') {
        return 1;
    }
    return 0;
}

void check_maximal_repeats_result(result_list *r, char* str) {
    int not_equal_count = 0;
    int not_maximal_count = 0;
    
    for (int i = 0; i < r->size; i++) {
        for (int j = 0; j < r->result[i].size; j++) {
            triple t = r->result[i].result[j];
            char* s1 = returnSubstring(str, t.p1, t.length);
            char* s2 = returnSubstring(str, t.p2, t.length);
            if (!checkTwoStringEqual(s1, s2, t.length)) {
                not_equal_count++;
            }
            if (str[t.p1-1] == str[t.p2-1] || str[t.p1+t.length] == str[t.p2+t.length]) {
                //                printf("------------\n");
//                                printf("%s\n",s1);
//                                printf("%s\n",s2);
                //                char* s1_extend = returnSubstring(str, t.p1-1, t.length+2);
                //                char* s2_extend = returnSubstring(str, t.p2-1, t.length+2);
                //                printf("%s\n",s1_extend);
                //                printf("%s\n",s2_extend);
                not_maximal_count++;
                //                free(s1_extend); free(s2_extend);
            }
            free(s1); free(s2);
        }
    }
    printf("Not equal count: %d\n", not_equal_count);
    printf("Not maximal count: %d\n", not_maximal_count);
    printf("Number of maximal repeated pairs found: %d\n", count_num_results(r));
}



void check_maximal_rc_result(result_list *r, char* str) {
    int not_equal_count = 0;
    int not_maximal_count = 0;
    
    for (int i = 0; i < r->size; i++) {
        for (int j = 0; j < r->result[i].size; j++) {
            triple t = r->result[i].result[j];
            char* s1 = returnSubstring(str, t.p1, t.length);
            char* s2_rc = returnReverseComplementSubstring(str, t.p2, t.length);
            if (!checkTwoStringEqual(s1, s2_rc, t.length)) {
//                printf("%s\n",s1);
//                printf("%s\n",s2_rc);
                not_equal_count++;
            }
            if ( is_complement_char(str[t.p1-1], str[t.p2+t.length]) || is_complement_char(str[t.p2-1], str[t.p1+t.length])) {
                not_maximal_count++;
            }
            free(s1); free(s2_rc);
        }
    }
    printf("Not equal count: %d\n", not_equal_count);
    printf("Not maximal count: %d\n", not_maximal_count);
    printf("Number of maximal repeated pairs found: %d\n", count_num_results(r));
}

result_list *convert_to_absolute_idx(result_list *r, int s1_num, int s2_num, int partition_len, int second_idx_offset) {
    for (int i = 0; i < r->size; i++) {
        for (int j = 0; j < r->result[i].size; j++) {
            r->result[i].result[j].p1 = to_seq_idx(r->result[i].result[j].p1, s1_num, partition_len);
            r->result[i].result[j].p2 = to_seq_idx(r->result[i].result[j].p2-second_idx_offset, s2_num, partition_len);
        }
    }
    return r;
}


triple* remove_from_rl(result_list *r, result_list **remove, int remove_rl_cnt, int partition_size) {
    triple *final_list = malloc(sizeof(triple)*10000);
    int count = 0;
    int count2 = 0;
    for (int i = 0; i < r->size; i++) {
        for (int j = 0; j < r->result[i].size; j++) {
            triple t = r->result[i].result[j];
            int present = 0;
            for (int k = 0; k < remove_rl_cnt; k++) {
                if (is_in_result_list(remove[k], t) == 1) {
                    present = 1;
                }
            }
            
            if (present == 0) {
                final_list[count].p1 = t.p1;
                final_list[count].p2 = t.p2;
                final_list[count].length = t.length;
                
                // don't need to worry about last partition -- always pass
                int dist1_to_conj = partition_size - (t.p1 % partition_size);
                int dist2_to_conj = partition_size - (t.p2 % partition_size);
                if (dist1_to_conj < t.length || dist2_to_conj < t.length ) {
                    count2++;
                } else {
                    printf("(%d,%d,%d)\n", t.p1, t.p2, t.length);
                }
                count++;
            }
        }
    }
    
    printf("diff is : %d\n",count);
    printf("number of pairs miseed due to occuring at conjunction is : %d\n",count2);
    return final_list;
}


void test_NC_021868_direct() {
    char *s1 = getStrFromFile("NC_021868_split/partition-1.txt");
    char *s2 = getStrFromFile("NC_021868_split/partition-2.txt");
    char *s3 = getStrFromFile("NC_021868_split/partition-3.txt");
    char* seq = getStrFromFile("NC_021868.txt");
    char* seq12 = concatenate_two_str(s1, s2);
    char* seq13 = concatenate_two_str(s1, s3);
    char* seq23 = concatenate_two_str(s2, s3);
    
    treenode_t *t = suffixTree_ukkonen(seq);
    treenode_t *t1 = suffixTree_ukkonen(s1);
    treenode_t *t2 = suffixTree_ukkonen(s2);
    treenode_t *t3 = suffixTree_ukkonen(s3);
    treenode_t *t12 = suffixTree_ukkonen(seq12);
    treenode_t *t13 = suffixTree_ukkonen(seq13);
    treenode_t *t23 = suffixTree_ukkonen(seq23);
    
    int p = strlen(s1);
    result_list *results_seq = outputRepeatedPairs(t, seq, 10, 0, 0, -1);
    result_list *results_1 = convert_to_absolute_idx(outputRepeatedPairs(t1, s1, 10, 0, 0, -1),1,1,p,0);
    result_list *results_2 = convert_to_absolute_idx(outputRepeatedPairs(t12, seq12, 10, 0, 1, p),1,2,p,p+1);
    result_list *results_3 = convert_to_absolute_idx(outputRepeatedPairs(t13, seq13, 10, 0, 1, p),1,3,p,p+1);
    result_list *results_4 = convert_to_absolute_idx(outputRepeatedPairs(t2, s2, 10, 0, 0, -1),2,2,p,0);
    result_list *results_5 = convert_to_absolute_idx(outputRepeatedPairs(t23, seq23, 10, 0, 1, p),2,3,p,p+1);
    result_list *results_6 = convert_to_absolute_idx(outputRepeatedPairs(t3, s3, 10, 0, 0, -1),3,3,p,0);
    
    
    int count1 = count_num_results(results_seq);
    int count2 = count_num_results(results_1) + count_num_results(results_2) + count_num_results(results_3) + count_num_results(results_4) + count_num_results(results_5) + count_num_results(results_6);
    
    result_list **results_par = malloc(sizeof(result_list)*6);
    results_par[0] = results_1;
    results_par[1] = results_2;
    results_par[2] = results_3;
    results_par[3] = results_4;
    results_par[4] = results_5;
    results_par[5] = results_6;
    remove_from_rl(results_seq, results_par, 6, p);
    printf("The total number of maximal repeats (without partition) is %d\n", count1);
    printf("The total number of maximal repeats (with partition) is %d\n", count2);
    printf("partition size is %d\n", p);
    
    freeTree(t); freeTree(t1); freeTree(t2); freeTree(t3); freeTree(t12); freeTree(t13); freeTree(t23);
    free(s1); free(s2); free(s3); free(seq); free(seq12); free(seq13); free(seq23);
    free_results(results_seq);
    free_results(results_1); free_results(results_2);
    free_results(results_3); free_results(results_4); free_results(results_5);
    free_results(results_6);
}




void test_NC_021868_reverse() {
    char *s1_f = getStrFromFile("NC_021868_split/partition-1.txt");
    char *s2_f = getStrFromFile("NC_021868_split/partition-2.txt");
    char *s3_f = getStrFromFile("NC_021868_split/partition-3.txt");
    char *s2_rc = returnReverseComplementSubstring(s2_f, 0, strlen(s2_f));
    char *s3_rc = returnReverseComplementSubstring(s3_f, 0, strlen(s3_f));
    int p = strlen(s1_f);
    
    char* seq12 = concatenate_two_str(s1_f, s2_rc);
    char* seq13 = concatenate_two_str(s1_f, s3_rc);
    char* seq23 = concatenate_two_str(s2_f, s3_rc);
    free(s1_f); free(s2_f); free(s3_f); free(s2_rc); free(s3_rc);

    char *s1 = getConcantenatedReversedComplementStrFromFile("NC_021868_split/partition-1.txt");
    char *s2 = getConcantenatedReversedComplementStrFromFile("NC_021868_split/partition-2.txt");
    char *s3 = getConcantenatedReversedComplementStrFromFile("NC_021868_split/partition-3.txt");

    char* seq = getConcantenatedReversedComplementStrFromFile("NC_021868.txt");
    treenode_t *t = suffixTree_mcCreight(seq);
    treenode_t *t1 = suffixTree_ukkonen(s1);
    treenode_t *t2 = suffixTree_ukkonen(s2);
    treenode_t *t3 = suffixTree_ukkonen(s3);
    treenode_t *t12 = suffixTree_ukkonen(seq12);
    treenode_t *t13 = suffixTree_ukkonen(seq13);
    treenode_t *t23 = suffixTree_ukkonen(seq23);

    // (15065,19225,10)
    result_list *results_seq = outputRepeatedPairs(t, seq, 10, 1, 0, strlen(seq)/2);
    result_list *results_1 = convert_to_absolute_idx(outputRepeatedPairs(t1, s1, 10, 1, 0, p),1,1,p,0);
    result_list *results_2 = convert_to_absolute_idx(outputRepeatedPairs(t12, seq12, 10, 1, 1, p),1,2,p,p+1);
    result_list *results_3 = convert_to_absolute_idx(outputRepeatedPairs(t13, seq13, 10, 1, 1, p),1,3,p,p+1);
    result_list *results_4 = convert_to_absolute_idx(outputRepeatedPairs(t2, s2, 10, 1, 0, p),2,2,p,0);
    result_list *results_5 = convert_to_absolute_idx(outputRepeatedPairs(t23, seq23, 10, 1, 1, p),2,3,p,p+1);
    result_list *results_6 = convert_to_absolute_idx(outputRepeatedPairs(t3, s3, 10, 1, 0, p),3,3,p,0);

    int count1 = count_num_results(results_seq);
    int count2 = count_num_results(results_1) + count_num_results(results_2) + count_num_results(results_3) + count_num_results(results_4) + count_num_results(results_5) + count_num_results(results_6);

    result_list **results_par = malloc(sizeof(result_list)*6);
    results_par[0] = results_1;
    results_par[1] = results_2;
    results_par[2] = results_3;
    results_par[3] = results_4;
    results_par[4] = results_5;
    results_par[5] = results_6;
    remove_from_rl(results_seq, results_par, 6, p);
    printf("The total number of maximal repeats (without partition) is %d\n", count1);
    printf("The total number of maximal repeats (with partition) is %d\n", count2);
    printf("partition size is %d\n", p);

    freeTree(t);
    freeTree(t1); freeTree(t2); freeTree(t3); freeTree(t12); freeTree(t13); freeTree(t23);
    free(s1); free(s2); free(s3); free(seq12); free(seq13); free(seq23);
    free(seq);
    free_results(results_seq);
    free_results(results_1); free_results(results_2);
    free_results(results_3); free_results(results_4); free_results(results_5);
    free_results(results_6);
}

//MARK: main

int main(int argc, char *argv[]) {
//    char* seq = getStrFromFile("NC_021868.txt");

    // arg[1] = reverse or direct
    // -s seq_file name (optional)
    // -t partition_file_directory_path task_numbers (optional)
    // -r first_level_minimum_maximal_repeat_length (optional, default 40)
    // -c second_level_minimum_maximal_repeat_length (optional, defualt 10)
    // -m mismatch_percentage (optional, default 0.1)
    // -e extension length (optional, default 800)
    // NOTE: one and only one of -s and -t must be selected
    
    if (argc < 3) {
        printf("Error: not enough arguments; at least 2 arguments (direct/reversed and one of the -t and -s options), but only found %d\n", argc-1);
        return 1;
    }
    
    // check required arguments
    char* reverse_or_direct = argv[1];
    if (strcmp(reverse_or_direct, "reversed") == 0 || strcmp(reverse_or_direct, "direct") == 0) {
        printf("Finding %s circle repeats\n", reverse_or_direct);
    } else {
        printf("Error: invalid argument. Please put \"direct\" or \"reversed\" on your second argument\n");
        return 1;
    }

    //checking optional arguments
    int min_maximal_len_idx = -1;
    int min_2nd_maximal_len_idx = -1;
    int mis_perc_idx = -1;
    int task_dir_idx = -1;
    int seq_file_idx = -1;
    int extension_idx = -1;
    
    int min_maximal_repeat_len = 40;
    int min_2nd_maximal_repeat_len = 20;
    float mis_perc = 0.1;
    int extension_len = 800;
    char* task_dir;
    char* task_arg;
    char* seq_file_name;


    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-r") == 0) {
            if (i+1 >= argc) {
                printf("Error: no argument found for minimum maximal repeat length\n");
                return 1;
            }

            if (atoi(argv[i+1]) <= 0) {
                printf("Error: invalid argument for minimum maximal repeat length\n");
                return 1;
            } else {
                min_maximal_len_idx = i+1;
                min_maximal_repeat_len = atoi(argv[min_maximal_len_idx]);
            }
        }
        
        if (strcmp(argv[i], "-e") == 0) {
            if (i+1 >= argc) {
                printf("Error: no argument found for extension length\n");
                return 1;
            }
            
            if (atoi(argv[i+1]) <= 0) {
                printf("Error: invalid argument for extension length\n");
                return 1;
            } else {
                extension_idx = i+1;
                extension_len = atoi(argv[extension_idx]);
            }
        }
        
        if (strcmp(argv[i], "-c") == 0) {
            if (i+1 >= argc) {
                printf("Error: no argument found for second minimum maximal repeat\n");
                return 1;
            }
            
            if (atoi(argv[i+1]) <= 0) {
                printf("Error: invalid argument for second minimum maximal repeat length\n");
                return 1;
            } else {
                min_2nd_maximal_len_idx = i+1;
                min_2nd_maximal_repeat_len = atoi(argv[min_2nd_maximal_len_idx]);
            }
        }

        if (strcmp(argv[i], "-m") == 0) {
            if (i+1 >= argc) {
                printf("Error: no argument found for allowed percentage of mismatch\n");
                return 1;
            }

            if (atof(argv[i+1]) > 1.0 || atof(argv[i+1]) < 0) {
                printf("Error: invalid argument for allowed percentage of mismatch\n");
                return 1;
            } else {
                mis_perc_idx = i+1;
                mis_perc = atof(argv[mis_perc_idx]);
            }
        }
        
        if (strcmp(argv[i], "-t") == 0) {
            if (i+2 >= argc) {
                printf("Error: not enough arguments found for -t\n");
                return 1;
            }
            task_dir = argv[i+1];
            task_arg = argv[i+2];
            task_dir_idx = i+1;
            
            struct stat st = {0};
            if (stat(task_dir, &st) == -1) {
                printf("Error: task directory not found.\n");
                return 1;
            }
            if (task_arg[0] == '-') {
                if (task_arg[1] == 's' || task_arg[1] == 't' || task_arg[1] == 'r' || task_arg[1] == 'm' || task_arg[1] == 'e') {
                    printf("Error: not enough arguments found for -t\n");
                    return 1;
                }
            }

        }
        
        if (strcmp(argv[i], "-s") == 0) {
            if (i+1 >= argc) {
                printf("Error: not enough arguments found for number of partitions\n");
                return 1;
            }
            seq_file_name = argv[i+1];
            seq_file_idx = i+1;
        }
    }

    if (min_maximal_len_idx == -1) {
        printf("No minimum maximal repeat length found. Use default value %d\n", min_maximal_repeat_len);
    }
    
    if (min_2nd_maximal_len_idx == -1) {
        printf("No second level minimum maximal repeat length found. Use default value %d\n", min_2nd_maximal_repeat_len);
    }

    if (mis_perc_idx == -1) {
        printf("No allowed mismatch percentage length found. Use default value %f\n", mis_perc);
    }
    
    int one_nbhd;
    if (min_2nd_maximal_repeat_len > min_maximal_repeat_len) {
        min_2nd_maximal_repeat_len = min_maximal_repeat_len;
        printf("second maximal repeat length (l2) is larger than first maximal repeat length (l1), so reset l2 to l1\n");
    }
    one_nbhd = min_2nd_maximal_repeat_len == min_maximal_repeat_len ? 1 : 0;
    
    int reversed = strcmp(reverse_or_direct, "reversed") == 0 ? 1 : 0;
    if (task_dir_idx * seq_file_idx > 0 ) {
        printf("Error: one and only one of the -s and -t option must be selected\n");
        return 1;
    } else if (seq_file_idx > 0) {
        // setup sequence
        char *str;
        str = reversed == 1 ? getConcantenatedReversedComplementStrFromFile(seq_file_name) : getStrFromFile(seq_file_name);
        printf("The length of the processed string is %lu\n", strlen(str));
        
        // build suffix tree
        treenode_t *t = suffixTree_mcCreight(str);
        printf("Suffix tree built\n");
        // find maximal repeated pairs and circle repeats
        result_list *results;
        if (reversed == 1) {
            results = outputRepeatedPairs(t, str, min_maximal_repeat_len, 1, 0, strlen(str)/2);
            findReverseApproximateCircleRepeat(results->result, results->size, str, mis_perc, min_2nd_maximal_repeat_len, extension_len, "result/rev_index.txt", 0, -1, -1, -1,- 1, one_nbhd);
        } else {
            results = outputRepeatedPairs(t, str, min_maximal_repeat_len, 0, 0, -1);
            findApproximateCircleRepeat(results->result, results->size, str, mis_perc, min_2nd_maximal_repeat_len, extension_len, "result/index.txt", 0, -1, -1, -1, -1, one_nbhd);
        }
                
        freeTree(t);
        free_results(results);
        free(str);
        
    } else {
        int num_partitions = count_num_files_in_dir(task_dir);
        task_set *tasks = parse_task_arg(task_arg, num_partitions);
        
        //set up partition size
        char partition_1_path[100];
        snprintf(partition_1_path, 100, "%s/partition-1.txt",task_dir);
        char *temp = getStrFromFile(partition_1_path);
        unsigned long partition_size = strlen(temp);
        free(temp);
        
        char task_output_dir[100];
        snprintf(task_output_dir, 100, "%s/%s-%s",task_dir, task_arg, reverse_or_direct);
        
        struct stat st = {0};
        if (stat(task_output_dir, &st) == -1) {
            mkdir(task_output_dir, 0700);
        } else {
            free_task_set(tasks);
            printf("Task seems to be already performed, because the folder %s already exists. If you want to reperform this task, delete that folder and try again.\n",task_output_dir);
            return 1;
        }
        
        for (int i = 0; i < tasks->num_tasks; i++) {
            char path_buf1[100];
            char path_buf2[100];
            int task_num1 = tasks->tasks[i].str_num_1;
            int task_num2 = tasks->tasks[i].str_num_2;

            int task_num = get_task_num(task_num1, task_num2, num_partitions);
            printf("-------------------\n");
            printf("Performing task %d\n", task_num);
            
            snprintf(path_buf1, 100, "%s/partition-%d.txt",task_dir, task_num1);
            snprintf(path_buf2, 100, "%s/partition-%d.txt",task_dir, task_num2);
            
            if( access(path_buf1, F_OK ) == -1 ) {
                printf("Error: %s is not found.\n", path_buf1);
                return 1;
            }
            if( access(path_buf2, F_OK ) == -1) {
                printf("Error: %s is not found.\n", path_buf2);
                return 1;
            }
            
            char task_output_file_path[200];
            snprintf(task_output_file_path, 200, "%s/task-%d-result(%d,%d).txt", task_output_dir, task_num, task_num1, task_num2);
            
            // setup sequence
            char* seq;
            char* seq1 = getStrFromFile(path_buf1);
            int pound_idx = strlen(seq1);
            
            int cat = task_num1 == task_num2 ? 0 : 1;
            
            if (cat == 0) {
                if (reversed == 0) {
                    // on the same sequence, direct
                    seq = seq1;
                } else {
                    // on the same sequence, reversed
                    seq = getConcantenatedReversedComplementStrFromFile(path_buf1);
                }
            } else {
                char* seq2 = getStrFromFile(path_buf2);
                if (reversed == 0) {
                    // on different sequences, direct
                    seq = concatenate_two_str(seq1, seq2);
                } else {
                    // on different sequences, reversed
                    char* seq2_rc = returnReverseComplementSubstring(seq2, 0, strlen(seq2));
                    seq = concatenate_two_str(seq1, seq2_rc);
                    free(seq2_rc);
                }
                free(seq2);
            }
            
            // build suffix tree and find all maximal repeated pairs
            treenode_t *t = suffixTree_mcCreight(seq);
            printf("Suffix tree built\n");
//            if (task_num == 3) {
//                printf("debug\n");
//                result_list *results2 = outputRepeatedPairs_debug(t, seq, min_maximal_repeat_len, reversed, cat, pound_idx);
//                free(results2);
//            }
            result_list *results = outputRepeatedPairs(t, seq, min_maximal_repeat_len, reversed, cat, pound_idx);
            freeTree(t);
            
            if (reversed == 0) {
                // pound_idx same value as partition size
                findApproximateCircleRepeat(results->result, results->size, seq, mis_perc, min_2nd_maximal_repeat_len, extension_len, task_output_file_path, 1, pound_idx, task_num1, task_num2, partition_size, one_nbhd);
            } else {
                if (cat == 1) {
                    char* seq2 = getStrFromFile(path_buf2);
                    char* seq_for_reversed_cr = concatenate_two_str(seq1, seq2);
                    findReverseApproximateCircleRepeat(results->result, results->size, seq_for_reversed_cr, mis_perc, min_2nd_maximal_repeat_len, extension_len, task_output_file_path, 1, pound_idx, task_num1, task_num2, partition_size, one_nbhd);
                    free(seq2); free(seq_for_reversed_cr);
                } else {
                    char* seq_for_reversed_cr = seq;
                    findReverseApproximateCircleRepeat(results->result, results->size, seq_for_reversed_cr, mis_perc, min_2nd_maximal_repeat_len, extension_len, task_output_file_path, 1, pound_idx, task_num1, task_num2, partition_size, one_nbhd);
                }
                

            }
            free_results(results);
            free(seq1);
            if (cat+reversed != 0) {
                free(seq);
            }

        }
        free_task_set(tasks);
    }

    return 0;
}










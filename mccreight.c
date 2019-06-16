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


// MARK: suffix tree







//MARK: tests

//void test_NC_021868_direct() {
//    char *str = getStrFromFile("NC_021868.txt");
//    printf("string length is %lu\n", strlen(str));
//
//    treenode_t *t = suffixTree_mcCreight(str);
//    result_list *results = outputRepeatedPairs(t, str, 10, 0);
//    findApproximateCircleRepeat(results->result, results->size, str, 0.1, 5, 10);
//
//
//    //findCircleDuplication(results->result, results->size, str, 0, 0.1, 5);
//
////    printf("%s\n", returnSubstring(str, 25542, 15));
////    printf("%s\n", returnSubstring(str, 32029, 15));
////    printf("%d\n", levenshtein("CAATTAGAGC", "CAATTTCTTC", 10, 10));
//
//    free(str);
//    free_results(results);
//
//    /*
//     number of maximal should be 3658
//     number of 1st-type should be >= 9 (depending on approxiamte threshold)
//     number of 2nd-type should be >= 5 (depending on approxiamte threshold)
//     */
//}

//void test_NC_021868_reverse() {
//    //char *str = getConcantenatedReversedStrFromFile("NC_021868.txt");
//    char *str = getConcantenatedReversedComplementStrFromFile("NC_021868.txt");
//    printf("string length is %lu\n", strlen(str));
//
//    treenode_t *t = suffixTree_mcCreight(str);
//    result_list *results = outputRepeatedPairs(t, str, 10, 1);
//    findReverseApproximateCircleRepeat(results->result, results->size, str, 0, 5, 10);
//
//
//    //findCircleDuplication(results->result, results->size, str, 0, 0.1, 5);
//
//    //    printf("%s\n", returnSubstring(str, 25542, 15));
//    //    printf("%s\n", returnSubstring(str, 32029, 15));
//    //    printf("%d\n", levenshtein("CAATTAGAGC", "CAATTTCTTC", 10, 10));
//
//    free(str);
//    free_results(results);
//
//}

//MARK: main

int main(int argc, char *argv[]) {
    
    // arg[1] = reverse or direct
    // -s seq_file name (optional)
    // -t partition_file_directory_path task_numbers (optional)
    // -r minimum_maximal_repeat_length (optional, default 40)
    // -m mismatch_percentage (optional, default 0.1)
    // -e extension length (optional, default 800)
    // NOTE: one and only one of -s and -t must be selected

    
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
    int mis_perc_idx = -1;
    int task_dir_idx = -1;
    int seq_file_idx = -1;
    int extension_idx = -1;
    
    int min_maximal_repeat_len = 40;
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
                printf("Error: invalid argument for minimum maximal repeat length\n");
                return 1;
            } else {
                extension_idx = i+1;
                extension_len = atoi(argv[extension_idx]);
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
            if (i+3 >= argc) {
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

    if (mis_perc_idx == -1) {
        printf("No allowed mismatch percentage length found. Use default value %f\n", mis_perc);
    }
    
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
            findReverseApproximateCircleRepeat(results->result, results->size, str, mis_perc, min_maximal_repeat_len, extension_len, "result/rev_index.txt", 0, -1, -1, -1,- 1);
        } else {
            results = outputRepeatedPairs(t, str, min_maximal_repeat_len, 0, 0, -1);
            findApproximateCircleRepeat(results->result, results->size, str, mis_perc, min_maximal_repeat_len, extension_len, "result/index.txt", 0, -1, -1, -1, -1);
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
            result_list *results = outputRepeatedPairs(t, seq, min_maximal_repeat_len, reversed, cat, pound_idx);
            freeTree(t);

            if (reversed == 0) {
                // pound_idx same value as partition size
                findApproximateCircleRepeat(results->result, results->size, seq, mis_perc, min_maximal_repeat_len, extension_len, task_output_file_path, 1, pound_idx, task_num1, task_num2, partition_size);
            } else {
                char* seq2 = getStrFromFile(path_buf2);
                char* seq_for_reversed_cr = concatenate_two_str(seq1, seq2);
                
                findReverseApproximateCircleRepeat(results->result, results->size, seq_for_reversed_cr, mis_perc, min_maximal_repeat_len, extension_len, task_output_file_path, 1, pound_idx, task_num1, task_num2, partition_size);
                free(seq2); free(seq_for_reversed_cr);
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










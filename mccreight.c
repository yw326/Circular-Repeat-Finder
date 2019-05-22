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

// MARK: suffix tree
/*
 * Primary reference: McCreight, Edward M. "A space-economical suffix tree
 * construction algorithm." Journal of the ACM (JACM) 23.2 (1976): 262-272.
 *
 * The names of most variables are consistent with the terminology used by
 * McCreight.
 */








//MARK: tests

void test_NC_021868_direct() {
    char *str = getStrFromFile("NC_021868.txt");
    printf("string length is %lu\n", strlen(str));
    
    treenode_t *t = suffixTree_mcCreight(str);
    result_list *results = outputRepeatedPairs(t, str, 10, 0, 0);
    findApproximateCircleRepeat(results->result, results->size, str, 0.1, 5, 10);
    
    
    //findCircleDuplication(results->result, results->size, str, 0, 0.1, 5);
    
//    printf("%s\n", returnSubstring(str, 25542, 15));
//    printf("%s\n", returnSubstring(str, 32029, 15));
//    printf("%d\n", levenshtein("CAATTAGAGC", "CAATTTCTTC", 10, 10));
    
    free(str);
    free_results(results);

    /*
     number of maximal should be 3658
     number of 1st-type should be >= 9 (depending on approxiamte threshold)
     number of 2nd-type should be >= 5 (depending on approxiamte threshold)
     */
}

void test_NC_021868_reverse() {
    //char *str = getConcantenatedReversedStrFromFile("NC_021868.txt");
    char *str = getConcantenatedReversedComplementStrFromFile("NC_021868.txt");
    printf("string length is %lu\n", strlen(str));
    
    treenode_t *t = suffixTree_mcCreight(str);
    result_list *results = outputRepeatedPairs(t, str, 10, 1, 0);
    findReverseApproximateCircleRepeat(results->result, results->size, str, 0, 5, 10);
    
    
    //findCircleDuplication(results->result, results->size, str, 0, 0.1, 5);
    
    //    printf("%s\n", returnSubstring(str, 25542, 15));
    //    printf("%s\n", returnSubstring(str, 32029, 15));
    //    printf("%d\n", levenshtein("CAATTAGAGC", "CAATTTCTTC", 10, 10));
    
    free(str);
    free_results(results);
    
    /*
     number of maximal should be 3658
     number of 1st-type should be >= 9 (depending on approxiamte threshold)
     number of 2nd-type should be >= 5 (depending on approxiamte threshold)
     */
}

//MARK: main

int main(int argc, char *argv[]) {

    
    // arg[1] = seq_file name
    // arg[2] = reverse or direct
    // -r minimum maximal repeat length (optional, default 90)
    // -e minimum extension length (optional, default 20)
    // -m mismatch percentage (optional, default 0.1)
    
    // check required arguments
    
    char* seq_file_name = argv[1];
    char* reverse_or_direct = argv[2];

    if( access(seq_file_name, F_OK ) == -1 ) {
        printf("Error: sequence file not found. Please put a valid file name on the first argument.\n");
        return 1;
    }

    if (strcmp(reverse_or_direct, "reversed") == 0 || strcmp(reverse_or_direct, "direct") == 0) {
        printf("Finding %s circle repeats\n", reverse_or_direct);
    } else {
        printf("Error: invalid argument. Please put \"direct\" or \"reversed\" on your second argument\n");
        return 1;
    }

    //checking optional arguments
    int min_maximal_len_idx = -1;
    int min_ext_len_idx = -1;
    int mis_perc_idx = -1;

    int min_maximal_repeat_len = 90;
    int min_extension_len = 20;
    float mis_perc = 0.1;

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
                printf("Error: no argument found for minimum extension length\n");
                return 1;
            }

            if (atoi(argv[i+1]) <= 0) {
                printf("Error: invalid argument for minimum extension length\n");
                return 1;
            } else {
                min_ext_len_idx = i+1;
                min_extension_len = atoi(argv[min_ext_len_idx]);
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
    }

    if (min_maximal_len_idx == -1) {
        printf("No minimum maximal repeat length found. Use default value %d\n", min_maximal_repeat_len);
    }

    if (min_ext_len_idx == -1) {
        printf("No minimum extension length found. Use default value %d\n", min_extension_len);
    }

    if (mis_perc_idx == -1) {
        printf("No allowed mismatch percentage length found. Use default value %f\n", mis_perc);
    }

    // set up sequence based on the reversed or direct
    char *str;
    if (strcmp(reverse_or_direct, "reversed") == 0) {
        //str = getConcantenatedReversedStrFromFile(seq_file_name);
        str = getConcantenatedReversedComplementStrFromFile(seq_file_name);
    }

    if (strcmp(reverse_or_direct, "direct") == 0) {
        str = getStrFromFile(seq_file_name);
    }

    // find reversed/direct circle repeats
    printf("The length of the processed string is %lu\n", strlen(str));
    treenode_t *t = suffixTree_mcCreight(str);
    printf("Suffix tree built\n");

    result_list *results;
    if (strcmp(reverse_or_direct, "reversed") == 0) {
        results = outputRepeatedPairs(t, str, min_maximal_repeat_len, 1, 0);
        findReverseApproximateCircleRepeat(results->result, results->size, str, mis_perc, min_extension_len, 800);
    }

    if (strcmp(reverse_or_direct, "direct") == 0) {
        results = outputRepeatedPairs(t, str, min_maximal_repeat_len, 0, 0);
        findApproximateCircleRepeat(results->result, results->size, str, mis_perc, min_extension_len, 800);

    }

    free_results(results);
    free(str);
    
    //test_NC_021868_direct();
//    test_NC_021868_reverse();
//
//output_two_halves_of_file("chr21_prefiltered.mask", "chr21_mask_first_half.txt", "chr21_mask_second_half.txt");
//
//    printf("%s\n", returnReverseComplementSubstring("ATCGGG", 0, 6));

    return 0;
}










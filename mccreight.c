#include "sfxtree.h"
#include "benchmark.h"
#include "maximal_repeat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


// MARK: suffix tree
/*
 * Primary reference: McCreight, Edward M. "A space-economical suffix tree
 * construction algorithm." Journal of the ACM (JACM) 23.2 (1976): 262-272.
 *
 * The names of most variables are consistent with the terminology used by
 * McCreight.
 */


treenode_t *suffixTree_mcCreight(const char *str)
{
    range_t s = {0, strlen(str)+1};

    treenode_t *root = calloc(1, sizeof(treenode_t));
    root->suffix_link = root;

    newChild(root, &s);

    /* Loop invariant: active_node must represent a prefix of the current
     * suffix */
    treenode_t *active_node = root;

    /* Suffixes are inserted in order from the longest to the shortest. */
    for (int i=1; i<s.end; i++) {
        range_t suf = {i, s.end};  /* suffix that will be inserted */

        /* Search for the current 'head' by scanning until we reach the
         * first character that does not match or we "fall out" of the tree.
         * The 'head' is defined as the longest prefix of the current suffix
         * that is already present in the tree as a prefix of another suffix.
         * 'tail' is the rest of the suffix (the part after the head).
         * gamma is the last part of 'head' that has to be scanned. */
        range_t partial_head = active_node->node_val;
        int gamma_start = RANGE_LEN(partial_head) + i;
        range_t gammatail = {gamma_start, s.end};
        treepoint_t start = {active_node, NULL, 0};
        treepoint_t head = slowScan(str, &gammatail, &start);

        /* Split where 'head' ends and insert the new suffix */
        active_node = splitAtPoint(&head);
        newChild(active_node, &suf);

        /* Set the suffix link of the head node, if needed */
        if (active_node->suffix_link == NULL) {
            /* Search the location in the tree of the current head without the
             * first character. Use the suffix link of the parent to speed it up. */
            range_t csialphabeta = active_node->node_val;
            /* alphabeta: current head except the first character (csi).
             * The case where csi is empty is implicitly handled. */
            range_t alphabeta = {csialphabeta.start+1, csialphabeta.end};
            /* beta: portion of head on the last arc in the path to head
             * alpha: alphabeta without beta */
            treenode_t *alpha_node = active_node->parent->suffix_link;
            range_t alpha = alpha_node->node_val;
            range_t beta = {alphabeta.start + RANGE_LEN(alpha), alphabeta.end};
            treepoint_t alphabeta_point = fastScan(str, &beta, alpha_node);

            treenode_t *linkt = splitAtPoint(&alphabeta_point);
            active_node->suffix_link = linkt;
        }

        /* Lemma 1 in McCreight's paper states that by removing the 1st character
         * from the current prefix's head, we get a prefix of the next suffix's
         * head. Thus we can use the suffix link to speed up the search for
         * the next head. */
        active_node = active_node->suffix_link;
    }

    return root;
}

void printNodeInfo(treenode_t *node, char *str) {
    printArcLabel(str, node);
    printPathLabel(str, node);
}



// MARK: string from file helper
#define MAXCHAR 400000000
char *getStrFromFile(char *file_name) {
    FILE *fp;
    fp = fopen(file_name, "r");

    char *str = malloc(sizeof(char)*MAXCHAR);
    int c;
    int n = 0;
    while ((c = fgetc(fp)) != EOF) {
        if ((char)c == 'A' || (char)c == 'T' || (char)c == 'C' || (char)c == 'G') {
            str[n++] = (char)c;
        }
    }

    str[n] = '\0';
    return str;
}

char *getConcantenatedReversedStrFromFile(char *file_name) {
    FILE *fp;
    fp = fopen(file_name, "r");

    char *str = malloc(sizeof(char)*MAXCHAR*2);
    int c;
    int n = 0;
    while ((c = fgetc(fp)) != EOF) {
        if ((char)c == 'A' || (char)c == 'T' || (char)c == 'C' || (char)c == 'G') {
            str[n++] = (char)c;
        }
    }

    int m = n;
    str[n] = '#';
    //n++;

    while (m != -1) {
        str[n++] = str[m--];
    }

    str[n] = '\0';
    return str;
}

//MARK: tests

void test_NC_021868_direct() {
    //char *str = getStrFromFile("NC_021868.txt");
    char *str = getConcantenatedReversedStrFromFile("NC_021868.txt");
    printf("string length is %lu\n", strlen(str));
    
    treenode_t *t = suffixTree_mcCreight(str);
    outputRepeatedPairs(t, str, 10, 5, 0.1, 0);
    
//    printf("%s\n", returnSubstring(str, 25542, 15));
//    printf("%s\n", returnSubstring(str, 32029, 15));
    //printf("%d\n", levenshtein("CAATTAGAGC", "CAATTTCTTC", 10, 10));
    
    free(str);

    /*
     number of maximal should be 3658
     number of 1st-type should be around 9 (depending on approxiamte threshold)
     number of 2nd-type should be around 5 (depending on approxiamte threshold)
     */
}

//MARK: main
// input a seq file, output a file with "N" deleted
void prefilter_file(char *file_name, char *output_file_name) {
    char* s = getStrFromFile(file_name);
    FILE *f = fopen(output_file_name, "w");
    if (f == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    fprintf(f, ">prefiltered \n");
    fprintf(f, "%s", s);
}

int main(int argc, char *argv[]) {

    //test_NC_021868_direct();
    
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
        str = getConcantenatedReversedStrFromFile(seq_file_name);
    }
    
    if (strcmp(reverse_or_direct, "direct") == 0) {
        str = getStrFromFile(seq_file_name);
    }
    
    // find reversed/direct circle repeats
    printf("The length of the processed string is %lu\n", strlen(str));
    treenode_t *t = suffixTree_mcCreight(str);
    printf("Suffix tree built\n");
    
    if (strcmp(reverse_or_direct, "reversed") == 0) {
        outputRepeatedPairs(t, str, min_maximal_repeat_len, min_extension_len, mis_perc, 1);
    }
    
    if (strcmp(reverse_or_direct, "direct") == 0) {
        outputRepeatedPairs(t, str, min_maximal_repeat_len, min_extension_len, mis_perc, 0);
    }

    free(str);
    


    return 0;
}










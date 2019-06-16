//
//  debug_helper.c
//  GeneSeq
//
//  Created by Yiming Wang on 2019/3/11.
//  Copyright © 2019 Yiming Wang. All rights reserved.
//

#include "debug_helper.h"

int convertCharToInt(char c) {
    if (c == 'A') {
        return 1;
    } else if (c == 'T') {
        return 2;
    } else if (c == 'C') {
        return 3;
    } else if (c == 'G') {
        return 4;
    } else if (c == '$') {
        return 0;
    } else if (c == '#') {
        return 5;
    } else if (c == '!') {
        return 6;
    }
    return -1;
}

char convertIntToChar(int i) {
    if (i == 1) {
        return 'A';
    } else if (i == 2) {
        return 'T';
    } else if (i == 3) {
        return 'C';
    } else if (i == 4) {
        return 'G';
    } else if (i == 0) {
        return '$';
    } else if (i == 5) {
        return '#';
    } else if (i == 6) {
        return '!';
    }
    return 'X';
}



void printArcLabel(char *str, treenode_t *node) {
    int start = node->arc_val.start;
    int end = node->arc_val.end;
    char s[end - start + 1];
    for (int i = start; i < end; i++) {
        s[i - start] = str[i];
    }
    s[end - start] = '\0';
    
    printf("Arc start: %d, end: %d ", start, end);
    printf("%s\n", s);
//    if (end == strlen(str) + 1) {
//        printf("%s\n", s);
//    } else {
//        printf("%s\n", s);
//    }
    
    
}

void printPathLabel(char *str, treenode_t *node) {
    int start = node->node_val.start;
    int end = node->node_val.end;
    char s[end - start + 1];
    for (int i = start; i < end; i++) {
        s[i - start] = str[i];
    }
    s[end - start] = '\0';
    printf("Path start: %d, end: %d ", start, end);
    printf("%s\n", s);
    
//    if (end == strlen(str) + 1) {
//        printf("%s$\n", s);
//    } else {
//        printf("%s\n", s);
//    }
    
}

void printDictionaryInfo(treenode_t *node, int linked_list_num) {
    printf("%d ", node->first_child == NULL);
    printf("The dictionary is { ");
    int count = 0;
    for (int i = 0; i < linked_list_num; i++) {
        
        char c = convertIntToChar(i);
        Node *n = node->node_dic[i].start;
        printf("%c: [", c);
        while (n != NULL) {
            printf("%i, ", n->data);
            n = n->next;
            count++;
        }
        printf("], ");
    }
    printf(" }\n");
    
    //    printf("--------------------------\n");
    //    printf("count is %d", count);
    //    if (count == 0) {
    //        treenode_t *child = node->first_child;
    //        while (child != NULL) {
    ////            printDictionaryInfo(child);
    //            printf("%d", child->first_child == NULL);
    //            child = child->next_sibling;
    //        }
    //    }
    //    printf("--------------------------\n");
    
}

void printNodeInfo(treenode_t *node, char *str) {
    printArcLabel(str, node);
    printPathLabel(str, node);
}


void check_direct_pair_distance(char* seq, int start1, int start2, int first_s1_len, int second_s2_len, int s1s2_len) {
    
    int first_s2_len = s1s2_len - first_s1_len;
    int second_s1_len = s1s2_len - second_s2_len;
    char* first_s1 = returnSubstring(seq, start1, first_s1_len);
    char* first_s2 = returnSubstring(seq, start1+first_s1_len, first_s2_len);
    char* second_s2 = returnSubstring(seq, start2, second_s2_len);
    char* second_s1 = returnSubstring(seq, start2+second_s2_len, second_s1_len);
    free(first_s1); free(first_s2); free(second_s1); free(second_s2);
    
    int dist1 = levenshtein_val(first_s1, second_s1, first_s1_len, second_s1_len);
    int dist2 = levenshtein_val(first_s2, second_s2, first_s2_len, second_s2_len);
    
    printf("the mismatch rate is %f\n", (double) (dist1+dist2) / s1s2_len);
}

void check_rc_pair_distance(char* seq, int start1, int start2, int first_s1_len, int second_s2_len, int s1s2_len) {
    
    int first_s2_len = s1s2_len - first_s1_len;
    int second_s1_len = s1s2_len - second_s2_len;
    char* first_s1 = returnSubstring(seq, start1, first_s1_len);
    char* first_s2 = returnSubstring(seq, start1+first_s1_len, first_s2_len);
    char* second_s1 = returnSubstring(seq, start2, second_s1_len);
    char* second_s2 = returnSubstring(seq, start2+second_s1_len, second_s2_len);
    char* second_s1_rc = returnReverseComplementSubstring(second_s1, 0, second_s1_len);
    char* second_s2_rc = returnReverseComplementSubstring(second_s2, 0, second_s2_len);

    int dist1 = levenshtein_val(first_s1, second_s1_rc, first_s1_len, second_s1_len);
    int dist2 = levenshtein_val(first_s2, second_s2_rc, first_s2_len, second_s2_len);
    
    free(first_s1); free(first_s2); free(second_s1); free(second_s2);
    free(second_s1_rc); free(second_s2_rc);
    
    printf("the mismatch rate is %f\n", (double) (dist1+dist2) / s1s2_len);
}

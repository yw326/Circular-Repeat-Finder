

#include "maximal_repeat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


int maximal_circle_repeat_count = 0;
int reverse_maximal_repeat_count = 0;
short linked_list_num = 6; // A,T,C,G,$ and # if concatenated
int min_extension_check = 20;






//MARK: maximal repeat finding helpers
//only used for reversed repeat finding
int past_pairs_min_index[10000000];
int past_pairs_max_index[10000000];
int past_pairs_len = 0;
triple_list formCartisianProduct(linked_list list1[linked_list_num], linked_list list2[linked_list_num], int length, unsigned long strlen, int reversed, int cat, int pound_idx) {
    
    int max_count = 5000000;
    triple *result = malloc(sizeof(triple)*max_count);
    int count = 0;
    
    
    for (int i = 0; i < linked_list_num; i++) {
        for (int j = 0; j < linked_list_num; j++) {
            if (i == j) { continue; }
            Node *start1 = list1[i].start;
            Node *start2 = list2[j].start;
            
            while(start1 != NULL) {
                while(start2 != NULL) {
                    
                    if (count > max_count) {
                        printf("Min length is too short. Count of triple in a node exceeds %d\n", max_count);
                        break;
                    }
                    
                    int a1 = start1->data < start2->data ? start1->data : start2->data;
                    int a2 = start1->data > start2->data ? start1->data : start2->data;
                    
                    if (a1 == a2) {
                        start2 = start2->next;
                        continue;
                    }
                    
                    if (reversed == 1) {
                        if (a1 < pound_idx && a2 > pound_idx) {
                            if (cat == 0) {
                                a2 = strlen - length - a2;
                                if (a1 == a2) {
                                    start2 = start2->next;
                                    continue;
                                }
                                
                                int min = a1 < a2 ? a1 : a2;
                                int max = a1 < a2 ? a2 : a1;
                                
                                // check if the pair is already found
                                int equal = 0;
                                for (int k = 0; k < past_pairs_len; k++) {
                                    if (min == past_pairs_min_index[k] && max == past_pairs_max_index[k]) {
                                        equal = 1;
                                        break;
                                    }
                                }
                                
                                if (equal == 1) {
                                    start2 = start2->next;
                                    continue;
                                }
                                
                                triple t = {min, max, length};
                                result[count] = t;
                                count++;
                                
                                past_pairs_min_index[past_pairs_len] = min;
                                past_pairs_max_index[past_pairs_len] = max;
                                past_pairs_len++;
                            } else {

                                a2 = strlen - length - a2 + pound_idx;
                                triple t = {a1, a2, length};
                                result[count] = t;
                                count++;
                            }
                        }
                    }
                    
                    
                    if (reversed == 0) {
                        if (cat == 0) {
                            triple t = {a1, a2, length};
                            result[count] = t;
                            count++;
                            
                        } else {
                            if (a1 < pound_idx && a2 > pound_idx) {
                                triple t = {a1, a2, length};
                                result[count] = t;
                                count++;
                            }
                        }
                        
                    }
                    
                    start2 = start2->next;
                }
                start1 = start1->next;
                start2 = list2[j].start;
            }
        }
    }
    
    // return the correct sized triple-list instead of max-sized (saving some space)
    triple *result2 = malloc(sizeof(triple)*count);
    for (int i = 0; i < count; i++) {
        result2[i] = result[i];
    }
    free(result);
    
    triple_list t = {result2, count};
    
    return t;
}

int numberOfChildren(treenode_t *node) {
    int count = 0;
    treenode_t *first_child = node->first_child;
    while (first_child != NULL) {
        first_child = first_child->next_sibling;
        count++;
    }
    return count;
}


linked_list *mergeLinkedLists(linked_list lists[linked_list_num][linked_list_num], int exclusion, int children_count) {
    
    linked_list *merged_list = malloc(sizeof(linked_list)*linked_list_num);
    
    for (int i = 0; i < linked_list_num; i++) {
        merged_list[i].start = NULL;
        merged_list[i].end = NULL;
    }

    for (int i = 0; i < children_count; i++) {
        if (i > exclusion) {
            for (int j = 0; j < linked_list_num; j++) {
                linked_list l = lists[i][j];
                if (l.start == NULL) { continue; }
                if (merged_list[j].start == NULL) {
                    merged_list[j].start = l.start;
                    merged_list[j].end = l.end;
                } else if (merged_list[j].end == NULL) {
                    if (l.end == NULL) {
                        merged_list[j].end = l.start;
                        merged_list[j].start->next = merged_list[j].end;
                    } else {
                        merged_list[j].end = l.end;
                        merged_list[j].start->next = l.start;
                    }
                } else {
                    merged_list[j].end->next = l.start;
                    
                    if (l.end == NULL) {
                        merged_list[j].end = l.start;
                    } else {
                        merged_list[j].end = l.end;
                    }
                }
            }
        }
    }
    
    
    return merged_list;
}


void freeLinkedList(linked_list merged_list[linked_list_num]) {
    
    for (int i = 0; i < linked_list_num; i++) {
        Node *tmp = merged_list[i].start;
        if (tmp == NULL) { continue; }
        //printf("%d", merged_list[i].start == NULL);
        
        while (merged_list[i].start->next != NULL) {
            tmp = merged_list[i].start;
            merged_list[i].start = merged_list[i].start->next;
            free(tmp);
        }
        free(merged_list[i].start);
    }
    
}





//MARK: major function
result_list* outputRepeatedPairs(treenode_t *root, char *str, int threshold, int reversed, int cat, int pound_idx) {
    if (reversed == 0 && cat == 0) {
        // A,T,C,G,$
        linked_list_num = 5;
    } else {
        // A,T,C,G,$,#
        linked_list_num = 6;
    }
    
    unsigned long len = strlen(str);
    treenode_t* s[len*2];
    treenode_t* q[len*2];

    int len_q = 1;
    int len_s = 0;
    q[0] = root;
    
    // bot-up traversal to put nodes in linked list
    while (len_q != 0) {

        treenode_t *node = q[len_q - 1];
        s[len_s] = node;
        len_q--;
        len_s++;
        
        if (node->first_child != NULL) {
            treenode_t *first_child = node->first_child;
            q[len_q] = first_child;
            len_q++;
            while (first_child->next_sibling != NULL) {
                first_child = first_child->next_sibling;
                q[len_q] = first_child;
                len_q++;
            }
        }
    }
    
    long max_result_len = len*2;
    result_list *results = malloc(sizeof(result_list));
    results->result = malloc(sizeof(triple_list)*max_result_len);
    int results_len = 0;

    while (len_s != 0) {
        treenode_t *node = s[len_s-1];

        for (int m = 0; m < linked_list_num; m++) {
            node->node_dic[m].start = NULL;
            node->node_dic[m].end = NULL;
        }
        
        int str_len = node->node_val.end - node->node_val.start;
        if (str_len < threshold) {
            
            treenode_t *child = node->first_child;
            while (child != NULL) {
                freeLinkedList(child->node_dic);
                child = child->next_sibling;
            }
            len_s--;
            continue;
        }
        
        

        //node->done = 1;
        
        // store left char and corresponding list to each node
        // list an array elements, each one is a linked-list
        // use the list to find corresponding pairs
        
        if (node->first_child == NULL) {
            
            // if it's a leaf, create a list with a start node containting the string start position
            
            int start = node->node_val.start;
            int left_char_value = convertCharToInt(str[start - 1]);
            if (start == 0) { left_char_value = 6; }
            
            node->node_dic[left_char_value].start = malloc(sizeof(Node));
            node->node_dic[left_char_value].start->data = start;
            node->node_dic[left_char_value].start->next = NULL;

            
        } else {

            int number_of_children = numberOfChildren(node);
            
            linked_list children_lists[number_of_children][linked_list_num];
            treenode_t *node = s[len_s-1];
            treenode_t *first_child = node->first_child;
            for (int m = 0; m < number_of_children; m++) {
                for (int n = 0; n < linked_list_num; n++) {
                    children_lists[m][n].start = first_child->node_dic[n].start;
                    children_lists[m][n].end = first_child->node_dic[n].end;
                }
                first_child = first_child->next_sibling;
            }
            
            
            //add the repeat pairs to result
            for (int m = 0; m < number_of_children; m++) {
                linked_list *list2 = mergeLinkedLists(children_lists, m, number_of_children);
                triple_list result = formCartisianProduct(children_lists[m], list2, str_len, len, reversed, cat, pound_idx);
                
                if (result.size != 0) {
                    results->result[results_len] = result;
                    results_len++;
                }
                free(list2);
            }
            
            treenode_t *child = node->first_child;
            
            while (child != NULL) {
                
                for (int k = 0; k < linked_list_num; k++) {
                    if (child->node_dic[k].start == NULL) { continue; }
                    
                    if (node->node_dic[k].start == NULL) {
                        // no entry has been recorded here, so create list same as the child entry
                        
                        node->node_dic[k].start = child->node_dic[k].start;
                        node->node_dic[k].end = child->node_dic[k].end;
                        
                    } else if (node->node_dic[k].end == NULL) {
                        // only start node created (i.e. one item in the linked list and end node not created)
                        // check children lists
                        
                        if (child->node_dic[k].end == NULL) {
                            // if children also one has one node (end node not created)
                            // 1. set end to child->start
                            // 2. set start->next to end
                            node->node_dic[k].end = child->node_dic[k].start;
                            node->node_dic[k].start->next = node->node_dic[k].end;
                        } else {
                            // if children has more then one children (end node created)
                            // 1. set end node to child->end
                            // 2. set start->next to child->start
                            
                            node->node_dic[k].end = child->node_dic[k].end;
                            node->node_dic[k].start->next = child->node_dic[k].start;
                        }
                        
                    } else {
                        // the node has an end node and start node
                        // set node->end->next to child->start
                        // check child node
                        node->node_dic[k].end->next = child->node_dic[k].start;
                        if (child->node_dic[k].end == NULL) {
                            node->node_dic[k].end = child->node_dic[k].start;
                        } else {
                            node->node_dic[k].end = child->node_dic[k].end;
                        }
                    }

                }
                
                child = child->next_sibling;
            }
            
        }
        
        len_s--;
    }    
    results->size = results_len;

    return results;
}

void free_results(result_list *results) {
    for (int m = 0; m < results->size; m++) {
        free(results->result[m].result);
    }
    free(results->result);
    free(results);
}






















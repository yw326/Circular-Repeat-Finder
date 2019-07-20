

#include "maximal_repeat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


int maximal_circle_repeat_count = 0;
int reverse_maximal_repeat_count = 0;
short linked_list_num = 6; // A,T,C,G,$ and # if concatenated
int min_extension_check = 20;

int debug_cnt = 0;




// MARK: deallocators
void free_arrlist(arr_list *l) {
    for (int i = 0; i < l->size; i++) {
        if (l->arrs[i].size != 0) {
            free(l->arrs[i].arr);
        }
    }
    free(l->arrs);
    free(l);
}

void freeLinkedList(linked_list merged_list[linked_list_num]) {
    for (int i = 0; i < linked_list_num; i++) {
        
        Node *tmp = merged_list[i].start;
        if (tmp == NULL) { continue; }
        
        while (merged_list[i].start->next != NULL) {
            tmp = merged_list[i].start;
            merged_list[i].start = merged_list[i].start->next;
            free(tmp);
        }
        free(merged_list[i].start);
    }
}

void free_results(result_list *results) {
    for (int m = 0; m < results->size; m++) {
        free(results->result[m].result);
    }
    free(results->result);
    free(results);
}


//MARK: maximal repeat finding helpers

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

//only used for reversed repeat finding
int past_pairs_min_index[10000000];
int past_pairs_max_index[10000000];
int past_pairs_len = 0;
triple_list formCartisianProduct(linked_list list1[linked_list_num], arr_list *list2, int length, unsigned long strlen, int reversed, int cat, int pound_idx) {
    
    int max_count = 5000000;
    triple *result = malloc(sizeof(triple)*max_count);
    int count = 0;
    
    for (int i = 0; i < linked_list_num; i++) {
        for (int j = 0; j < linked_list_num; j++) {
            if (i == j) { continue; }
            
            Node *start1 = list1[i].start;
            int start2_ptr = 0;
            
            while(start1 != NULL) {
                while(start2_ptr < list2->arrs[j].size) {
                    
                    if (count > max_count) {
                        printf("Min length is too short. Count of triple in a node exceeds %d\n", max_count);
                        break;
                    }
                    
                    int data2 = list2->arrs[j].arr[start2_ptr];
                    int a1 = start1->data < data2 ? start1->data : data2;
                    int a2 = start1->data > data2 ? start1->data : data2;
                    
                    if (a1 == a2) {
                        start2_ptr++;
                        continue;
                    }
                    
                    if (reversed == 1) {
                        if (a1 < pound_idx && a2 > pound_idx) {
                            if (cat == 0) {
                                a2 = strlen - length - a2;
                                if (a1 == a2) {
                                    start2_ptr++;
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
                                    start2_ptr++;
                                    continue;
                                }
                                
                                triple t = {min, max, length};
                                result[count] = t;
                                count++;
                                
                                past_pairs_min_index[past_pairs_len] = min;
                                past_pairs_max_index[past_pairs_len] = max;
                                past_pairs_len++;
                            } else {
                                
                                a2 = strlen - length - a2 + pound_idx + 1;
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
                    start2_ptr++;
                }
                start1 = start1->next;
                start2_ptr = 0;
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


arr_list *mergeLinkedLists(linked_list lists[linked_list_num][linked_list_num], int exclusion, int children_count) {
    
    arr_list *merged_list = malloc(sizeof(arr_list));
    merged_list->size = linked_list_num;
    merged_list->arrs = malloc(sizeof(arr_with_size)*linked_list_num);
    short arr_sizes[linked_list_num];
    
    for(int i = 0; i < linked_list_num; i++) {
        arr_sizes[i] = 0;
    }

    for (int i = 0; i < children_count; i++) {
        // note: not i != exclusion, otherwise repetitive
        if (i > exclusion) {
            for (int j = 0; j < linked_list_num; j++) {
                linked_list l = lists[i][j];
                if (l.start == NULL) { continue; }
                if (arr_sizes[j] == 0) {
                    merged_list->arrs[j].arr = malloc(sizeof(int)*5000);
                }
                
                Node *temp = l.start;
                while(temp != NULL) {
                    merged_list->arrs[j].arr[arr_sizes[j]] = temp->data;
                    arr_sizes[j]++;
                    temp = temp->next;
                }
                
            }
        }
    }
    
    for(int i = 0; i < linked_list_num; i++) {
        merged_list->arrs[i].size = arr_sizes[i];
    }
    
    return merged_list;
}





//MARK: output repeats func
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

    
    // put nodes in an array in a bot-up traversal order
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
//            node->node_dic[m].size = 0;
        }
        
        int str_len = node->node_val.end - node->node_val.start;
        if (str_len < threshold) {
            treenode_t *child = node->first_child;
            
            int num_children = 0;
            while (child != NULL) {
//                if (len_s - 1 == 0) {
//                    break;
//                }
                
                if (numberOfChildren(node) == 1) {
                    break;
                }
                
//                printf("y1\n");
//                printf("len_s - 1: %d\n", len_s-1);
//                printf("number of children: %d\n", numberOfChildren(node));
//                if (node->parent) {
//                    printf("paren'ts number of children: %d\n", numberOfChildren(node->parent));
//                }
//                printDictionaryInfo(child, linked_list_num);
                num_children++;
                freeLinkedList(child->node_dic);
                child = child->next_sibling;
                
            }

            len_s--;
            continue;
        }
        
        
        // store left char and corresponding list to each node
        // list an array elements, each one is a linked-list
        // use the list to find corresponding pairs
        
        if (node->first_child == NULL) {
            
            // if it's a leaf, create a list with a start node containting the string start position
            int start = node->node_val.start;
            int left_char_value = convertCharToInt(str[start - 1]);
            if (start == 0) { left_char_value = 0; }
            
            
            
            node->node_dic[left_char_value].start = malloc(sizeof(Node));
            node->node_dic[left_char_value].start->data = start;
            node->node_dic[left_char_value].start->next = NULL;
            node->node_dic[left_char_value].end = NULL;
//            node->node_dic[left_char_value].size = 1;

            
            
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

                arr_list *list2 = mergeLinkedLists(children_lists, m, number_of_children);
                triple_list result = formCartisianProduct(children_lists[m], list2, str_len, len, reversed, cat, pound_idx);

                if (result.size != 0) {
                    results->result[results_len] = result;
                    results_len++;
                }
                free_arrlist(list2);

            }
            
            treenode_t *child = node->first_child;
            
            while (child != NULL) {
                for (int k = 0; k < linked_list_num; k++) {
                    if (child->node_dic[k].start == NULL) { continue; }

                    
                    if (node->node_dic[k].start == NULL) {
                        // no entry has been recorded here, so create list same as the child entry
                        node->node_dic[k].start = child->node_dic[k].start;
                        node->node_dic[k].end = child->node_dic[k].end;
//                        node->node_dic[k].size = child->node_dic[k].size;

                        
                    } else if (node->node_dic[k].end == NULL) {
                        // only start node created (i.e. one item in the linked list and end node not created)
                        // check children lists
                        
                        if (child->node_dic[k].end == NULL) {
                            // if children also one has one node (end node not created)
                            // 1. set end to child->start
                            // 2. set start->next to end
                            node->node_dic[k].end = child->node_dic[k].start;
                            node->node_dic[k].end->next = NULL;
                            node->node_dic[k].start->next = node->node_dic[k].end;

                        } else {
                            // if children has more then one children (end node created)
                            // 1. set end node to child->end
                            // 2. set start->next to child->start
                            
                            node->node_dic[k].end = child->node_dic[k].end;
                            node->node_dic[k].end->next = NULL;
                            node->node_dic[k].start->next = child->node_dic[k].start;
                        }
//                        node->node_dic[k].size += child->node_dic[k].size;

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
                        node->node_dic[k].end->next = NULL;
                    }
//                    node->node_dic[k].size += child->node_dic[k].size;
                    
                }
                
                child = child->next_sibling;
            }

        }
        
        
        len_s--;
    }
    results->size = results_len;
    return results;
}

























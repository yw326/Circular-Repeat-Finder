//
//  str_helper.c
//  GeneSeq
//
//  Created by Yiming Wang on 2019/3/11.
//  Copyright © 2019 Yiming Wang. All rights reserved.
//

#include "str_helper.h"

//MARK: helper string functions
char *returnSubstring(char *str, int start, int length) {
    char *s = malloc(sizeof(char)*(length+1));
    
    for (int i = 0; i < length; i++) {
        s[i] = str[i+start];
    }
    s[length] = '\0';
    //printf("%s\n",s);
    return s;
}

char *returnReversedSubstring(char *str, int start, int length) {
    char *s = malloc(sizeof(char)*(length+1));
    
    for (int i = 0; i < length; i++) {
        s[i] = str[start+length-i-1];
    }
    s[length] = '\0';
    //printf("%s\n",s);
    return s;
}

char *returnComplementSubstring(char *str, int start, int length) {
    char *s = malloc(sizeof(char)*(length+1));
    
    for (int i = 0; i < length; i++) {
        char c = str[start+i];
        if (c == 'A') {
            s[i] = 'T';
        } else if (c == 'T') {
            s[i] = 'A';
        } else if (c == 'C') {
            s[i] = 'G';
        } else if (c == 'G') {
            s[i] = 'C';
        }
    }
    s[length] = '\0';
    //printf("%s\n",s);
    return s;
}

char *returnReverseComplementSubstring(char *str, int start, int length) {
    char *s = malloc(sizeof(char)*(length+1));
    
    for (int i = 0; i < length; i++) {
        char c = str[start+length-i-1];
        if (c == 'A') {
            s[i] = 'T';
        } else if (c == 'T') {
            s[i] = 'A';
        } else if (c == 'C') {
            s[i] = 'G';
        } else if (c == 'G') {
            s[i] = 'C';
        }
    }
    s[length] = '\0';
    //printf("%s\n",s);
    return s;
}

// return s1#s2$
char *concatenate_two_str(char *s1, char *s2) {
    unsigned long s1_len = strlen(s1);
    unsigned long s2_len = strlen(s2);
    char *str3 = malloc(sizeof(char)*(3 + s1_len + s2_len));
    for (int i = 0; i < s1_len; i++) {
        str3[i] = s1[i];
    }

    str3[s1_len] = '#';
    for (int i = 0; i < s2_len; i++) {
        str3[i+s1_len+1] = s2[i];
    }
    str3[s1_len+s2_len+1] = '$';
    str3[s1_len+s2_len+2] = '\0';

    
    return str3;
}

int checkTwoStringEqual(char *str1, char *str2, int len) {
    for (int i = 0; i < len; i++) {
        if (str1[i] != str2[i]) {
            return 0;
        }
    }
    return 1;
}

char *reverse_str(char* str) {
    long len = strlen(str);
    char *result = malloc(sizeof(char)*(len+1));
    
    for (int i = 0; i < len; i++) {
        result[i] = str[len-1-i];
    }
    result[len] = '\0';
    return result;
}

my_string* partition_long_str(char* seq, int num_partitions) {
    my_string *result = malloc(sizeof(my_string)*num_partitions);
    
    // first +1 accounts for '\0'
    // second +1 accounts for partition length rounding
    unsigned long partition_size = (strlen(seq) / num_partitions) + 1 + 1;
    unsigned long seq_len = strlen(seq);
    unsigned long curr_ptr = 0;
    
    for (int i = 0; i < num_partitions; i++){
        char* str = (char*) malloc(partition_size*sizeof(char)+1);
        
        if (i == num_partitions - 1) {
            unsigned long j;
            for (j = 0; curr_ptr < seq_len; j++) {
                str[j] = seq[curr_ptr];
                curr_ptr++;
            }
            str[j+1] = '\0';
            result[i].str = str;
            result[i].length = j;
            break;
        }

        for (unsigned long j = 0; j < partition_size; j++) {
            if (j != partition_size - 1) {
                str[j] = seq[curr_ptr];
                curr_ptr++;
            } else if (j == partition_size - 1) {
                str[j] = '\0';
                result[i].str = str;
                result[i].length = partition_size-1;
            }
        }
    }

    return result;
}

void free_my_strings(my_string *my_strings, int num_my_strings) {
    for (int i = 0; i < num_my_strings; i++) {
        free(my_strings[i].str);
    }
    free(my_strings);
}

void print_my_string(my_string *my_strings, int num_my_strings) {
    for (int i = 0; i < num_my_strings; i++) {
        printf("%s, size %ld\n", my_strings[i].str, my_strings[i].length);
    }
}

unsigned long to_seq_idx(unsigned long partition_idx, int partition_num, unsigned long partition_size) {
    return partition_size * (partition_num - 1) + partition_idx;
}

void print_parse_task_arg_error_msg() {
    printf("Task argument error. Possible formats are:\n");
    printf("(X, Y, a, b are integers)\n");
    printf("-t X-Y \t perform tasks whose task# is in range [X,Y] (inclusive)\n");
    printf("-t X \t perform the task with task# X \n");
    printf("-t (a,b) \t perform the task corresponding to partitions pair (a,b) \n");
    printf("-t [X,Y,...] \t perform tasks whose task# is in the specified array\n");
}


// only care about positive
int is_valid_int(char* str) {
    unsigned long len = strlen(str);
    for (unsigned long i = 0; i < len; i++) {
        if (isdigit(str[i]) == 0 ) {
            return 0;
        }
    }
    return 1;
}

// start inclusive, end exclusive
char* substring_char_arr(char* str, int start, int end) {
    if (start >= end || end > strlen(str)) {
        printf("substring error\n");
        return NULL;
    }
    int str_len = end - start;
    char* result = malloc(sizeof(char)*str_len+1);
    for (int i = 0; i < str_len; i++) {
        result[i] = str[start+i];
    }
    result[str_len] = '\0';
    return result;
}

task_set *parse_task_arg(char* arg, int max_num_partitions) {
    int num_tasks;
    int arg_len = strlen(arg);
    my_task *tasks;
    task_set *result = malloc(sizeof(task_set));
    
    int range_indicator_idx = -1;
    int comma_count = 0;
    int comma_idx = -1;
    for (int i = 0; i < arg_len; i++) {
        if (arg[i] == '-') {
            range_indicator_idx = i;
        }
        if (arg[i] == ',') {
            comma_count++;
            comma_idx = i;
        }
    }
    
    //arg format: X-Y
    if (range_indicator_idx != -1) {
        char *start_s = substring_char_arr(arg, 0, range_indicator_idx);
        char *end_s = substring_char_arr(arg, range_indicator_idx+1, arg_len);
        if (!(is_valid_int(start_s) && is_valid_int(end_s))) {
            printf("Error: invalid integer value for specifying range of tasks.\n");
            return NULL;
        }
        int start = atoi(start_s); int end = atoi(end_s);
        free(start_s); free(end_s);
        if (start > end) {
            printf("Error: when specifying range of tasks, starting < ending.\n");
            return NULL;
        }
        num_tasks = end - start + 1;
        tasks = malloc(sizeof(my_task)*num_tasks);
        for (int i = 0; i < num_tasks; i++) {
            my_task *task = get_task(start+i, max_num_partitions);
            tasks[i].str_num_1 = task->str_num_1;
            tasks[i].str_num_2 = task->str_num_2;
            free(task);
        }
        result->num_tasks = num_tasks;
        result->tasks = tasks;
        return result;
    }

    //arg format: X
    if (is_valid_int(arg)) {
        num_tasks = 1;
        tasks = malloc(sizeof(my_task)*num_tasks);
        my_task *task = get_task(atoi(arg), max_num_partitions);
        tasks[0].str_num_1 = task->str_num_1;
        tasks[0].str_num_2 = task->str_num_2;
        free(task);
        result->tasks = tasks;
        result->num_tasks = 1;
        return result;
    }
    
    //arg format: (a,b)
    if (comma_count == 1 && arg[0] == '(' && arg[arg_len-1] == ')') {
        num_tasks = 1;
        tasks = malloc(sizeof(my_task)*num_tasks);
        char *a = substring_char_arr(arg, 1, comma_idx);
        char *b = substring_char_arr(arg, comma_idx+1, arg_len-1);
        if (!(is_valid_int(a) && is_valid_int(b)) ) {
            printf("Error: invalid integer value for specifying tasks.\n");
            return NULL;
        }
        tasks[0].str_num_1 = atoi(a) < atoi(b) ? atoi(a) : atoi(b);
        tasks[0].str_num_2 = atoi(a) < atoi(b) ? atoi(b) : atoi(a);
        free(a); free(b);
        
        result->tasks = tasks;
        result->num_tasks = num_tasks;
        return result;
    }
    
    //arg format: [X,Y,....]
    if (arg[0] == '[' && arg[arg_len-1] == ']') {
        num_tasks = comma_count + 1;
        tasks = malloc(sizeof(my_task)*num_tasks);
        
        int ptr = 0;
        int start_ptr = 1;
        int count = 0;
        while (comma_count > 0) {
            if (arg[ptr] == ',') {
                char* task_num_str = substring_char_arr(arg, start_ptr, ptr);
                start_ptr = ptr + 1;
                if (!is_valid_int(task_num_str)) {
                    printf("Error: invalid integer value for specifying tasks.\n");
                    return NULL;
                }
                my_task *task = get_task(atoi(task_num_str), max_num_partitions);
                tasks[count].str_num_1 = task->str_num_1;
                tasks[count].str_num_2 = task->str_num_2;
                free(task_num_str); free(task);
                count++;
                comma_count--;
            }
            ptr++;
        }
        char* task_num_str = substring_char_arr(arg, start_ptr, arg_len-1);
        my_task *task = get_task(atoi(task_num_str), max_num_partitions);
        tasks[num_tasks-1].str_num_1 = task->str_num_1;
        tasks[num_tasks-1].str_num_2 = task->str_num_2;
        free(task_num_str); free(task);
        
        result->tasks = tasks;
        result->num_tasks = num_tasks;
        return result;
    }
    
    print_parse_task_arg_error_msg();
    return NULL;
}

void free_task_set(task_set *ts) {
    free(ts->tasks);
    free(ts);
}

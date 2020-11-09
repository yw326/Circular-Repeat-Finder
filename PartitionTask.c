#include "PartitionTask.h"

void print_parse_task_arg_error_msg() {
    printf("Task argument error. Possible formats are:\n");
    printf("(X, Y, a, b are integers)\n");
    printf("-t X-Y \t perform tasks whose task# is in range [X,Y] (inclusive)\n");
    printf("-t X \t perform the task with task# X \n");
    printf("-t (a,b) \t perform the task corresponding to partitions pair (a,b) \n");
    printf("-t [X,Y,...] \t perform tasks whose task# is in the specified array\n");
}

int is_valid_int(char* str) {
    unsigned long len = strlen(str);
    for (unsigned long i = 0; i < len; i++) {
        if (isdigit(str[i]) == 0 ) {
            return 0;
        }
    }
    return 1;
}

task *getTask(int task_num, int max_num_partitions) {

    if (task_num > (max_num_partitions+1)*max_num_partitions/2) {
        printf("Error: task number exceeds possible number of tasks (%d)\n", (max_num_partitions+1)*max_num_partitions/2);
        return NULL;
    }
    
    task *t = malloc(sizeof(task));
    int first_str_num = 1;
    int second_str_num = task_num;
    for (int i = 1; i < max_num_partitions + 1; i++) {
        if (second_str_num - (max_num_partitions - i + 1) <= 0 ) {
            t->partitionNum1 = first_str_num;
            t->partitionNum2 = second_str_num + (i-1);
            t->partitionSize = 0;
            return t;
        } else {
            first_str_num++;
            second_str_num -= max_num_partitions - i + 1;
        }
    }
    return NULL;
}

int getTaskNum(int partitionNum1, int partitionNum2, int maxNumPartitions){
    int task_num = 0;
    int i;
    for (i = 1; i < partitionNum1; i++) {
        task_num += maxNumPartitions - i + 1;
    }
    task_num += partitionNum2 - partitionNum1 + 1;
    return task_num;
}

TaskSet *parseTaskArg(char* arg, int max_num_partitions) {
    int num_tasks;
    int arg_len = strlen(arg);
    task *tasks;
    TaskSet *result = malloc(sizeof(TaskSet));
    
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
        char *start_s = returnSubstring(arg, 0, range_indicator_idx);
        char *end_s = returnSubstring(arg, range_indicator_idx+1, arg_len);
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
        tasks = malloc(sizeof(task)*num_tasks);
        for (int i = 0; i < num_tasks; i++) {
            task *t = getTask(start+i, max_num_partitions);
            tasks[i].partitionNum1 = t->partitionNum1;
            tasks[i].partitionNum2 = t->partitionNum2;
            free(t);
        }
        result->numTasks = num_tasks;
        result->tasks = tasks;
        return result;
    }

    //arg format: X
    if (is_valid_int(arg)) {
        num_tasks = 1;
        tasks = malloc(sizeof(task)*num_tasks);
        task *t = getTask(atoi(arg), max_num_partitions);
        tasks[0].partitionNum1 = t->partitionNum1;
        tasks[0].partitionNum2 = t->partitionNum2;
        free(t);
        result->tasks = tasks;
        result->numTasks = 1;
        return result;
    }
    
    //arg format: (a,b)
    if (comma_count == 1 && arg[0] == '(' && arg[arg_len-1] == ')') {
        num_tasks = 1;
        tasks = malloc(sizeof(task)*num_tasks);
        char *a = returnSubstring(arg, 1, comma_idx);
        char *b = returnSubstring(arg, comma_idx+1, arg_len-1);
        if (!(is_valid_int(a) && is_valid_int(b)) ) {
            printf("Error: invalid integer value for specifying tasks.\n");
            return NULL;
        }
        tasks[0].partitionNum1 = atoi(a) < atoi(b) ? atoi(a) : atoi(b);
        tasks[0].partitionNum2 = atoi(a) < atoi(b) ? atoi(b) : atoi(a);
        free(a); free(b);
        
        result->tasks = tasks;
        result->numTasks = num_tasks;
        return result;
    }
    
    //arg format: [X,Y,....]
    if (arg[0] == '[' && arg[arg_len-1] == ']') {
        num_tasks = comma_count + 1;
        tasks = malloc(sizeof(task)*num_tasks);
        
        int ptr = 0;
        int start_ptr = 1;
        int count = 0;
        while (comma_count > 0) {
            if (arg[ptr] == ',') {
                char* task_num_str = returnSubstring(arg, start_ptr, ptr);
                start_ptr = ptr + 1;
                if (!is_valid_int(task_num_str)) {
                    printf("Error: invalid integer value for specifying tasks.\n");
                    return NULL;
                }
                task *t = getTask(atoi(task_num_str), max_num_partitions);
                tasks[count].partitionNum1 = t->partitionNum1;
                tasks[count].partitionNum2 = t->partitionNum1;
                free(task_num_str); free(t);
                count++;
                comma_count--;
            }
            ptr++;
        }
        char* task_num_str = returnSubstring(arg, start_ptr, arg_len-1);
        task *t = getTask(atoi(task_num_str), max_num_partitions);
        tasks[num_tasks-1].partitionNum1 = t->partitionNum1;
        tasks[num_tasks-1].partitionNum1 = t->partitionNum2;
        free(task_num_str); free(t);
        
        result->tasks = tasks;
        result->numTasks = num_tasks;
        return result;
    }
    
    print_parse_task_arg_error_msg();
    return NULL;
}

void freeTaskSet(TaskSet *ts) {
    free(ts->tasks);
    free(ts);
}


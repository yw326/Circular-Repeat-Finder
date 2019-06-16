//
//  task.c
//  GeneSeq
//
//  Created by Yiming Wang on 2019/5/30.
//  Copyright © 2019 Yiming Wang. All rights reserved.
//

#include "task.h"

my_task *get_task(int task_num, int max_num_partitions) {
    
    if (task_num > (max_num_partitions+1)*max_num_partitions/2) {
        printf("Error: task number exceeds possible number of tasks (%d)\n", (max_num_partitions+1)*max_num_partitions/2);
        return NULL;
    }
    
    my_task *task = malloc(sizeof(my_task));
    int first_str_num = 1;
    int second_str_num = task_num;
    for (int i = 1; i < max_num_partitions + 1; i++) {
        if (second_str_num - (max_num_partitions - i + 1) <= 0 ) {
            task->str_num_1 = first_str_num;
            task->str_num_2 = second_str_num + (i-1);
            return task;
        } else {
            first_str_num++;
            second_str_num -= max_num_partitions - i + 1;
        }
    }
    return NULL;
}

int get_task_num(int str_num_1, int str_num_2, int max_num_partitions) {
    int task_num = 0;
    int i;
    for (i = 1; i < str_num_1; i++) {
        task_num += max_num_partitions - i + 1;
    }
    task_num += str_num_2 - str_num_1 + 1;
    return task_num;
}


void print_task(my_task *task) {
    printf("(%d,%d)\n", task->str_num_1, task->str_num_2);
}


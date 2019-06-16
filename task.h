//
//  task.h
//  GeneSeq
//
//  Created by Yiming Wang on 2019/5/30.
//  Copyright © 2019 Yiming Wang. All rights reserved.
//

#ifndef task_h
#define task_h

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int str_num_1;
    int str_num_2;
} my_task;

typedef struct {
    my_task *tasks;
    int num_tasks;
} task_set;

#endif /* task_h */

my_task *get_task(int task_num, int max_num_partitions);
int get_task_num(int str_num_1, int str_num_2, int max_num_partitions);
void print_task(my_task *task);

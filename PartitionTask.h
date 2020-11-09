#ifndef task_h
#define task_h

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <ctype.h>
#include "Utils.h"

typedef struct {
    int partitionNum1;
    int partitionNum2;
    unsigned long partitionSize;
} task;

typedef struct {
    task *tasks;
    int numTasks;
} TaskSet;

#endif 

task *getTask(int task_num, int max_num_partitions);
int getTaskNum(int partitionNum1, int partitionNum2, int MaxNumPartitions);
void printTask(task *t);
TaskSet *parseTaskArg(char* arg, int max_num_partitions);
void freeTaskSet(TaskSet *ts);
#include "TestPartitionTask.h"

int checkTask(task t, int expectedNum1, int expectedNum2) {
    return t.partitionNum1 == expectedNum1 && t.partitionNum2 == expectedNum2;
}

int testParseTaskArg() {
    // arrange
    int numPartition = 4;
    const char* arg = "3-6";

    // act
    TaskSet *ts = parseTaskArg(arg, 4);

    // assert
    int failed = 0;
    if (checkTask(ts->tasks[0], 1, 3) && checkTask(ts->tasks[1], 1, 4) && checkTask(ts->tasks[2], 2, 2) && checkTask(ts->tasks[3], 2, 3)) {
        printf("Passed: testParseTaskArg\n");
    } else {
        failed++;
        printf("Failed: testParseTaskArg\n");
    }
    
    freeTaskSet(ts);
    
    return failed;
}

int testGetTaskNum() {
    // arrange
    int p1 = 2;
    int p2 = 3;
    int numPartition = 4;

    // act
    int taskNum = getTaskNum(2,3,4);

    // assert
    if (taskNum == 6) {
        printf("Passed: testGetTaskNum\n");
        return 0;
    }
    
    printf("Failed: testGetTaskNum\n");
    return 1;
}

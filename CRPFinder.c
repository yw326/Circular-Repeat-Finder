//
//  CRPFinder.c
//  mdf-2.0
//
//  Created by Yiming Wang on 2020/7/5.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include "DirectCircleRepeatFinder.h"
#include "InvertedCircleRepeatFinder.h"
#include "Utils.h"
#include "PartitionTask.h"


int main(int argc, char *argv[]) {
    
    // arg[1] = direct or inverted
    // --single     name (optional)
    // --partition  partition_file_directory_path task_numbers (optional)
    // --l1         first_level_minimum_maximal_repeat_length (optional, default 40)
    // --l2         second_level_minimum_maximal_repeat_length (optional, defualt 20)
    // --alpha      mismatch_percentage (optional, default 0.1)
    // --L          extension length (optional, default 800)

    const char* useDirectOptionString = "direct";
    const char* useInvertedOptionString = "inverted";

    if (argc < 3) {
        printf("Error: not enough arguments; at least 2 arguments (%s/%s and one of the --partition and --single options), but only found %d\n", 
            useDirectOptionString, useInvertedOptionString, argc-1);
        return 1;
    }

    // check whether to search direct or inverted
    char* directOrInvertedArg = argv[1];
    if (strcmp(directOrInvertedArg, useDirectOptionString) == 0 || strcmp(directOrInvertedArg, useInvertedOptionString) == 0) {
        printf("Finding %s circle repeats\n", directOrInvertedArg);
    } else {
        printf("Error: invalid argument. Please put \"%s\" or \"%s\" on your first argument\n", useDirectOptionString, useInvertedOptionString);
        return 1;
    }

    // check other arguments
    int l1Idx = -1;
    int l2Idx = -1;
    int LIdx = -1;
    int alphaIdx = -1;
    int taskDirIdx = -1;
    int seqFileIdx = -1;

    int l1 = 40;
    int l2 = 20;
    float alpha = 0.1;
    int L = 800;
    char* taskDir;
    char* taskArg;
    char* seqFileName;

    for(int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--l1") == 0) {
            if (i+1 >= argc) {
                printf("Error: no argument found for first-level minimum maximal repeat length (l1)\n");
                return 1;
            }

            if (atoi(argv[i+1]) <= 0) {
                printf("Error: invalid argument for first-level minimum maximal repeat length (l1)\n");
                return 1;
            } else {
                l1Idx = i+1;
                l1 = atoi(argv[l1Idx]);
            }
        }

        if (strcmp(argv[i], "--l2") == 0) {
            if (i+1 >= argc) {
                printf("Error: no argument found for second-level minimum maximal repeat length (l2)\n");
                return 1;
            }

            if (atoi(argv[i+1]) <= 0) {
                printf("Error: invalid argument for second-level minimum maximal repeat length (l2)\n");
                return 1;
            } else {
                l2Idx = i+1;
                l2 = atoi(argv[l2Idx]);
            }
        }

        if (strcmp(argv[i], "--L") == 0) {
            if (i+1 >= argc) {
                printf("Error: no argument found for extension length (L)\n");
                return 1;
            }

            if (atoi(argv[i+1]) <= 0) {
                printf("Error: invalid argument for extension length (L)\n");
                return 1;
            } else {
                LIdx = i+1;
                L = atoi(argv[LIdx]);
            }
        }

        if (strcmp(argv[i], "--alpha") == 0) {
            if (i+1 >= argc) {
                printf("Error: no argument found for allowed percentage of mismatch (alpha)\n");
                return 1;
            }

            if (atof(argv[i+1]) > 1.0 || atof(argv[i+1]) < 0) {
                printf("Error: invalid argument for allowed percentage of mismatch (alpha)\n");
                return 1;
            } else {
                alphaIdx = i+1;
                alpha = atof(argv[alphaIdx]);
            }
        }

        if (strcmp(argv[i], "--single") == 0) {
            if (i+1 >= argc) {
                printf("Error: not enough arguments found for sequence file name (--single)\n");
                return 1;
            }
            seqFileName = argv[i+1];
            seqFileIdx = i+1;
        }

        if (strcmp(argv[i], "--partition") == 0) {
            if (i+2 >= argc) {
                printf("Error: not enough arguments found for -t\n");
                return 1;
            }
            taskDir = argv[i+1];
            taskArg = argv[i+2];
            taskDirIdx = i+1;
            
            struct stat st = {0};
            if (stat(taskDir, &st) == -1) {
                printf("Error: task directory not found.\n");
                return 1;
            }
            if (taskArg[0] == '-') {
                printf("Error: not enough arguments found for --partition\n");
                return 1;
            }

        }
    }

    if (l1Idx == -1) {
        printf("No first-level minimum maximal repeat length (l1) found. Use default value %d\n", l1);
    }
    
    if (l2Idx == -1) {
        printf("No second-level minimum maximal repeat length (l2) found. Use default value %d\n", l2);
    }

    if (alphaIdx == -1) {
        printf("No allowed mismatch percentage (alpha) found. Use default value %f\n", alpha);
    }

    if (l2 > l1) {
        l2 = l1;
        printf("second maximal repeat length (l2) is larger than first level maximal repeat length (l1), so reset l2 to l1\n");
    }

    if (taskDirIdx * seqFileIdx > 0) {
        printf("Error: one and only one of the --single and --partition option should be selected\n");
        return 1;
    }

    int searchInSingleSequence = seqFileIdx > 0;
    int useDirectSearch = strcmp(directOrInvertedArg, useDirectOptionString) == 0 ? 1 : 0;
    
    // perform search on a single sequence
    if (searchInSingleSequence) {
        unsigned long sequenceLength = getDNASequenceLengthFromFile(seqFileName);
        const char* seq = malloc(sizeof(char)*(sequenceLength+1));
        getDNASequenceFromFile(seqFileName, seq);

        if (useDirectSearch) {
            findDirectCircleRepeatedPairs(seq, NULL, sequenceLength, 0, l1, l2, L, alpha, 
                "result/DirectCircleRepeatIndices.txt", 0, NULL);
        } else {
            findInvertedCircleRepeatedPairs(seq, NULL, sequenceLength, 0, l1, l2, L, alpha, 
                "result/InvertedCircleRepeatIndices.txt", 0, NULL);
        }

        free(seq);


    } else {
        unsigned long partitionSize = getPartitionSize(taskDir);
        int numPartitions = countNumPartitionFilesInDir(taskDir);
        TaskSet *tasks = parseTaskArg(taskArg, numPartitions);

        char taskOutputDir[100];
        snprintf(taskOutputDir, 100, "%s/%s-%s", taskDir, taskArg, directOrInvertedArg);

        struct stat st = {0};
        if (stat(taskOutputDir, &st) == -1) {
            mkdir(taskOutputDir, 0700);
        } else {
            freeTaskSet(tasks);
            printf("Task seems to be already performed, because the folder %s already exists. If you want to reperform this task, delete that folder and try again.\n", taskOutputDir);
            return 1;
        }

        for (int i = 0; i < tasks->numTasks; i++) {
            tasks->tasks[i].partitionSize = partitionSize;
            int taskNum1 = tasks->tasks[i].partitionNum1;
            int taskNum2 = tasks->tasks[i].partitionNum2;
            int taskNum = getTaskNum(taskNum1, taskNum2, numPartitions);

            printf("-------------------\n");
            printf("Performing task %d\n", taskNum);

            char pathBuf1[100];
            char pathBuf2[100];
            snprintf(pathBuf1, 100, "%s/partition-%d.txt",taskDir, taskNum1);
            snprintf(pathBuf2, 100, "%s/partition-%d.txt",taskDir, taskNum2);

            if( access(pathBuf1, F_OK ) == -1 ) {
                printf("Error: %s is not found.\n", pathBuf1);
                return 1;
            }

            if( access(pathBuf2, F_OK ) == -1) {
                printf("Error: %s is not found.\n", pathBuf2);
                return 1;
            }

            char taskOutputFilePath[200];
            snprintf(taskOutputFilePath, 200, "%s/task-%d-result(%d,%d).txt", taskOutputDir, taskNum, taskNum1, taskNum2);
            
            // set up seq
            char* seq1 = malloc(sizeof(char)*(partitionSize+1));
            char* seq2 = malloc(sizeof(char)*(partitionSize+1));
            getDNASequenceFromFile(pathBuf1, seq1);
            
            int isOnSameSequence = taskNum1 == taskNum2;
            
            if (!isOnSameSequence) {
                getDNASequenceFromFile(pathBuf2, seq2);
            } 

            unsigned long n = strlen(seq1);
            unsigned long n2 = isOnSameSequence ? 0 : strlen(seq2);

            // perform search
            if (useDirectSearch) {
                findDirectCircleRepeatedPairs(seq1, seq2, n, n2, l1, l2, L, alpha, taskOutputFilePath, 0, &tasks->tasks[i]);
            } else {
                findInvertedCircleRepeatedPairs(seq1, seq2, n, n2, l1, l2, L, alpha, taskOutputFilePath, 0, &tasks->tasks[i]);
            }
            
            free(seq1);
            free(seq2);
        }
        freeTaskSet(tasks);
    }



    return 0;
}

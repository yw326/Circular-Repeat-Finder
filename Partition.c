//
//  partition.c
//  GeneSeq
//
//  Created by Yiming Wang on 2019/5/30.
//  Copyright © 2019 Yiming Wang. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "Utils.h"


int main(int argc, char *argv[]) {
    
    // arg[1] = seqFile name
    // arg[2] = number of partition
    // arg[3] = name of the directory where the splitted sequence files are stored
    
    if (argc < 4) {
        printf("not enough arguments provided: 3 required, %d provided\n", argc - 1);
        return 1;
    }
    
    char* seqFileName = argv[1];
    if( access(seqFileName, F_OK ) == -1 ) {
        printf("Error: sequence file not found. Please put a valid file name on the first argument.\n");
        return 1;
    }
    
    int num_partitions = atoi(argv[2]);
    if (num_partitions <= 0) {
        printf("Error: invalid parition number (must be >= 1).\n");
        return 1;
    }

    char* dir_name = argv[3];
    struct stat st = {0};
    if (stat(dir_name, &st) == -1) {
        mkdir(dir_name, 0700);
    } else {
        printf("Error: such directory already exists.\n");
        return -1;
    }
    
    unsigned long sequenceLength = getDNASequenceLengthFromFile(seqFileName);
    printf("sequence length: %ld\n", sequenceLength);

    const char* seq = malloc(sizeof(char)*(sequenceLength+1));
    getDNASequenceFromFile(seqFileName, seq);

    if (num_partitions >= sequenceLength) {
        printf("Error: number of partitions greater than sequence length\n");
        return 1;
    }

    char** sequences = partitionSequence(seq, num_partitions);
    free(seq);

    for (int i = 0; i < num_partitions; i++) {
        char *file_path = getSplittedSeqFilePath(dir_name, i+1);
        FILE* seq_file = fopen(file_path, "w");
        fprintf(seq_file, "%s\n", sequences[i]);
        printf("partition %d finished\n", i+1);
        fclose(seq_file);
    }

    freePartitions(sequences, num_partitions);
    
    
    return 0;
}

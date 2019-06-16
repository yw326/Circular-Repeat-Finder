//
//  partition.c
//  GeneSeq
//
//  Created by Yiming Wang on 2019/5/30.
//  Copyright © 2019 Yiming Wang. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "str_helper.h"
#include "file_helper.h"
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

char *get_splitted_seqfile_path(char* dir_name, int seq_num) {
    char* path = malloc(sizeof(char)*1000);
    char* splitted_seqfile_name_prefix = "partition-";
    char* file_suffix = ".txt";
    
    char seq_num_str[100];
    sprintf(seq_num_str, "%d", seq_num);
    
    unsigned long splitted_seqfile_name_prefix_len = strlen(splitted_seqfile_name_prefix);
    unsigned long dir_name_len = strlen(dir_name);
    unsigned long seq_num_str_len = strlen(seq_num_str);
    unsigned long file_suffix_len = strlen(file_suffix);
    
    // dir_name/
    for (int i = 0; i < dir_name_len; i++) {
        path[i] = dir_name[i];
    }
    path[dir_name_len] = '/';
    
    //partition-seq_num.txt
    for (int i = 0; i < splitted_seqfile_name_prefix_len; i++) {
        path[i+dir_name_len+1] = splitted_seqfile_name_prefix[i];
    }

    for (int i = 0; i < seq_num_str_len; i++) {
        path[i+dir_name_len+splitted_seqfile_name_prefix_len+1] = seq_num_str[i];
    }
    
    for (int i = 0; i < file_suffix_len; i++) {
        path[i+dir_name_len+splitted_seqfile_name_prefix_len+seq_num_str_len+1] = file_suffix[i];
    }
    
    return path;
}



int main(int argc, char *argv[]) {
    
    // arg[1] = seq_file name
    // arg[2] = number of partition
    // arg[3] = name of the directory where the splitted sequence files are stored
    
    
    
    if (argc < 4) {
        printf("not enough arguments provided: 3 required, %d provided\n", argc - 1);
        return 1;
    }
    
    char* seq_file_name = argv[1];
    if( access(seq_file_name, F_OK ) == -1 ) {
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
    
    char *seq = getStrFromFile(seq_file_name);
    if (num_partitions >= strlen(seq)) {
        printf("Error: number of partitions greater than sequence length\n");
        return 1;
    }
    
    my_string* my_strings = partition_long_str(seq, num_partitions);
    for (int i = 0; i < num_partitions; i++) {
        char *file_path = get_splitted_seqfile_path(dir_name, i+1);
        FILE* index_file = fopen(file_path, "w");
        fprintf(index_file, "%s\n", my_strings[i].str);
        printf("partition %d finished\n", i+1);
        fclose(index_file);
    }
    
    free_my_strings(my_strings, num_partitions);
    free(seq);
    
    return 0;
}

//
//  Utils.c
//  mdf-2.0
//
//  Created by Yiming Wang on 2020/7/5.
//

#include "Utils.h"

unsigned long getDNASequenceLengthFromFile (char *fileName) {
    FILE *fp;
    fp = fopen(fileName, "r");
    unsigned long n = 0;
    int c;
    while ((c = fgetc(fp)) != EOF) {
        if ((char)c == 'A' || (char)c == 'T' || (char)c == 'C' || (char)c == 'G') {
            n++;
        }
    }
    fclose(fp);
    return n;
}

// allocate n + 1
void getDNASequenceFromFile(char *fileName, char* seq) {
    FILE *fp;
    fp = fopen(fileName, "r");
    
    int c;
    unsigned long n = 0;

    while ((c = fgetc(fp)) != EOF) {
        if ((char)c == 'A' || (char)c == 'T' || (char)c == 'C' || (char)c == 'G') {
            seq[n++] = (char)c;
        }
    }
    
    seq[n] = '\0';

    fclose(fp);
}

// allocate n1+n2+2
char* getConcatenatedSequence(char* seq, char* seq1, char* seq2, unsigned long n1, unsigned long n2) {
    unsigned long i = 0;

    while(i < n1) {
        seq[i] = seq1[i];
        i++;
    }

    seq[i] = '#';
    i++;

    while(i < n1+n2+1) {
        seq[i] = seq2[i-n1-1];
        i++;
    }

    seq[i] = '\0';

    return seq;
}

void getReversedSequence(char* seq, unsigned long n, char* resultSeq) {
    for(int i = 0; i < n; i++) {
        resultSeq[i] = seq[n-i-1];
    }
    resultSeq[n] = '\0';
}


// allocate 2n + 2
void getConcantenatedInvertedSequence(char* givenSeq, unsigned long n, char* resultSeq) {

    int i = 0;
    while (i < n) {
        resultSeq[i] = givenSeq[i];
        i++;
    }

    resultSeq[i] = '#';
    i++;

    int m = n - 1;
    while (m >= 0) {
        char c = givenSeq[m];
        if (c == 'A') {
            resultSeq[i] = 'T';
        } else if (c == 'T') {
            resultSeq[i] = 'A';
        } else if (c == 'C') {
            resultSeq[i] = 'G';
        } else if (c == 'G') {
            resultSeq[i] = 'C';
        }
        i++;
        m--;
    }
    resultSeq[2*n+1] = '\0';
}

void getConcantenatedInvertedSequenceDiff(char* s1, char* s2, unsigned long n1, unsigned long n2, char* resultSeq) {
    int i = 0;
    while (i < n1) {
        resultSeq[i] = s1[i];
        i++;
    }

    resultSeq[i] = '#';
    i++;

    int m = n2 - 1;
    while (m >= 0) {
        char c = s2[m];
        if (c == 'A') {
            resultSeq[i] = 'T';
        } else if (c == 'T') {
            resultSeq[i] = 'A';
        } else if (c == 'C') {
            resultSeq[i] = 'G';
        } else if (c == 'G') {
            resultSeq[i] = 'C';
        }
        i++;
        m--;
    }
    resultSeq[n1+n2+1] = '\0';
}


// allcoate length + 1
void substring(char* s, int start, int length, char* substr) {
    strncpy(substr, s + start, length);
    substr[length] = '\0';
}

char* returnSubstring(char* s, int start, int length) {
    char* substr = malloc(sizeof(char)*(length+2));
    strncpy(substr, s + start, length);
    substr[length] = '\0';
    return substr;
}



// allcoate length + 1
void invertedSubstring(char *str, int start, int length, char* substr) {
    
    for (int i = 0; i < length; i++) {
        char c = str[start+length-i-1];
        if (c == 'A') {
            substr[i] = 'T';
        } else if (c == 'T') {
            substr[i] = 'A';
        } else if (c == 'C') {
            substr[i] = 'G';
        } else if (c == 'G') {
            substr[i] = 'C';
        }
    }
    substr[length] = '\0';
}


int tupleComparator(const void *v1, const void *v2) {
    const int* t1 = (int*) v1;
    const int* t2 = (int*) v2;
    if (t1[0] > t2[0]) {
        return 1;
    } else if (t1[0] < t2[1]) {
        return -1;
    } else {
        return 0;
    }
}

void sortTuples(int **tuples, int n) {
    qsort(tuples, n, sizeof(int)*2, tupleComparator);
}

int charToIndex(char c) {
    if (c == 'A') {
        return 1;
    } else if (c == 'T') {
        return 2;
    } else if (c == 'C') {
        return 3;
    } else if (c == 'G') {
        return 4;
    } else if (c == '#') {
        return 5;
    } else if (c == '?') {
        return 0;
    } else {
        return -1;
    }
}

unsigned long getCorresondingIndexFromConcatenatedInvertedSequenceLeft(unsigned long startIndex, int subStringLength, 
                                                                   unsigned long n) {
    // (total length) - startIndex - stringLength
    return 2*n + 1 - startIndex - subStringLength;
}

unsigned long getCorresondingIndexFromConcatenatedInvertedSequenceRight(unsigned long startIndex, int subStringLength, 
                                                                        unsigned long totalLength) {
    // original: XXXX#ATTCGT
    // inverted: XXXX#ACGAAT
    // given substring GAA in inverted, want to find TTC
    // totalLength = 11, stringlength = 3, startIndex = 7 => get TTC start at 6
    return totalLength - subStringLength - startIndex;
}


char *getSplittedSeqFilePath(char* dir_name, int seq_num) {
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
    
    path[dir_name_len+splitted_seqfile_name_prefix_len+seq_num_str_len+1+file_suffix_len] = '\0';
    
    return path;
}

char** partitionSequence(char* seq, int numberOfPartitions) {
    char** resultSequences = malloc(sizeof(char*)*numberOfPartitions);
    unsigned long n = strlen(seq);
    unsigned long partitionSize = n / numberOfPartitions;
    if (n % numberOfPartitions != 0) {
        partitionSize += 1;
    }
    printf("partition size: %ld\n", partitionSize);

    for (int i = 0; i < numberOfPartitions - 1; i++) {
        char* partition = malloc(sizeof(char)*partitionSize + 1);
        strncpy(partition, seq + i*partitionSize, partitionSize);
        partition[partitionSize] = '\0';
        resultSequences[i] = partition;
    }

    unsigned long lastPartitionLength = n - (numberOfPartitions-1)*partitionSize;
    char* lastPartition = malloc(sizeof(char)*lastPartitionLength + 1 );
    strncpy(lastPartition, seq + (numberOfPartitions-1)*partitionSize, lastPartitionLength);
    lastPartition[lastPartitionLength] = '\0';
    resultSequences[numberOfPartitions-1] = lastPartition;

    return resultSequences;
}

void freePartitions(char** partitions, int numberOfPartitions) {
    for (int i = 0; i < numberOfPartitions; i++) {
        free(partitions[i]); 
    }
    free(partitions);
}

int countNumPartitionFilesInDir(char* dir) {
    int file_count = 0;
    DIR * dirp;
    struct dirent * entry;
    
    dirp = opendir(dir);
    while ((entry = readdir(dirp)) != NULL) {
        // if (entry->d_type == DT_REG && strcmp(".DS_Store", entry->d_name) != 0 ) {
        //     file_count++;
        // }
        char* prefix = returnSubstring(entry->d_name, 0, 9);
        if (strcmp("partition", prefix) == 0) {
            file_count++;
        }
        free(prefix);
        
    }
    closedir(dirp);
    return file_count;
}

unsigned long getPartitionSize(const char* dir) {
    char partition_1_path[100];
    snprintf(partition_1_path, 100, "%s/partition-1.txt",dir);
    return getDNASequenceLengthFromFile(partition_1_path); 
}

unsigned long partitionIdxToSeqIdx(unsigned long partitionIdx, int partitionNum, unsigned long partitionSize) {
    return partitionSize * (partitionNum - 1) + partitionIdx;
}
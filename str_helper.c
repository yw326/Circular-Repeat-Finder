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
    unsigned long curr_ptr = 0;
    
    for (int i = 0; i < num_partitions; i++){
        char* str = malloc(sizeof(partition_size)*sizeof(char));
        
        if (i == num_partitions - 1) {
            unsigned long j;
            for (j = 0; curr_ptr < strlen(seq); j++) {
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


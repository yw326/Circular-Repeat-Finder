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

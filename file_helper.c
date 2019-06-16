//
//  file_helper.c
//  GeneSeq
//
//  Created by Yiming Wang on 2019/3/11.
//  Copyright © 2019 Yiming Wang. All rights reserved.
//

#include "file_helper.h"

// MARK: string from file helper
#define MAXCHAR 400000000
char *getStrFromFile(char *file_name) {
    FILE *fp;
    fp = fopen(file_name, "r");
    
    char *str = malloc(sizeof(char)*MAXCHAR);
    int c;
    int n = 0;
    while ((c = fgetc(fp)) != EOF) {
        if ((char)c == 'A' || (char)c == 'T' || (char)c == 'C' || (char)c == 'G') {
            str[n++] = (char)c;
        }
    }
    
    str[n] = '\0';
    fclose(fp);
    return str;
}

char *getComplementSeqFromFile(char *file_name) {
    FILE *fp;
    fp = fopen(file_name, "r");
    
    char *str = malloc(sizeof(char)*MAXCHAR);
    int c;
    int n = 0;
    while ((c = fgetc(fp)) != EOF) {
        if ((char)c == 'A') {
            str[n++] = 'T';
        } else if ((char)c == 'T') {
            str[n++] = 'A';
        } else if ((char)c == 'C') {
            str[n++] = 'G';
        } else if ((char)c == 'G') {
            str[n++] = 'C';
        }
    }
    
    str[n] = '\0';
    fclose(fp);

    return str;
}


char *getConcantenatedReversedStrFromFile(char *file_name) {
    FILE *fp;
    fp = fopen(file_name, "r");
    
    char *str = malloc(sizeof(char)*MAXCHAR*2);
    int c;
    int n = 0;
    while ((c = fgetc(fp)) != EOF) {
        if ((char)c == 'A' || (char)c == 'T' || (char)c == 'C' || (char)c == 'G') {
            str[n++] = (char)c;
        }
    }
    
    int m = n;
    str[n] = '#';
    //n++;
    
    while (m != -1) {
        str[n++] = str[m--];
    }
    
    str[n] = '\0';
    fclose(fp);

    return str;
}

char *getConcantenatedReversedComplementStrFromFile(char *file_name) {
    FILE *fp;
    fp = fopen(file_name, "r");
    
    char *str = malloc(sizeof(char)*MAXCHAR*2);
    int c;
    int n = 0;
    while ((c = fgetc(fp)) != EOF) {
        if ((char)c == 'A' || (char)c == 'T' || (char)c == 'C' || (char)c == 'G') {
            str[n++] = (char)c;
        }
    }
    
    int m = n;
    str[n] = '#';
    n++;

    while (m != -1) {
        char c = str[m--];
        if (c == 'A') {
            str[n++] = 'T';
        } else if (c == 'T') {
            str[n++] = 'A';

        } else if (c == 'C') {
            str[n++] = 'G';
        } else if (c == 'G') {
            str[n++] = 'C';
        }
    }
    str[n] = '\0';
    fclose(fp);

    return str;
}

// input a seq file, output a file with "N" deleted
void prefilter_file(char *file_name, char *output_file_name) {
    char* s = getStrFromFile(file_name);
    FILE *f = fopen(output_file_name, "w");
    if (f == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    fprintf(f, ">prefiltered \n");
    fprintf(f, "%s", s);
    free(s);
    fclose(f);
}

// output first half and second half of the sequence into two files respectively
void output_two_halves_of_file(char *file_name, char *output_name1, char *output_name2) {
    char* s = getStrFromFile(file_name);
    long len = strlen(s);
    
    FILE *f1 = fopen(output_name1, "w");
    char s1[len/2 + 1];
    for (int i = 0; i < len/2; i++) {
        s1[i] = s[i];
    }
    s1[len/2 + 1] = '\0';
    fprintf(f1, ">%s \n", output_name1);
    fprintf(f1, "%s", s1);
    fclose(f1);
    
    FILE *f2 = fopen(output_name2, "w");
    char s2[len/2 + 1];
    for (int i = 0; i < len/2; i++) {
        s2[i] = s[i + len/2];
    }
    s2[len/2 + 1] = '\0';
    fprintf(f2, ">%s \n", output_name2);
    fprintf(f2, "%s", s2);
    fclose(f2);
    
    free(s);
}


int count_num_files_in_dir(char* dir) {
    int file_count = 0;
    DIR * dirp;
    struct dirent * entry;
    
    dirp = opendir(dir);
    while ((entry = readdir(dirp)) != NULL) {
        if (entry->d_type == DT_REG && strcmp(".DS_Store", entry->d_name) != 0 ) {
            file_count++;
        }
    }
    closedir(dirp);
    return file_count;
}



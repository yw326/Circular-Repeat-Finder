

#include "maximal_repeat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


int maximal_circle_repeat_count = 0;
int reverse_maximal_repeat_count = 0;
short linked_list_num = 6;
int min_extension_check = 20;
int using_slurm = 0;

// MARK: edit distance
/* Returns an unsigned integer, depicting
 * the difference between `a` and `b`.
 * See http://en.wikipedia.org/wiki/Levenshtein_distance
 * for more information. */

#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

int levenshtein(char *s1, char *s2, int s1len, int s2len) {
    unsigned int x, y, lastdiag, olddiag;
    unsigned int column[s1len+1];
    for (y = 1; y <= s1len; y++)
        column[y] = y;
    for (x = 1; x <= s2len; x++) {
        column[0] = x;
        for (y = 1, lastdiag = x-1; y <= s1len; y++) {
            olddiag = column[y];
            column[y] = MIN3(column[y] + 1, column[y-1] + 1, lastdiag + (s1[y-1] == s2[x-1] ? 0 : 1));
            lastdiag = olddiag;
//            if (column[y] < s1len*(1-similarity)) {
//                return 10000;
//            }
        }
    }
    return(column[s1len]);
}

// MARK: KMP algorithm
void computeLPSarray(char *pat, int M, int *lps)
{
    int len = 0;    //lenght of previous longest prefix suffix
    int i ;
    
    lps[0] = 0 ;    //lps[0] is always 0
    i = 1;
    
    while(i <  M)    //loop calculates lps[i] for i = 1 to M-1
    {
        if(pat[i] == pat[len])
        {
            len++;
            lps[i] = len;
            i++;
        }
        else    //pat[i] != pat[len]
        {
            if(len != 0)
            {
                //this is tricky
                //consider the example AAACAAAA and i =7
                len = lps[len - 1];
                //we donot increment i here
            }
            else    //if len == 0
            {
                lps[i] = 0;
                i++;
            }
        }
    }
}

arr_with_size* KMP(char *txt, char *pat, int N, int M) {
    int arr[N];
    int size = 0;
    
    int *lps = (int*)malloc(M*sizeof(int));
    int j = 0;
    
    computeLPSarray(pat, M, lps);
    
    int i = 0;
    while(i < N)
    {
        if(pat[j] == txt[i])
        {
            j++;
            i++;
        }
        if (j == M) {
            arr[size] = i-j;
            size++;
            //printf("Found pattern at index %d\n",i - j);
            j = lps[j-1];
        }
        else if(pat[j] != txt[i])
        {
            if(j != 0)
                j = lps[j-1];
            else
                i++;
        }
    }
    free(lps);
    
    int *result = malloc(sizeof(int)*size);
    for (int i = 0; i < size; i++) {
        result[i] = arr[i];
        //printf("arr %d\n", result[i]);
    }
    arr_with_size *arr_ws = malloc(sizeof(arr_with_size));
    arr_ws->arr = result;
    arr_ws->size = size;
    
    return arr_ws;
    
}


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

int checkTwoStringEqual(char *str1, char *str2, int len) {
    for (int i = 0; i < len; i++) {
        if (str1[i] != str2[i]) {
            return 0;
        }
    }
    return 1;
}

//MARK: circle duplictions checking from repeated pairs
//void checkTwoPairsCircleDuplication(triple t1, triple t2) {
//    int s1 = t1.length;
//    int s2 = t2.length;
//    int a1 = t1.p1 < t1.p2 ? t1.p1 : t1.p2;
//    int a2 = t1.p1 > t1.p2 ? t1.p1 : t1.p2;
//    int b1 = t2.p1 < t2.p2 ? t2.p1 : t2.p2;
//    int b2 = t2.p1 > t2.p2 ? t2.p1 : t2.p2;
//
//    //    printf("a1,a2,len (%d,%d,%d)\n", a1, a2, s1);
//    //    printf("b1,b2,len (%d,%d,%d)\n", b1, b2, s2);
//
//    if (linked_list_num == 5) {
//        if ((a1 < b1 && a2 > b2) || (b1 < a1 && b2 > a2)) {
//            if (a1 + s1 == b1 && b2 + s2 == a2) {
//                printf("------------------\n");
//                printf("first: (%d,%d,%d,%d)\n", a1, b2, s1 + s2, s1);
//                printf("(%d,%d,%d)\n",a1,a2,s1);
//                printf("(%d,%d,%d)\n",b1,b2,s2);
//                maximal_circle_repeat_count++;
//            }
//            if (a2 + s1 == b2 && b1 + s2 == a1) {
//                printf("------------------\n");
//                printf("second: (%d,%d,%d,%d)\n", a2, b1, s1 + s2, s2);
//                printf("(%d,%d,%d)\n",a1,a2,s1);
//                printf("(%d,%d,%d)\n",b1,b2,s2);
//                maximal_circle_repeat_count++;
//            }
//        }
//    }
//
//    if (linked_list_num == 6) {
//        if ((a1 < b1 && a2 < b2) || (b1 < a1 && b2 < a2)) {
//            if (a1 + s1 == b1 && a2 + s2 == b2) {
//                printf("------------------\n");
//                printf("first: (%d,%d,%d,%d)\n", a1, a2, s1 + s2, s1);
//                printf("(%d,%d,%d)\n",a1,a2,s1);
//                printf("(%d,%d,%d)\n",b1,b2,s2);
//                maximal_circle_repeat_count++;
//            }
//            if (b1 + s1 == a1 && b2 + s2 == a2) {
//                printf("------------------\n");
//                printf("second: (%d,%d,%d,%d)\n", b1, b2, s1 + s2, s2);
//                printf("(%d,%d,%d)\n",a1,a2,s1);
//                printf("(%d,%d,%d)\n",b1,b2,s2);
//                maximal_circle_repeat_count++;
//            }
//        }
//    }
//
//}

void findCircleDuplication(tuple *result_list, int size, char *str, double pre_check_ratio, double mismatch_ratio, int min_extension_len) {
    
    
    int arr[size];
    unsigned long maximal_repeat_pair_count = 0;
    
    for (int i = 0; i < size; i++) {
        arr[i] = result_list[i].size;
        maximal_repeat_pair_count += arr[i];
    }
    
    printf("-------------------\n");
    printf("The total number of maximal repeat count is %lu\n", maximal_repeat_pair_count);
    
    int max_check_length = 1000;
    int min_check_length = min_extension_len;
    
    unsigned int count1 = 0;
    unsigned int count2 = 0;
    unsigned int strlength = strlen(str);
    
    printf("-------------------\n");
    
    //...s1s2...s2s1... with s2 being maximal
    printf("Finding first type non-maximal cirlce repeat...\n");
    
    FILE* first_repeat_index_file;
    FILE* first_repeat_seq_file;
    
    if (using_slurm == 0){
        first_repeat_index_file = fopen("result/1st-type_circle_repeat", "w");
        first_repeat_seq_file = fopen("result/1st-type cr seq", "w");
    }
    int c = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < arr[i]; j++) {
            
            triple t = result_list[i].result[j];
            int p1 = t.p1 < t.p2 ? t.p1 : t.p2;
            int p2 = t.p1 > t.p2 ? t.p1 : t.p2;
            
            
            //substr after second s2
            if (p2+t.length+min_check_length > strlength || p1+t.length>=p2 || p1-min_check_length < 0) { continue; }
            
            char *pattern = returnSubstring(str, p2+t.length, min_check_length);
            
            //substr before first s2
            int text_length = p1 - max_check_length < 0 ? p1 : max_check_length;
            char *text = returnSubstring(str, p1-text_length, text_length);
            
            arr_with_size *result = KMP(text, pattern, text_length, min_check_length);
            
            for (int m = 0; m < result->size; m++) {
                int start = result->arr[m] + p1 - text_length;
//                int equal = 1;
                c++;
                
                int extension_len = p1-start;
                int pre_check_len = (int) pre_check_ratio * extension_len;

                char* pre_ext1 = returnSubstring(str, start, pre_check_len);
                char* pre_ext2 = returnSubstring(str, p2+t.length, pre_check_len);
                int pre_dist = levenshtein(pre_ext1, pre_ext2, pre_check_len, pre_check_len);
                free(pre_ext1);
                free(pre_ext2);
                
                
                
                
                if (pre_dist < pre_check_len*mismatch_ratio) {
                    continue;
                }

                char* extension1 = returnSubstring(str, start, extension_len);
                char* extension2 = returnSubstring(str, p2+t.length, extension_len);
                int distance = levenshtein(extension1, extension2, p1-start, p1-start);
                free(extension1);
                free(extension2);

//                for (int n = 0; n < p1 - start; n++) {
//                    if (str[start+n] != str[p2+t.length+n] ) {
//                        equal = 0;
//                        break;
//                    }
//                }
                if (distance <= mismatch_ratio*extension_len) {
                //if (equal == 1) {
                    count1++;
                    if (using_slurm == 0) {
                        fprintf(first_repeat_index_file, "(%d,%d,%d,%d)\n", start, p2, p1-start+t.length, t.length);
                        fprintf(first_repeat_seq_file, ">seq%d\n", count1);
                        fprintf(first_repeat_seq_file, "%s\n", returnSubstring(str, start, t.length+p1-start));
                        fprintf(first_repeat_seq_file, ">seq%d-maximal\n", count1);
                        fprintf(first_repeat_seq_file, "%s\n", returnSubstring(str, p1, t.length));
                        fprintf(first_repeat_seq_file, ">seq%d-non-maximal\n", count1);
                        fprintf(first_repeat_seq_file, "%s\n", returnSubstring(str, start, p1 - start));
                    } else {
                        printf("(%d,%d,%d,%d)\n", start, p2, p1-start+t.length, t.length);
//                        printf(">seq%d\n", count1);
//                        printf("%s\n", returnSubstring(str, start, t.length+p1-start));
//                        printf(">seq%d-maximal\n", count1);
//                        printf("%s\n", returnSubstring(str, p1, t.length));
//                        printf(">seq%d-non-maximal\n", count1);
//                        printf("%s\n", returnSubstring(str, start, p1 - start));
                    }
                    
                    
                    //printf("(%d,%d,%d,%d)\n", start, p2, p1-start+t.length, t.length);
                }
            }
            free(text);
            free(pattern);
            free(result->arr);
            free(result);
            

            /*
             for (int len = min_check_length; len < max_check_length; len++) {
             
             if (p2 + len > strlength || p1 - len < 0) { break; }
             
             for (int k = 0; k < len; k++) {
             if (str[p1-k-1] != str[p2+t.length+len-k-1]) { break; }
             
             if (k == len - 1 && p1+t.length<p2) {
             count1++;
             
             int start = p1-len;
             fprintf(first_repeat_index_file, "(%d,%d,%d,%d)\n", start, p2, p1-start+t.length, t.length);
             
             fprintf(first_repeat_seq_file, ">seq%d\n", count1);
             fprintf(first_repeat_seq_file, "%s\n", returnSubstring(str, start, t.length+p1-start));
             fprintf(first_repeat_seq_file, ">seq%d-maximal\n", count1);
             fprintf(first_repeat_seq_file, "%s\n", returnSubstring(str, p1, t.length));
             fprintf(first_repeat_seq_file, ">seq%d-non-maximal\n", count1);
             fprintf(first_repeat_seq_file, "%s\n", returnSubstring(str, start, p1 - start));
             
             printf("(%d,%d,%d,%d)\n", p1-len, p2, len+t.length, len);
             }
             }
             }
             */
        }
    }

//    if (using_slurm == 0) {
//        fclose(first_repeat_index_file);
//        fclose(first_repeat_seq_file);
//    }

    
    
    printf("-------------------\n");
    printf("Finding second type non-maximal cirlce repeat\n");
    FILE* second_repeat_index_file;
    FILE* second_repeat_seq_file;
    
    if (using_slurm == 0) {
        second_repeat_index_file= fopen("result/2nd-type circle repeat", "w");
        second_repeat_seq_file = fopen("result/2nd-type cr seq", "w");
    }
    
    //...s1s2...s2s1... with s1 being maximal
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < arr[i]; j++) {
            
            triple t = result_list[i].result[j];
            int p1 = t.p1 < t.p2 ? t.p1 : t.p2;
            int p2 = t.p1 > t.p2 ? t.p1 : t.p2;
            
            //substr after first s1
            if (p1+t.length+min_check_length > p2 - min_check_length) { continue; }
            char *pattern = returnSubstring(str, p1+t.length, min_check_length);
            
            //substr before second s1
            int text_length = p2 - max_check_length < p1 + t.length + max_check_length ? (p2 - p1 - t.length)/2: max_check_length;
            char *text = returnSubstring(str, p2-text_length, text_length);
            
            arr_with_size *result = KMP(text, pattern, text_length, min_check_length);
            for (int m = 0; m < result->size; m++) {
                c++;

                int start = result->arr[m]+p2-text_length;
                //int equal = 1;
                int extension_len = p2-start;
        
                int pre_check_len = (int) pre_check_ratio * extension_len;
                char* pre_ext1 = returnSubstring(str, start, pre_check_len);
                char* pre_ext2 = returnSubstring(str, p1+t.length, pre_check_len);
                int pre_dist = levenshtein(pre_ext1, pre_ext2, pre_check_len, pre_check_len);
                free(pre_ext1);
                free(pre_ext2);
                
                if (pre_dist < pre_check_len*mismatch_ratio) {
                    continue;
                }
                
                char* extension1 = returnSubstring(str, start, extension_len);
                char* extension2 = returnSubstring(str, p1+t.length, extension_len);
                int distance = levenshtein(extension1, extension2, extension_len, extension_len);
                free(extension1);
                free(extension2);
                
                
//                for (int n = 0; n < p2 - start; n++) {
//                    if (str[start+n] != str[p1+t.length+n] ) {
//                        equal = 0;
//                        break;
//                    }
//                }

                if (distance <= mismatch_ratio*extension_len) {
                //if (equal == 1) {
                    count2++;
                    if (using_slurm == 0) {
                        fprintf(second_repeat_index_file, "(%d,%d,%d,%d)\n", p1,  start, p2-start+t.length, t.length);
                        
                        fprintf(second_repeat_seq_file, ">seq%d\n", count2);
                        fprintf(second_repeat_seq_file, "%s\n", returnSubstring(str, p1, t.length+p2-start));
                        fprintf(second_repeat_seq_file, ">seq%d-maximal\n", count2);
                        fprintf(second_repeat_seq_file, "%s\n", returnSubstring(str, p1, t.length));
                        fprintf(second_repeat_seq_file, ">seq%d-non-maximal\n", count2);
                        fprintf(second_repeat_seq_file, "%s\n", returnSubstring(str, p1+t.length, p2 - start));
                    } else {
                        printf("(%d,%d,%d,%d)\n", p1,  start, p2-start+t.length, t.length);
//                        printf(">seq%d\n", count2);
//                        printf("%s\n", returnSubstring(str, p1, t.length+p2-start));
//                        printf(">seq%d-maximal\n", count2);
//                        printf("%s\n", returnSubstring(str, p1, t.length));
//                        printf(">seq%d-non-maximal\n", count2);
//                        printf("%s\n", returnSubstring(str, p1+t.length, p2 - start));
                    }
                    
                    
                    //printf("(%d,%d,%d,%d)\n", p1,  start, p2-start+t.length, t.length);
                }
            }
            free(text);
            free(pattern);
            free(result->arr);
            free(result);
            
            
            /*
             for (int len = min_check_length; len < max_check_length; len++) {
             
             for (int k = 0; k < max_check_length; k++) {
             
             if (str[p1+t.length+k] != str[p2-len+k]) {
             break;
             }
             
             if (k == len - 1 && p1+t.length+len < p2-len) {
             //printf("%s",returnSubstring(str, p1, t.length));
             
             int start = p2-len;
             fprintf(second_repeat_index_file, "(%d,%d,%d,%d)\n", p1,  start, p2-start+t.length, t.length);
             
             fprintf(second_repeat_seq_file, ">seq%d\n", count2);
             fprintf(second_repeat_seq_file, "%s\n", returnSubstring(str, p1, t.length+p2-start));
             fprintf(second_repeat_seq_file, ">seq%d-maximal\n", count2);
             fprintf(second_repeat_seq_file, "%s\n", returnSubstring(str, p1, t.length));
             fprintf(second_repeat_seq_file, ">seq%d-non-maximal\n", count2);
             fprintf(second_repeat_seq_file, "%s\n", returnSubstring(str, p1+t.length, p2 - start));
             
             //printf("(%d,%d,%d,%d)\n", p1, p2 - len, len+t.length, len);
             count2++;
             }
             }
             
             }*/
        }
    }
    
    
//    if (using_slurm == 0) {
//        fclose(second_repeat_index_file);
//        fclose(second_repeat_seq_file);
//    }
    
    
    printf("-------------------\n");
    printf("The total number of maximal repeat count is %lu\n", maximal_repeat_pair_count);
    printf("The total number of KMP match is %d\n", c);

    //printf("Total number of maximal circle repeat count is %i\n", maximal_circle_repeat_count);
    printf("the number of first type non-maximal cirlce repeat is %i \n", count1);
    printf("the number of second type non-maximal cirlce repeat is %i\n", count2);
    
    
    //    printf("error count is %d\n", error_count);
}

//void checkTwoPairsForReverseCircleDuplication(triple t1, triple t2, int strlen) {
//    int s1 = t1.length;
//    int s2 = t2.length;
//    int a1 = t1.p1 < t1.p2 ? t1.p1 : t1.p2;
//    int a2 = t1.p1 > t1.p2 ? t1.p1 : t1.p2;
//    int b1 = t2.p1 < t2.p2 ? t2.p1 : t2.p2;
//    int b2 = t2.p1 > t2.p2 ? t2.p1 : t2.p2;
//
//    a2 = strlen - a2 - s1;
//    b2 = strlen - b2 - s2;
//
//    a1 = a1 < a2 ? a1 : a2;
//    a2 = a1 > a2 ? a1 : a2;
//    b1 = b1 < b2 ? b1 : b2;
//    b2 = b1 > b2 ? b1 : b2;
//
////    printf("a1,a2,len (%d,%d,%d)\n", a1, a2, s1);
////    printf("b1,b2,len (%d,%d,%d)\n", b1, b2, s2);
//
//
//    if ((a1 < b1 && a2 > b2) || (b1 < a1 && b2 > a2)) {
//        if (a1 + s1 == b1 && b2 + s2 == a2) {
//            printf("------------------\n");
//            printf("first: (%d,%d,%d,%d)\n", a1, b2, s1 + s2, s1);
//            printf("(%d,%d,%d)\n",a1,a2,s1);
//            printf("(%d,%d,%d)\n",b1,b2,s2);
//            reverse_maximal_repeat_count++;
//        }
//        if (a2 + s1 == b2 && b1 + s2 == a1) {
//            printf("------------------\n");
//            printf("second: (%d,%d,%d,%d)\n", a2, b1, s1 + s2, s2);
//            printf("(%d,%d,%d)\n",a1,a2,s1);
//            printf("(%d,%d,%d)\n",b1,b2,s2);
//            reverse_maximal_repeat_count++;
//        }
//    }
//
//}

void findReverseCircleDuplication(tuple *result_list, int size, char *str, double pre_check_ratio, double mismatch_ratio, int min_extension_len) {

    int arr[size];
    unsigned long maximal_repeat_pair_count = 0;
    int str_len = strlen(str);
    
    for (int i = 0; i < size; i++) {
        arr[i] = result_list[i].size;
        maximal_repeat_pair_count += arr[i];
    }
    
    
    /*
     printf("-------------------\n");
     printf("Finding maximal circle repeats...\n");
     //int error_count = 0;
     
     for (int i = 0; i < size; i++) {
     for (int j = 0; j < arr[i]; j++) {
     
     triple t1 = result_list[i].result[j];
     for (int m = j + 1; m < arr[i]; m++) {
     checkTwoPairsCircleDuplication(t1, result_list[i].result[m]);
     }
     for (int n = i + 1; n < size; n++) {
     for (int x = 0; x < arr[n]; x++) {
     triple t2 = result_list[n].result[x];
     checkTwoPairsCircleDuplication(t1, t2);
     }
     
     }
     }
     }
     
     printf("Total number of reverse maximal circle repeat count is %i\n", reverse_maximal_repeat_count);
     */
    
    int max_check_length = 1000;
    int min_check_length = min_extension_len;
    
    unsigned int count1 = 0;
    unsigned int count2 = 0;
    
    printf("Finding first type non-maximal reverse cirlce repeat...\n");
    FILE* first_repeat_index_file = fopen("result/1st-type reversed-circle repeat", "w");
    FILE* first_repeat_seq_file = fopen("result/1st-type rev-cr seq", "w");
    //..s1s2...s1^(-1)s2^(-1)... s1 being maximal
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < arr[i]; j++) {
            
            triple t = result_list[i].result[j];
            int p1 = t.p1 < t.p2 ? t.p1 : t.p2;
            int p2 = t.p1 > t.p2 ? t.p1 : t.p2;
            
            //substr after first s1
            if (p1+t.length+min_check_length > p2) { continue; }
            char *pattern = returnSubstring(str, p1+t.length, min_check_length);
            
            //reverse substr after second s1
            int text_length = p2+max_check_length+t.length > str_len ? str_len - p2 - t.length : max_check_length;
            char *text = returnReversedSubstring(str, p2 + t.length, text_length);
            
            arr_with_size *result = KMP(text, pattern, text_length, min_check_length);
            for (int m = 0; m < result->size; m++) {
                
                
                
//                int start = p2 + t.length + (text_length - result->arr[m] - min_check_length);
//                int equal = 1;
//                for (int n = 0; n < start-p2-t.length; n++) {
//                    if (str[start-n-1] != str[p1+t.length+min_check_length+n] ) {
//                        equal = 0;
//                        break;
//                    }
//                }
                
                
                int extension_len = text_length - result->arr[m];
                int start = p2 + t.length;
                
                char* extension1 = returnReversedSubstring(str, start, extension_len);
                char* extension2 = returnSubstring(str, p1+t.length, extension_len);
                int distance = levenshtein(extension1, extension2, extension_len, extension_len);
                free(extension1);
                free(extension2);
                
                //                for (int n = 0; n < p1 - start; n++) {
                //                    if (str[start+n] != str[p2+t.length+n] ) {
                //                        equal = 0;
                //                        break;
                //                    }
                //                }
                if (distance <= mismatch_ratio*extension_len) {
//                if (equal == 1) {
                    count1++;
                    fprintf(first_repeat_index_file, "(%d,%d,%d,%d)\n", p1,  p2, extension_len+t.length, t.length);
                    
                    fprintf(first_repeat_seq_file, ">seq%d\n", count1);
                    fprintf(first_repeat_seq_file, "%s\n", returnSubstring(str, p1, extension_len+t.length));
                    fprintf(first_repeat_seq_file, ">seq%d-maximal\n", count1);
                    fprintf(first_repeat_seq_file, "%s\n", returnSubstring(str, p1, t.length));
                    fprintf(first_repeat_seq_file, ">seq%d-non-maximal\n", count1);
                    fprintf(first_repeat_seq_file, "%s\n", returnSubstring(str, p1+t.length, extension_len));
                    
                    //printf("(%d,%d,%d,%d)\n", p1,  p2, min_check_length+start-p2, t.length);
                }
            }
            free(result->arr);
            free(result);
            free(text);
            free(pattern);
            
            
            /*
             for (int len = min_check_length; len < max_check_length; len++) {
             
             if (p2 + len + t.length > str_len || p1 - len < 0) { break; }
             
             for (int k = 0; k < len; k++) {
             if (str[p1+t.length+k] != str[p2+t.length+len-k-1]) { break; }
             
             if (k == len - 1 && p1+len < p2) {
             count1++;
             
             fprintf(first_repeat_index_file, "(%d,%d,%d,%d)\n", p1,  p2, len+t.length, t.length);
             
             fprintf(first_repeat_seq_file, ">seq%d\n", count1);
             fprintf(first_repeat_seq_file, "%s\n", returnSubstring(str, p1, len+t.length));
             fprintf(first_repeat_seq_file, ">seq%d-maximal\n", count1);
             fprintf(first_repeat_seq_file, "%s\n", returnSubstring(str, p1, t.length));
             fprintf(first_repeat_seq_file, ">seq%d-non-maximal\n", count1);
             fprintf(first_repeat_seq_file, "%s\n", returnSubstring(str, p1+t.length, len));
             
             printf("(%d,%d,%d,%d)\n", p1, p2, len+t.length, len);
             
             }
             }
             }
             */
        }
    }
    fclose(first_repeat_index_file);
    fclose(first_repeat_seq_file);
    
    printf("-------------------\n");
    printf("Finding second type non-maximal revserse cirlce repeat\n");
    FILE* second_repeat_index_file = fopen("result/2nd-type reversed-circle repeat", "w");
    FILE* second_repeat_seq_file = fopen("result/2nd-type rev-cr seq", "w");
    //..s1s2...s1^(-1)s2^(-1)... s2 being maximal
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < arr[i]; j++) {
            
            triple t = result_list[i].result[j];
            int p1 = t.p1 < t.p2 ? t.p1 : t.p2;
            int p2 = t.p1 > t.p2 ? t.p1 : t.p2;
            
            
            //substr before first s2
            if (p1-min_check_length < 0 || p2-min_check_length < p1+t.length) { continue; }
            char *pattern = returnSubstring(str, p1-min_check_length, min_check_length);
            
            //reverse substr before second s2
            int text_length = p2-max_check_length < p1+t.length ? p2-p1-t.length : max_check_length;
            char *text = returnReversedSubstring(str, p2-text_length, text_length);
            
            arr_with_size *result = KMP(text, pattern, text_length, min_check_length);
            for (int m = 0; m < result->size; m++) {
//                int start = p2 - result->arr[m] - min_check_length;
//                int equal = 1;
//                for (int n = 0; n < p2-start; n++) {
//                    if (str[start+n] != str[p1-n-1] ) {
//                        equal = 0;
//                        break;
//                    }
//                }
                
                int extension_len = result->arr[m] + min_check_length;
                int start = p2 - extension_len;
                
                char* extension1 = returnReversedSubstring(str, p2-extension_len, extension_len);
                char* extension2 = returnSubstring(str, p1-extension_len, extension_len);
                int distance = levenshtein(extension1, extension2, extension_len, extension_len);
                free(extension1);
                free(extension2);
                
                if (distance <= mismatch_ratio*extension_len) {
//                if (equal == 1) {
                    count2++;
                    fprintf(second_repeat_index_file, "(%d,%d,%d,%d)\n", p1 - extension_len, p2 - extension_len, t.length+extension_len, t.length);
                    
                    fprintf(second_repeat_seq_file, ">seq%d\n", count2);
                    fprintf(second_repeat_seq_file, "%s\n", returnSubstring(str, p1-extension_len, extension_len+t.length));
                    fprintf(second_repeat_seq_file, ">seq%d-maximal\n", count2);
                    fprintf(second_repeat_seq_file, "%s\n", returnSubstring(str, p1, t.length));
                    fprintf(second_repeat_seq_file, ">seq%d-non-maximal\n", count2);
                    fprintf(second_repeat_seq_file, "%s\n", returnSubstring(str, p1-extension_len, extension_len));
                    
                    //printf("(%d,%d,%d,%d)\n", p1-(p2-start),  start, t.length+p2-start, t.length);
                }
            }
            free(result->arr);
            free(result);
            free(text);
            free(pattern);
            
            
            /*
             for (int len = min_check_length; len < max_check_length; len++) {
             
             for (int k = 0; k < max_check_length; k++) {
             
             if (str[p1-k-1] != str[p2-len+k]) {
             break;
             }
             
             if (k == len - 1 && p1+t.length+len<p2) {
             count2++;
             
             fprintf(second_repeat_index_file,"(%d,%d,%d,%d)\n", p1-len,p2-len,len+t.length,t.length);
             
             fprintf(second_repeat_seq_file, ">seq%d\n", count2);
             fprintf(second_repeat_seq_file, "%s\n", returnSubstring(str, p1-len, len+t.length));
             fprintf(second_repeat_seq_file, ">seq%d-maximal\n", count2);
             fprintf(second_repeat_seq_file, "%s\n", returnSubstring(str, p1, t.length));
             fprintf(second_repeat_seq_file, ">seq%d-non-maximal\n", count2);
             fprintf(second_repeat_seq_file, "%s\n", returnSubstring(str, p1-len, len));
             
             //printf("(%d,%d,%d,%d)\n", p1-len, p2-len, len+t.length, len);
             }
             }
             }
             */
        }
    }
    fclose(second_repeat_index_file);
    fclose(second_repeat_seq_file);
    
    printf("-------------------\n");
    printf("The total number of maximal repeat count is %lu\n", maximal_repeat_pair_count);
    printf("the number of first type non-maximal reverse cirlce repeat is %i \n", count1);
    printf("the number of second type non-maximal reverse cirlce repeat is %i\n", count2);
    
}

//MARK: maximal repeat finding helpers
//only used for reversed repeat finding
int past_pairs_min_index[1000000];
int past_pairs_len = 0;
tuple formCartisianProduct(linked_list list1[linked_list_num], linked_list list2[linked_list_num], int length, int strlen) {
    
    //    printf("----------------------\n");
    //    for (int i = 0; i < 5; i++) {
    //        if (list1[i].start != NULL) {
    //            printf("list1[%d] = {%d}\n", i, list1[i].start->data);
    //        }
    //
    //        if (list2[i].start != NULL) {
    //            printf("list2[%d] = {%d}\n", i, list2[i].start->data);
    //        }
    //    }
    
    //    if (list1[1].start != NULL) {
    //        if (list1[1].start->data == 14 && list1[1].start->next != NULL) {
    //            printf("haha %d\n", list1[1].start->next->data);
    //        }
    //    }
    
    int max_count = 200000;
    triple *result = malloc(sizeof(triple)*max_count);
    int count = 0;
    
    
    for (int i = 0; i < linked_list_num; i++) {
        for (int j = 0; j < linked_list_num; j++) {
            if (i == j) { continue; }
            Node *start1 = list1[i].start;
            Node *start2 = list2[j].start;
            
            while(start1 != NULL) {
                while(start2 != NULL) {
                    
                    if (count > max_count) {
                        printf("Min length is too short. Count of triple in a node exceeds %d\n", max_count);
                        break;
                    }
                    
                    
                    if (linked_list_num == 6) {
                        
                        int half_str_len_minus_1 = (strlen - 1)/2;
                        int a1 = start1->data < start2->data ? start1->data : start2->data;
                        int a2 = start1->data > start2->data ? start1->data : start2->data;
                        
                        if (a1 < half_str_len_minus_1 && a2 > half_str_len_minus_1) {
                            
                            a2 = strlen - length - a2;
                            
                            if (a1 == a2) {
                                start2 = start2->next;
                                continue;
                            }
                            
                            int min = a1 < a2 ? a1 : a2;
                            
                            int equal = 0;
                            for (int k = 0; k < past_pairs_len; k++) {
                                if (min == past_pairs_min_index[k]) {
                                    equal = 1;
                                    break;
                                }
                            }
                            
                            if (equal == 1) {
                                start2 = start2->next;
                                continue;
                            }
                            
                            triple t = {a1, a2, length};
                            result[count] = t;
                            count++;
                            
                            
                            
                            past_pairs_min_index[past_pairs_len] = min;
                            past_pairs_len++;
                            
                            //printf("(%d,%d,%d)\n", a1, a2, length);
                        }
                        
                    }
                    
                    
                    if (linked_list_num == 5) {
                        //printf("(%d, %d, %d)\n", start1->data, start2->data, length);
                        triple t = {start1->data, start2->data, length};
                        result[count] = t;
                        count++;
                    }
                    
                    start2 = start2->next;
                }
                start1 = start1->next;
                start2 = list2[j].start;
            }
        }
    }
    
    triple *result2 = malloc(sizeof(triple)*count);
    
    for (int i = 0; i < count; i++) {
        result2[i] = result[i];
    }
    
    free(result);
    
    tuple t = {result2, count};
    
    return t;
}

int numberOfChildren(treenode_t *node) {
    int count = 0;
    treenode_t *first_child = node->first_child;
    while (first_child != NULL) {
        first_child = first_child->next_sibling;
        count++;
    }
    return count;
}


linked_list *mergeLinkedLists(linked_list lists[linked_list_num][linked_list_num], int exclusion, int children_count) {
    
    linked_list *merged_list = malloc(sizeof(linked_list)*linked_list_num);
    
    for (int i = 0; i < linked_list_num; i++) {
        merged_list[i].start = NULL;
        merged_list[i].end = NULL;
    }
    
    //    for (int i = 0; i < linked_list_num; i++) {
    //        for (int j = 0; j < 5; j++) {
    //            printf("list[%d][%d] start:%d end:%d\n", i, j, lists[i][j].start->data, lists[i][j].end->data);
    //        }
    //    }
    
    
    for (int i = 0; i < children_count; i++) {
        if (i > exclusion) {
            for (int j = 0; j < linked_list_num; j++) {
                linked_list l = lists[i][j];
                if (l.start == NULL) { continue; }
                if (merged_list[j].start == NULL) {
                    //                    merged_list[j].start = malloc(sizeof(Node));
                    merged_list[j].start = l.start;
                    
                    //                    if (l.end != NULL) {
                    //                        merged_list[j].end = malloc(sizeof(Node));
                    //                    }
                    merged_list[j].end = l.end;
                    
                } else if (merged_list[j].end == NULL) {
                    
                    //merged_list[j].end = malloc(sizeof(Node));
                    if (l.end == NULL) {
                        merged_list[j].end = l.start;
                        merged_list[j].start->next = merged_list[j].end;
                    } else {
                        merged_list[j].end = l.end;
                        merged_list[j].start->next = l.start;
                    }
                    
                } else {
                    //merged_list[j].end->next = malloc(sizeof(linked_list));
                    merged_list[j].end->next = l.start;
                    
                    if (l.end == NULL) {
                        merged_list[j].end = l.start;
                    } else {
                        merged_list[j].end = l.end;
                    }
                }
            }
        }
    }
    
    
    return merged_list;
}

void freeLinkedList(linked_list merged_list[linked_list_num]) {
    
    for (int i = 0; i < linked_list_num; i++) {
        Node *tmp = merged_list[i].start;
        if (tmp == NULL) { continue; }
        //printf("%d", merged_list[i].start == NULL);
        
        while (merged_list[i].start->next != NULL) {
            tmp = merged_list[i].start;
            merged_list[i].start = merged_list[i].start->next;
            free(tmp);
        }
        free(merged_list[i].start);
    }
    
}



int convertCharToInt(char c) {
    if (c == 'A') {
        return 1;
    } else if (c == 'T') {
        return 2;
    } else if (c == 'C') {
        return 3;
    } else if (c == 'G') {
        return 4;
    } else if (c == '$') {
        return 0;
    } else if (c == '#') {
        return 5;
    }
    return -1;
}

char convertIntToChar(int i) {
    if (i == 1) {
        return 'A';
    } else if (i == 2) {
        return 'T';
    } else if (i == 3) {
        return 'C';
    } else if (i == 4) {
        return 'G';
    } else if (i == 0) {
        return '$';
    } else if (i == 5) {
        return '#';
    }
    return 'X';
}

//MARK: debug functions
void printArcLabel(char *str, treenode_t *node) {
    int start = node->arc_val.start;
    int end = node->arc_val.end;
    char s[end - start + 1];
    for (int i = start; i < end; i++) {
        s[i - start] = str[i];
    }
    s[end - start] = '\0';
    
    printf("Arc start: %d, end: %d ", start, end);
    if (end == strlen(str) + 1) {
        printf("%s$\n", s);
    } else {
        printf("%s\n", s);
    }
    
    
}

void printPathLabel(char *str, treenode_t *node) {
    int start = node->node_val.start;
    int end = node->node_val.end;
    char s[end - start + 1];
    for (int i = start; i < end; i++) {
        s[i - start] = str[i];
    }
    s[end - start] = '\0';
    printf("Path start: %d, end: %d ", start, end);
    if (end == strlen(str) + 1) {
        printf("%s$\n", s);
    } else {
        printf("%s\n", s);
    }
    
}

void printDictionaryInfo(treenode_t *node) {
    printf("%d ", node->first_child == NULL);
    printf("The dictionary is { ");
    int count = 0;
    for (int i = 0; i < linked_list_num; i++) {
        
        char c = convertIntToChar(i);
        Node *n = node->node_dic[i].start;
        printf("%c: [", c);
        while (n != NULL) {
            printf("%i, ", n->data);
            n = n->next;
            count++;
        }
        printf("], ");
    }
    printf(" }\n");
    
    //    printf("--------------------------\n");
    //    printf("count is %d", count);
    //    if (count == 0) {
    //        treenode_t *child = node->first_child;
    //        while (child != NULL) {
    ////            printDictionaryInfo(child);
    //            printf("%d", child->first_child == NULL);
    //            child = child->next_sibling;
    //        }
    //    }
    //    printf("--------------------------\n");
    
    
    
}

//MARK: major function
void outputRepeatedPairs(treenode_t *root, char *str, int threshold, int min_extension_len, float mis_perc, int reversed) {
    if (reversed == 0) {
        linked_list_num = 5;
    } else {
        linked_list_num = 6;
    }
    
    unsigned long len = strlen(str);
    
    treenode_t* s[len*2];
    treenode_t* q[len*2];
    int len_q = 1;
    int len_s = 0;
    q[0] = root;
    
    // bot-up traversal
    while (len_q != 0) {

        treenode_t *node = q[len_q - 1];
        s[len_s] = node;
        len_q--;
        len_s++;
        
        if (node->first_child != NULL) {
            treenode_t *first_child = node->first_child;
            q[len_q] = first_child;
            len_q++;
            while (first_child->next_sibling != NULL) {
                first_child = first_child->next_sibling;
                q[len_q] = first_child;
                len_q++;
            }
        }
    }
    
    int max_result_len = 200000;
    tuple results[max_result_len];
    int results_len = 0;

    
    while (len_s != 0) {
        treenode_t *node = s[len_s-1];
        for (int m = 0; m < linked_list_num; m++) {
            node->node_dic[m].start = NULL;
            node->node_dic[m].end = NULL;
        }
        
        int str_len = node->node_val.end - node->node_val.start;
        if (str_len < threshold) {
            
            treenode_t *child = node->first_child;
            while (child != NULL) {
                //printDictionaryInfo(child);
                freeLinkedList(child->node_dic);
                treenode_t *child_ptr = child;
                child = child->next_sibling;
                free(child_ptr);
            }
            len_s--;
            continue;
        }
        
        
        
        //node->done = 1;
        
        // store left char and corresponding list to each node
        // list an array with 5 elements, each one is a linked-list
        // use the list to find corresponding pairs
        
        if (node->first_child == NULL) {
            
            // if it's a leaf, create a list with a start node containting the string start position
            
            int start = node->node_val.start;
            int left_char_value = convertCharToInt(str[start - 1]);
            if (start == 0) { left_char_value = 0; }
            
            node->node_dic[left_char_value].start = malloc(sizeof(Node));
            node->node_dic[left_char_value].start->data = start;
            node->node_dic[left_char_value].start->next = NULL;
            
            //printDictionaryInfo(node);
            
            //            if (start == 0) {
            //                printf("%d", left_char_value);
            //                printf("hahahaha \n");
            //            }
            
            //            printf("%d ", start);
            //            printf("%c\n", str[start - 1]);
            //            printPathLabel(str, s[j]);
            
        } else {
            
            //            int c = 0;
            //            treenode_t *node_child = node->first_child;
            //            while (node_child != NULL) {
            //
            ////                printf("-----------------------\n");
            ////                //printPathLabel(str, node_child);
            //                printDictionaryInfo(node_child);
            //
            //                node_child = node_child->next_sibling;
            //                c++;
            //            }
            
            
            int number_of_children = numberOfChildren(node);
            
            linked_list children_lists[number_of_children][linked_list_num];
            treenode_t *node = s[len_s-1];
            treenode_t *first_child = node->first_child;
            for (int m = 0; m < number_of_children; m++) {
                for (int n = 0; n < linked_list_num; n++) {
                    children_lists[m][n].start = first_child->node_dic[n].start;
                    children_lists[m][n].end = first_child->node_dic[n].end;
                }
                first_child = first_child->next_sibling;
            }
            
            
            //add the repeat pairs to result
            for (int m = 0; m < number_of_children; m++) {
                linked_list *list2 = mergeLinkedLists(children_lists, m, number_of_children);
                tuple result = formCartisianProduct(children_lists[m], list2, str_len, len);
                
                if (result.size != 0) {
                    results[results_len] = result;
                    results_len++;
                }
                
                //                freeLinkedList(list2);
                //                free(list2);
            }
            
            treenode_t *child = node->first_child;
            
            while (child != NULL) {
                
                for (int k = 0; k < linked_list_num; k++) {
                    if (child->node_dic[k].start == NULL) { continue; }
                    
                    if (node->node_dic[k].start == NULL) {
                        // no entry has been recorded here, so create list same as the child entry
                        node->node_dic[k].start = malloc(sizeof(Node));
                        node->node_dic[k].start = child->node_dic[k].start;
                        
                        if (child->node_dic[k].end != NULL) {
                            node->node_dic[k].end = malloc(sizeof(Node));
                        }
                        node->node_dic[k].end = child->node_dic[k].end;
                        
                    } else if (node->node_dic[k].end == NULL) {
                        // only start node created (i.e. one item in the linked list and end node not created)
                        // check children lists
                        
                        node->node_dic[k].end = malloc(sizeof(Node));
                        if (child->node_dic[k].end == NULL) {
                            // if children also one has one node (end node not created)
                            // 1. set end to child->start
                            // 2. set start->next to end
                            node->node_dic[k].end = child->node_dic[k].start;
                            node->node_dic[k].start->next = node->node_dic[k].end;
                            
                            
                        } else {
                            // if children has more then one children (end node created)
                            // 1. set end node to child->end
                            // 2. set start->next to child->start
                            
                            node->node_dic[k].end = child->node_dic[k].end;
                            node->node_dic[k].start->next = child->node_dic[k].start;
                        }
                        
                    } else {
                        // the node has an end node and start node
                        // set node->end->next to child->start
                        // check child node
                        node->node_dic[k].end->next = child->node_dic[k].start;
                        
                        if (child->node_dic[k].end == NULL) {
                            node->node_dic[k].end = child->node_dic[k].start;
                        } else {
                            node->node_dic[k].end = child->node_dic[k].end;
                        }
                    }
                    //                    free(child->node_dic[k].start);
                    //                    free(child->node_dic[k].end);
                }
                
                child = child->next_sibling;
            }
            
        }
        
        len_s--;
    }
    
    if (reversed == 0) {
        findCircleDuplication(results, results_len, str, 0.05, mis_perc, min_extension_len);
    }
    
    
    if (reversed == 1) {
        findReverseCircleDuplication(results, results_len, str, 0.05, mis_perc, min_extension_len);
    }
    
    
    
    for (int m = 0; m < results_len; m++) {
        free(results[m].result);
    }
    
}
























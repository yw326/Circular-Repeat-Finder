//
//  file_helper.h
//  GeneSeq
//
//  Created by Yiming Wang on 2019/3/11.
//  Copyright © 2019 Yiming Wang. All rights reserved.
//

#ifndef file_helper_h
#define file_helper_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>


#endif /* file_helper_h */

char *getStrFromFile(char *file_name);
char *getComplementSeqFromFile(char *file_name);
char *getConcantenatedReversedStrFromFile(char *file_name);
char *getConcantenatedReversedComplementStrFromFile(char *file_name);

int count_num_files_in_dir(char* dir);

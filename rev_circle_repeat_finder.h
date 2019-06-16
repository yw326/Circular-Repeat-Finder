//
//  rev_circle_repeat_finder.h
//  GeneSeq
//
//  Created by Yiming Wang on 2019/3/18.
//  Copyright © 2019 Yiming Wang. All rights reserved.
//

#ifndef rev_circle_repeat_finder_h
#define rev_circle_repeat_finder_h

#include <stdio.h>

#include <stdio.h>
#include "sfxtree.h"
#include "maximal_repeat.h"
#include "edit_distance.h"

#endif /* rev_circle_repeat_finder_h */

void findReverseApproximateCircleRepeat(triple_list *my_result_list, int size, char *str, double mismatch_ratio, int min_extension_len, int max_check_len, char* output_file_path, int task, int pound_idx, int partition_num1, int partition_num2, unsigned long partition_size);

int* get_result_minimizing_dist2(char* s1, char* s2, char* s3, int len);

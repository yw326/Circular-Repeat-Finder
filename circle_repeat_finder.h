//
//  circle_repeat_finder.h
//  GeneSeq
//
//  Created by Yiming Wang on 2019/3/11.
//  Copyright © 2019 Yiming Wang. All rights reserved.
//

#ifndef circle_repeat_finder_h
#define circle_repeat_finder_h

#include <stdio.h>
#include "sfxtree.h"
#include "maximal_repeat.h"
#include "edit_distance.h"

#endif /* circle_repeat_finder_h */

void findApproximateCircleRepeat(tuple *my_result_list, int size, char *str, double mismatch_ratio, int min_extension_len, int max_check_len);


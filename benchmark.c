//#include "benchmark.h"
//#include <stdint.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <inttypes.h>
//#include <math.h>
//#ifdef __APPLE__
//#include <mach/mach_time.h>
//#elif _WIN32
//#include <windows.h>
//#elif __linux__
//#include <time.h>
//#elif
//#define NOBENCH
//#endif
//
//
//struct {
//#ifdef __APPLE__
//    uint64_t stime;
//#elif _WIN32
//    LONGLONG stime;
//#elif __linux__
//    struct timespec stime;
//#endif
//} timer_state;
//
//
//void timerStart(void)
//{
//#ifdef __APPLE__
//    timer_state.stime = mach_absolute_time();
//#elif _WIN32
//    QueryPerformanceCounter((LARGE_INTEGER*)&(timer_state.stime));
//#elif __linux__
//    clock_gettime(CLOCK_MONOTONIC_RAW, &(timer_state.stime));
//#endif
//}
//
//
//uint64_t timerStop(void)
//{
//    uint64_t diff;
//
//#ifdef __APPLE__
//    uint64_t etime;
//    mach_timebase_info_data_t info;
//
//    etime = mach_absolute_time();
//    mach_timebase_info(&info);
//    diff = (etime - timer_state.stime) * info.numer / info.denom;
//#elif _WIN32
//    LONGLONG etime;
//    LONGLONG freq;
//    uint64_t tmp1, tmp2;
//
//    QueryPerformanceCounter((LARGE_INTEGER*)&etime);
//    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
//    diff = etime - timer_state.stime;
//    tmp1 = (diff * 1000000) / freq;
//    tmp2 = ((diff * 1000000) % freq) * 1000 / freq;
//    diff = tmp1 * 1000 + tmp2;
//#elif __linux__
//    struct timespec etime;
//    uint64_t t0, t1;
//
//    clock_gettime(CLOCK_MONOTONIC_RAW, &etime);
//    t0 = timer_state.stime.tv_nsec + timer_state.stime.tv_sec * 1000000000;
//    t1 = etime.tv_nsec + etime.tv_sec * 1000000000;
//    diff = t1 - t0;
//#endif
//
//    return diff;
//}
//
//
//uint64_t xorshift64star(void)
//{
//    static uint64_t x = UINT64_C(1970835257944453882);
//    x ^= x >> 12;
//    x ^= x << 25;
//    x ^= x >> 27;
//    return x * UINT64_C(2685821657736338717);
//}
//
//
//int randomInRange(int a, int b)
//{
//    int d = b - a;
//    if (d <= 0)
//        return a;
//    uint64_t m = UINT64_MAX - UINT64_MAX % d;
//    uint64_t n = xorshift64star();
//    while (n > m)
//        n = xorshift64star();
//    return n % d + a;
//}
//
//
//#define CHARSET_END ('z'+1)    /* not included */
//#define CHARSET_START ('a')
//
//void genTestCase(int len, char *out, int entropy)
//{
//    int i, lme=0, ume=0;
//
//    for (i=0; i<len; i++) {
//        if (i == 0) {
//            out[i] = (char)randomInRange(CHARSET_START, CHARSET_END);
//        } else {
//            int lm = (CHARSET_START - out[i-1]) * entropy + lme;
//            lme = lm % 50;
//            int um = (CHARSET_END - out[i-1]) * entropy + ume;
//            ume = um % 50;
//            lm /= 50;
//            um /= 50;
//            out[i] = out[i-1] + randomInRange(lm, um);
//        }
//    }
//
//    out[i] = '\0';
//}
//
//
//#define SAMPLE_SIZE (15)
//#define PREHEAT_SIZE (1)
//#define MAX_STR (5000)
//#define MIN_STR (1)
//#define TRIAL_CNT (500)
//
//void benchmark(treenode_t *(*sfxt)(const char *str))
//{
//    int i, n, l, e, c, trial;
//    uint64_t time;
//    char buf[MAX_STR+1];
//    treenode_t *trees[SAMPLE_SIZE+PREHEAT_SIZE];
//
//#ifdef NOBENCH
//    exit(1);
//#endif
//
//    puts("$data << END");
//
//    for (trial=0; trial<TRIAL_CNT; trial++) {
//        c = trial % 3;
//        if (c == 0)
//            e = 0;
//        else if (c == 1)
//            e = 49;
//        else
//            e = randomInRange(1, 10);
//        l = randomInRange(MIN_STR, MAX_STR);
//        genTestCase(l, buf, e);
//
//        n = 0;
//
//        for (i=0; i<PREHEAT_SIZE; i++)
//            trees[n++] = sfxt(buf);
//
//        timerStart();
//        for (i=0; i<SAMPLE_SIZE; i++)
//            trees[n++] = sfxt(buf);
//        time = timerStop();
//
//        for (i=0; i<SAMPLE_SIZE+PREHEAT_SIZE; i++)
//            freeTree(trees[i]);
//
//        printf("%d, %" PRIu64 ".%04" PRIu64 ", %d, %s\n", l,
//               time / (SAMPLE_SIZE * 1000),
//               (time % (SAMPLE_SIZE * 1000)) * 10000/(SAMPLE_SIZE * 1000), e, buf);
//    }
//
//    puts("END\n"
//         "f(x) = a*x**2 + b*x + c\n"
//         "fit f(x) $data u 1:2 via a, b, c\n"
//         "title_f(a, b, c) = sprintf('f(x) = %.2fx^2 + %.2fx + %.2f', a, b, c)\n"
//         "set palette rgbformulae 33, 13, 10\n"
//         "set xlabel \"strlen(s)\"\n"
//         "set ylabel \"T [us]\"\n"
//         "set cblabel \"complexity\"\n"
//         "plot $data using 1:2:3 w points palette t \"data\", f(x) t title_f(a, b, c)\n");
//}
//
//
//

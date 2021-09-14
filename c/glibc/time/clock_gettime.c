/* Copyright (c) Colorado School of Mines, CST.*/
/* All rights reserved.                       */

#include <signal.h>
#include <time.h>
#include <stdio.h>



#define test(clk_id) {\
    struct timespec tp;\
    clock_gettime(clk_id, &tp);\
    printf("%40s: %ld, %ld\n", #clk_id, tp.tv_sec, tp.tv_nsec);\
}

int main()
{
    test(CLOCK_REALTIME);
    test(CLOCK_REALTIME_COARSE);
    test(CLOCK_MONOTONIC);
    test(CLOCK_MONOTONIC_COARSE);
    test(CLOCK_MONOTONIC_RAW);
    test(CLOCK_BOOTTIME);
    test(CLOCK_PROCESS_CPUTIME_ID);
    test(CLOCK_THREAD_CPUTIME_ID);
}


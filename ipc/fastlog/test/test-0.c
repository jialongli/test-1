#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <signal.h>

#define __USE_GNU
#include <sched.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>

#include <fastlog.h>

#include "common.h"

bool signal_exit = false;

void signal_handler(int signum)
{
    switch(signum) {
    case SIGINT:
        printf("Catch ctrl-C.\n");
        fastlog_exit();
        break;
    }
    exit(1);
}

int main(int argc, char *argv[])
{
    int nthread = 1;
    
    signal(SIGINT, signal_handler);

    fastlog_init(FASTLOG_ERR/*, "meta.out", "log.out"*/, NULL, NULL, 6, 10*1024*1024/* 10MB */, 3/*CPU3*/);

    fastlog_setlevel(FASTLOG_DEBUG);
    
    
    FAST_LOG(FASTLOG_INFO, "MAIN", "start to run...\n");

#if 0
    printf("####### Benchmark #######\n");
    test_benchmark();
    sleep(5);
    printf("####### Benchmark #######\n");
    test_benchmark();
#elif 1
    if(argc >= 2) {
        
        if(strcmp(argv[1], "modules") == 0) {
            
            printf("####### Modules #######\n");
            modules_init();

            modules_loop();

            goto normal_exit;
            
        } else if(strcmp(argv[1], "benchmark") == 0) {
        
            char *ofile = NULL;
            
            if(argc >= 3) {
                nthread = atoi(argv[2]);
                if(nthread <= 0) {
                    goto error_exit;
                }
            }
            
            if(argc >= 4) {
                ofile = argv[3];
            }
            
            printf("####### Benchmark #######\n");
            test_benchmark(nthread, ofile);

            goto normal_exit;
        } else {
            goto error_exit;
        }
    } else {
        goto error_exit;
    }

error_exit:
    printf("%s [modules|benchmark] [nthread>0] [stats file].\n", argv[0]);
    
normal_exit:
#endif
    fastlog_exit();
	return 0;
}


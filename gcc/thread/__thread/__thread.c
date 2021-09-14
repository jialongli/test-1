//https://stackoverflow.com/questions/32245103/how-does-the-gcc-thread-work
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <time.h>
//#include <linux/getcpu.h>

#define __USE_GNU
#include <sched.h>
#include <ctype.h>
#include <string.h>


pthread_key_t key;

__thread int data __attribute__((tls_model("local-exec"))) = 0;

struct log_struct {
    FILE  * fp;
    char filename[256];
};

struct thread_arg {
    int i;
};

int get_by_key (void) {
    return *(int*) (pthread_getspecific (key));
}


void* task_routine(void* parameters)
{
    struct thread_arg * arg = (struct thread_arg*)parameters;
    
    int i = arg->i;
    while(i>=0) {
        data++;
        i--;
    }

    printf("%ld: data = %d(%p)\n", pthread_self(), data, &data);
    pthread_exit(arg);
}

/* The main program. */
int main ()
{
    int ncpu = sysconf (_SC_NPROCESSORS_ONLN);
    int i;
	pthread_t tids[4096];
    int nr_threads = 8;
    
    for(i=0; i<nr_threads;i++) {
        struct thread_arg * arg = malloc(sizeof(struct thread_arg));
        arg->i = i;
	    pthread_create(&tids[i], NULL, &task_routine, (void*)arg);

        arg = NULL;
    }

    for(i=0; i<nr_threads;i++) {
        void *p = NULL;
        
        pthread_join(tids[i], &p);

        free(p);
    }

    system("more /proc/self/maps");

    printf("threads exit.\n");
    printf("%ld: data = %d(%p)\n", pthread_self(), data, &data);
	return 0;
}


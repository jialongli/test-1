#include <stdio.h>
#include <linux/futex.h>
#include <sys/syscall.h>
#include <errno.h>
#include <stdlib.h>
//#include <stdatomic.h>
#include <pthread.h>


struct worker_args{
    int *futex_1;
    int *futex_2;
};


int futex(int *uaddr, int futex_op, int val,
        const struct timespec *timeout, int *uaddr2, int val3)
{
    return syscall(SYS_futex, uaddr, futex_op, val,
            timeout, uaddr, val3);
}

static void fwait(int *futexp)
{
    int s;
    while (1) {
        /* Is the futex available? */
        if (__sync_bool_compare_and_swap(futexp, 0, 1)) {
            break;      /* Yes */
        }

        /* Futex is not available; wait */

        s = futex(futexp, FUTEX_WAIT, 0, NULL, NULL, 0);
        // __sync_synchronize();
        if (s == -1 && errno != EAGAIN)
            exit(0);
    }
}

static void fpost(int *futexp)
{
    int s = 0;
    if (__sync_bool_compare_and_swap(futexp, 1, 0)) {
        /* Prevent the lost wake up problem. */
        while (*futexp == 0){   
            s = futex(futexp, FUTEX_WAKE, 1, NULL, NULL, 0);
            if (s  == -1)
                exit(0);
        }
    }
}

void* worker(void* param){
    int i;

    for ( i = 0; i < 8; i++){
        fwait(((struct worker_args*) param)->futex_1);
        printf("Thread\n");
        fpost(((struct worker_args*) param)->futex_2);
    }
    return NULL;
}

int main(){
    pthread_t thread;
    int ret;
    int futex_1 = 0;
    int futex_2 = 1;
    struct worker_args args;
    int i;
    
    args.futex_1 = &futex_1;
    args.futex_2 = &futex_2;

    ret = pthread_create( &thread, NULL, worker, (void*)&args);

    for ( i = 0; i < 8; i++){
        fwait(args.futex_2);
        printf("Main\n");

        if (i == 7){
            break;
        }

        fpost(args.futex_1);
    }

    pthread_join( thread, NULL);

    return 0;
}

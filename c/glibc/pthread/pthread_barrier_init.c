
#include <pthread.h>
#include <stdio.h>

#define NR_THREAD   10

pthread_barrier_t barrier;


void* test_task_fn(void* unused)
{
	printf("test_task_fn %ld.\n", pthread_self());

    sleep(pthread_self()%NR_THREAD);

    pthread_barrier_wait(&barrier);

    printf("thread %ld done.\n", pthread_self());
    
    pthread_exit(NULL);
	return NULL;
}

/* The main program. */
int main ()
{

    int *pstatus;
    int i;
	pthread_t threadids[NR_THREAD];

    pthread_barrier_init(&barrier, NULL, NR_THREAD);

    for(i=0;i<NR_THREAD;i++)
	    pthread_create(&threadids[i], NULL, test_task_fn, NULL);
    
    for(i=0;i<NR_THREAD;i++)
        pthread_join(threadids[i], (void**)&pstatus);

    pthread_barrier_destroy(&barrier);

    
	return 0;
}





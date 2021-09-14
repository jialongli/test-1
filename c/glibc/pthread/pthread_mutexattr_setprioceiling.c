
#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mutex;
pthread_mutexattr_t mutexattr;


void* test_task1_fn(void* unused)
{
	printf("test_task1_fn.\n");

    pthread_mutex_lock(&mutex);
    
	printf("task1: .\n");
    
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
	return NULL;
}


void* test_task2_fn(void* unused)
{
	printf("test_task2_fn.\n");

    pthread_mutex_lock(&mutex);
    
	printf("task2: .\n");
    
    pthread_mutex_unlock(&mutex);

    
    pthread_exit(NULL);
	return NULL;
}


/* The main program. */
int main ()
{
    int *pstatus;
	pthread_t thread_id1, thread_id2;

    pthread_mutexattr_init(&mutexattr);

    pthread_mutexattr_setrobust(&mutexattr, PTHREAD_MUTEX_ROBUST);
    pthread_mutexattr_setprotocol(&mutexattr, PTHREAD_PRIO_INHERIT);
    pthread_mutexattr_setprioceiling(&mutexattr, 12);
    
    pthread_mutex_init(&mutex, &mutexattr);

    
	pthread_create(&thread_id1, NULL, test_task1_fn, NULL);
	pthread_create(&thread_id2, NULL, test_task2_fn, NULL);



    
	pthread_join(thread_id1, (void**)&pstatus);
	pthread_join(thread_id1, (void**)&pstatus);


    pthread_mutex_destroy(&mutex);
    
	return 0;
}





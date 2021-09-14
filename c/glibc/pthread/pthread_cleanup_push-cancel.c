#include <malloc.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>

/* Allocate a temporary buffer. */
void* allocate_buffer (size_t size)
{
    printf("allocate_buffer.\n");
	return malloc (size);
}
/* Deallocate a temporary buffer. */
void deallocate_buffer (void* buffer)
{
    printf("deallocate_buffer.\n");
	free (buffer);
}


void some_oth_cleanup(void* buffer)
{
    printf("some_oth_cleanup.\n");
}


void* test_task_fn(void* unused)
{
//	pthread_cleanup_push (some_oth_cleanup, NULL);
//	pthread_cleanup_pop (1);
    
	printf("test_task_fn.\n");
    static int status = 12121;
    
	/* Allocate a temporary buffer. */
	void* temp_buffer = allocate_buffer (1024);

	/* Register a cleanup handler for this buffer, to deallocate it in
	 * case the thread exits or is cancelled. */
	pthread_cleanup_push (deallocate_buffer, temp_buffer);;
    

    /* Do some work here that might call pthread_exit or might be
	 * cancelled... */
    sleep(1);
    
	/* Unregister the cleanup handler. Because we pass a nonzero value,
	 * this actually performs the cleanup by calling
	 * deallocate_buffer. */
	pthread_cleanup_pop (1);
    
    pthread_exit(&status);
	return NULL;
    
}

/* The main program. */
int main ()
{
    int *pstatus;
	pthread_t thread_id;
    struct timespec abstime = {1,1};;

    time_t abst = time(&abst); //man 2 time
    abstime.tv_sec = abst+2;
    
	pthread_create(&thread_id, NULL, test_task_fn, NULL);

	pthread_timedjoin_np(thread_id, (void**)&pstatus, &abstime);

    pthread_cancel(thread_id);

	return 0;
}

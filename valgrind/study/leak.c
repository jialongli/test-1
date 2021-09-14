#include <malloc.h>
#include <stdio.h>
#include <pthread.h>

#ifndef NOLEAK
#define LEAK 1
#endif

void* test_task_fn(void* unused)
{
	printf("test_task_fn.\n");
    
    char *str = malloc(64);

    int i;
#ifdef LEAK    
    for (i=0; i<2; i++) {
        str = malloc(64);
    }
#endif //LEAK    
    free(str);

    pthread_exit(NULL);
	return NULL;
}

/* The main program. */
int main ()
{
	pthread_t thread_id;
    
	pthread_create(&thread_id, NULL, test_task_fn, NULL);

	pthread_join(thread_id, NULL);

    printf("Exit program.\n");

	return 0;
}


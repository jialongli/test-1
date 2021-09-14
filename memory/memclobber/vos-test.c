#include <vos.h>
#include <stdio.h>
#include <malloc.h>
#include <pthread.h>

void demo_test1() {
    
    char *str1 = vos_malloc(64);
    char *str2 = vos_malloc(64);

    static int N = 0;

    if((++N)%3==0) {
//        str2[-1] = 'A';
//        str2[64] = 'A';
    }
    
    str1 = vos_realloc(str1, 128);

    sprintf(str1, "Hello World.");
    printf("str1 = %s\n", str1);
    char *str3 = vos_strdup(str1);
    printf("str3 = %s\n", str3);
    vos_free(str3);
    
    vos_free(str1);
    str1 = vos_malloc(128);

    vos_free(str1);
    vos_free(str2);
}

void* test_task_fn(void* unused)
{
	printf("test_task_fn.\n");
    
    int i, j=0;
    while(1) {
        printf("while loop -> %d.\n", ++j);
        demo_test1();
        sleep(1);
    }

    pthread_exit(NULL);
	return NULL;
}

/* The main program. */
int main(int argc, char *argv[]) 
{
    int i;
    for(i=0;i<argc;i++){
        printf("argv[%d] = %s\n", i, argv[i]);
    }

	pthread_t thread_id;
    
	pthread_create(&thread_id, NULL, test_task_fn, NULL);

	pthread_join(thread_id, NULL);

    printf("Exit program.\n");

	return 0;
}




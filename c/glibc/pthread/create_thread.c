#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <sched.h>
#include <errno.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <math.h>

#define HAS_PRIORITY_INHERIT 1


struct thread {
//	struct list_head _threads;
	pthread_t pthread;
	pthread_attr_t attr;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	void *arg;
	void *(*func)(void*);
	int priority;
	int policy;
	int flags;
	int id;
};
typedef struct { volatile int counter; } atomic_t;

static int _use_pi = 1;

void init_pi_mutex(pthread_mutex_t * m)
{
#if HAS_PRIORITY_INHERIT
	pthread_mutexattr_t attr;
	int ret;
	int protocol;

	if ((ret = pthread_mutexattr_init(&attr)) != 0) {
		printf("Failed to init mutexattr: %d (%s)\n", ret,
		       strerror(ret));
	};
	if (_use_pi
	    && (ret =
		pthread_mutexattr_setprotocol(&attr,
					      PTHREAD_PRIO_INHERIT)) != 0) {
		printf("Can't set protocol prio inherit: %d (%s)\n", ret,
		       strerror(ret));
	}
	if ((ret = pthread_mutexattr_getprotocol(&attr, &protocol)) != 0) {
		printf("Can't get mutexattr protocol: %d (%s)\n", ret,
		       strerror(ret));
	}
	if ((ret = pthread_mutex_init(m, &attr)) != 0) {
		printf("Failed to init mutex: %d (%s)\n", ret, strerror(ret));
	}
#endif

	/* FIXME: does any of this need to be destroyed ? */
}

struct thread * create_thread(void *(*func) (void *), void *arg, int prio, int policy)
{
	struct sched_param param;
	int id, ret;
	struct thread *thread;

//	id = atomic_inc(&_thread_count);

	thread = malloc(sizeof(struct thread));
	if (!thread)
		return NULL;

//	list_add_tail(&thread->_threads, &_threads);
	pthread_cond_init(&thread->cond, NULL);	// Accept the defaults
	init_pi_mutex(&thread->mutex);
	thread->id = id;
	thread->priority = prio;
	thread->policy = policy;
	thread->flags = 0;
	thread->arg = arg;
	thread->func = func;
	param.sched_priority = prio;

	pthread_attr_init(&thread->attr);
	pthread_attr_setinheritsched(&thread->attr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&thread->attr, thread->policy);
	pthread_attr_setschedparam(&thread->attr, &param);

	if ((ret =
	     pthread_create(&thread->pthread, &thread->attr, func,
			    (void *)thread))) {
		printf("pthread_create failed: %d (%s)\n", ret, strerror(ret));
//		list_del(&thread->_threads);
		pthread_attr_destroy(&thread->attr);
		free(thread);
		return NULL;
	}
	pthread_attr_destroy(&thread->attr);

	return thread;
}

struct thread *  create_fifo_thread(void *(*func) (void *), void *arg, int prio)
{
	return create_thread(func, arg, prio, SCHED_FIFO);
}

struct thread *  create_rr_thread(void *(*func) (void *), void *arg, int prio)
{
	return create_thread(func, arg, prio, SCHED_RR);
}

struct thread *  create_other_thread(void *(*func) (void *), void *arg)
{
	return create_thread(func, arg, 0, SCHED_OTHER);
}

void* f1(void*arg) {
    
}
#ifndef NO_CREATE_PTHREAD_MAIN_TEST
int main()
{
    create_fifo_thread(f1, NULL, 10);
    return 0;
}
#endif

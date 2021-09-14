/**
 *  pthread_atfork 示例程序
 *  
 *  作者：rtoax
 *  日期：2020年11月18日
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
 
#define LOG_ERROR(err, msg) do { errno = err; perror(msg); exit(-1); } while(0)

#define log_parent(fmt...)  do{printf("\033[33m[%d]", getpid());printf(fmt);printf("\033[m");}while(0)
#define log_child(fmt...)  do{printf("\033[32m[%d]", getpid());printf(fmt);printf("\033[m");}while(0)


int count = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void sig_handler(int signum)
{
    printf("\033[m Catch Ctrl-C.\n");
    exit(1);
}

void atfork_prepare() {
    int err;
    log_parent("atfork_prepare: pthread_mutex_lock ...\n");
    err = pthread_mutex_lock(&lock);
    if (err != 0) LOG_ERROR(err, "atfork_prepare: pthread_mutex_lock failed");
}
 
void atfork_parent() {
    int err;
    log_parent("atfork_parent: pthread_mutex_unlock ...\n");
    err = pthread_mutex_unlock(&lock);
    if (err != 0) LOG_ERROR(err, "atfork_parent: pthread_mutex_unlock");
}
 
void atfork_child() {
    int err;
    log_child("atfork_child: pthread_mutex_unlock ...\n");
    err = pthread_mutex_unlock(&lock);
    if (err != 0) LOG_ERROR(err, "atfork_child: pthread_mutex_unlock");
}
 
 
void* thread_proc(void* arg) {
    while(1) {
        pthread_mutex_lock(&lock);
        count++;
        log_parent("parent thread:  count:%d\n",count);
        sleep(5);
        pthread_mutex_unlock(&lock);
        sleep(1);
    }
    return NULL;
}
 
int main(int argc,char * argv[])
{
    int err;
    pid_t pid;
    pthread_t tid;

    signal(SIGINT, sig_handler);

    /* 创建线程 */
    pthread_create(&tid, NULL, thread_proc, NULL);

    /* atfork操作 */
    err = pthread_atfork(atfork_prepare, atfork_parent, atfork_child);
    if (err != 0) LOG_ERROR(err, "atfork");

    sleep(1);
    
    log_parent("parent is about to fork start...\n");

    pid = fork();
    
    
    if (pid < 0) {
        LOG_ERROR(errno, "fork");
    } else if (pid == 0) {
        // child process
        int status;
        log_child("child running\n");
    
        while(1) {
            pthread_mutex_lock(&lock);
            count ++;
            log_child("child: count:%d\n",count);
            sleep(2);
            pthread_mutex_unlock(&lock);
            sleep(1);
        }
        exit(0);
    }
    log_parent("parent is about to fork done...\n");

    pthread_join(tid, NULL);
}



#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
 
int main() {
    int fd;
    pid_t child;
    char buf[BUFSIZ] = "";
    ssize_t br;
 
    fd = syscall(SYS_memfd_create, "foofile", 0);
    if (fd == -1) {
        perror("memfd_create");
        exit(EXIT_FAILURE);
    }
 
    child = fork();
    if (child == 0) {
        //子进程将 stdout 重定向至 memory fd 中
        dup2(fd, 1);
        close(fd);
        execlp("/bin/date", "", NULL);
        perror("execlp date");
        exit(EXIT_FAILURE);
    } else if (child == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
 
    waitpid(child, NULL, 0);

    //父进程从 memmory fd 中读取
    lseek(fd, 0, SEEK_SET);
    br = read(fd, buf, BUFSIZ);
    if (br == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    buf[br] = 0;
 
    printf("pid:%d\n", getpid());
    printf("child said: '%s'\n", buf);
    pause();
    exit(EXIT_SUCCESS);
}


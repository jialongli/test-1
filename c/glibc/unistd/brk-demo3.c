#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

int main(void)
{
    void *tret;
    char *pmem;
    int i;
    long sbrkret;

    pmem = (char *)malloc(32);
    if (pmem == NULL) {
    perror("malloc");
    exit (EXIT_FAILURE);
    }

    printf ("pmem:%p\n", pmem);

    for (i = 0; i < 65; i++) {
        sbrk(1);
        //0x20ff8 就是堆和 bss段 之间的空隙常数；
        //改变后要用 sbrk(0) 再次获取更新后的program break位置
        printf ("%d\n", sbrk(0) - (long)pmem - 0x20ff8);   
    }
    free(pmem);
    
    return 0;
}
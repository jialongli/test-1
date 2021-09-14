#include <stdio.h>
#include <numa.h>


//gcc numa_available.c -lnuma


int main()
{
    char *str = numa_alloc(numa_pagesize());

    printf("numa_max_node = %d\n", numa_max_node());

    numa_free(str, numa_pagesize());
}



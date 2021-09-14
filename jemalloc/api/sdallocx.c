#include <stdio.h>
#include <jemalloc/jemalloc.h>

int main()
{
    int *I = mallocx(1024, MALLOCX_LG_ALIGN(8));

    size_t s1 = sallocx(I, 0);
    printf("size = %d.\n", s1);
    
    sdallocx(I, 256, MALLOCX_ZERO);

//    size_t s2 = xallocx(I, 1024, 1024, MALLOCX_ZERO);
//    printf("size = %d.\n", s2);
    
    size_t s3 = sallocx(I, 0);
    printf("size = %d.\n", s3);
    
    free(I);
}

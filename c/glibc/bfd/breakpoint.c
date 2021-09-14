
#include <stdio.h>

#if __SIZEOF_POINTER__ > 4
#define RTOAX_BREAKPOINT_PTR ".quad"
#else
#define RTOAX_BREAKPOINT_PTR ".long"
#endif


#define RTOAX_BREAKPOINT \
    asm("0:"                              \
        ".pushsection embed-breakpoints;" \
        RTOAX_BREAKPOINT_PTR" 0b;"       \
        ".popsection;")


int main() {
    printf("Hello,\n");
    RTOAX_BREAKPOINT;
    printf("world!\n");
    RTOAX_BREAKPOINT;
    return 0;
}


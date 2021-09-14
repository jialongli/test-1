#include <stdio.h>

#include <mlinklist.h>
#include <common.h>

int main()
{
    int i;
    mll_t mll;

    struct ll_node nodes[] = {
        {1, NULL}, {2, NULL}, {3, NULL}, {4, NULL}, 
    };
    
    create_mll(&mll, MMAP_FILENAME, sizeof(struct ll_node)*LINKLIST_SIZE, 1);
    
    for(i=0; i<sizeof(nodes)/sizeof(nodes[0]) && i < LINKLIST_SIZE; i++) {
        mll_insert(&mll, &nodes[i]);
    }
}


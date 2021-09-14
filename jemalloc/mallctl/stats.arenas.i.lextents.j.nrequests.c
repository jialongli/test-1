#include <stdio.h>
#include <jemalloc/jemalloc.h>

//stats.arenas.<i>.lextents.<j>.nrequests (uint64_t) r- [--enable-stats]
//	Cumulative number of allocation requests satisfied by large extents of the corresponding size class.

void jemalloc_arena_narenas(unsigned *pnarenas)
{
    size_t len;

    len = sizeof(*pnarenas);
    je_mallctl("arenas.narenas", pnarenas, &len, NULL, 0);
//    printf("narenas = %ld.\n", *pnarenas);
    
}

unsigned jemalloc_arenas_nlextents()
{
    unsigned nlextents;
    size_t len;

    len = sizeof(nlextents);
    je_mallctl("arenas.nlextents", &nlextents, &len, NULL, 0);
//    printf("nlextents = %d.\n", nlextents);
    return nlextents;
}



void jemalloc_stats_arenas_i_lextents_j_nrequests()
{
    unsigned narenas, i, j;
    
    size_t mib[6];
    size_t len, miblen;
    size_t value;
    
    len = 32;
    
    jemalloc_arena_narenas(&narenas);

    miblen = 6;
    
    je_mallctlnametomib("stats.arenas.0.lextents.0.nrequests", mib, &miblen);
    for (i = 0; i < narenas; i++) {
        size_t nlextents = jemalloc_arenas_nlextents();
        for (j = 0; j < nlextents; j++) {

            len = 32; //检测返回的结果是不是 8 
        
            mib[2] = i;
            mib[4] = j;
        
            je_mallctlbymib(mib, miblen, &value, &len, NULL, 0);
            
            /* Do something... */
            if(value!=0)
            printf("stats.arenas.%d.lextents.%d.nrequests = %d ,(%d)\n", i, j, value, len);
        }
    }
}


void jemalloc_test()
{
    char *str1 = je_malloc(1234);
    int i;
    for(i=0;i<1024;i++) {
        str1 = je_mallocx(1024, MALLOCX_ARENA(2));
    }

    unsigned arena1, arena2;
	size_t sz = sizeof(unsigned);

	je_mallctl("arenas.create", &arena1, &sz, NULL, 0);
    printf("arenas.create arena1 = %d\n", arena1);
    
	void *huge = je_mallocx(1024, MALLOCX_ARENA(arena1));
    
}


int main()
{
    unsigned narenas;
    jemalloc_test();
    jemalloc_stats_arenas_i_lextents_j_nrequests();
    
}





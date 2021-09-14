#include <stdio.h>
#include <omp.h>




void main(int argc, char *argv[])
{

        int sum = 0, i;

        omp_set_num_threads(5);

        #pragma omp parallel for reduction(+:sum)
        for ( i = 0; i < 5; i ++ ) {

                sum += i;

                printf("sum = %d\n",sum);
        }
        printf("sum = %d\n",sum);
        


}

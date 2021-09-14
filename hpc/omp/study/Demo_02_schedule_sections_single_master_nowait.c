#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(){


        int i, sum = 0;
        #pragma omp parallel for reduction(+:sum)
        for ( i = 0; i < 5 ; i ++) {

                sum += i;
                printf("i = %d, sum = %d\n",i,sum);
          }
        printf("sum = %d\n",sum);

        printf("##################################################\n");


        //#pragma omp parallel for schedule(guided, 3)
        //#pragma omp parallel for schedule(static, 3)
        #pragma omp parallel for schedule(dynamic, 3)
        for (i=0; i<12; i++){

                printf("i = %d\n",i);
          }


        printf("##################################################\n");


        #pragma omp parallel sections
          {
                #pragma omp section
                printf("section:1, id = %d/%d\n", omp_get_thread_num(),omp_get_num_threads());
              
          }

        printf("##################################################\n");



        #pragma omp parallel  
          {

                #pragma omp single
                printf("1\n");

                #pragma omp master
                printf("2\n");

                #pragma omp single nowait
                printf("3\n");

                #pragma omp single nowait
                printf("4\n");
          }

        printf("##################################################\n");


        #pragma omp parallel
          {
                #pragma omp for nowait
                for(i=0;i<5;i++){
                        
                        //printf("-%d-",i);
                    }
                //printf("-%d-\n",i);
          }















}

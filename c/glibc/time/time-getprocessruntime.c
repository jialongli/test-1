/* Copyright (c) Colorado School of Mines, CST.*/
/* All rights reserved.                       */

#include <time.h>
#include <stdio.h>
#include <string.h>

struct timerun {
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_day;
    int tm_year;
};


static time_t proc_start_time;
static time_t proc_curr_time;


//printf("%lf\n", difftime(12.888888883333, 12.4421));
static  __attribute__((constructor(101))) void __proctime_start()
{
    proc_start_time = time(NULL);
}


time_t get_curr_time()
{
    proc_curr_time = time(NULL);
    return proc_curr_time;
}

time_t get_run_time()
{
    return difftime(get_curr_time(), proc_start_time);
}

void time_to_timerun(time_t time, struct timerun *tr)
{
    tr->tm_sec = time%60;
    tr->tm_min = (time/60)%60;
    tr->tm_hour = (time/3600)%24;
    tr->tm_day = (time/(3600*24))%365;
    tr->tm_year = (time/(3600*24*365));

}

int get_self_run_time(struct timerun *tr)
{
    memset(tr, 0 ,sizeof(struct timerun));

    time_t time = get_run_time();
    time_to_timerun(time, tr);    
}

int main()
{
	struct timerun tr;
    time_t time = 0;
    while(1){
        sleep(1);
//        get_self_run_time(&tr);
        time += 60*60*24*365 + 60*60*25 + 60 + 1;
        time_to_timerun(time, &tr);
	    printf("Year %d, Day %d, Hour %d, Mins %d, Sec %d\n", tr.tm_year, tr.tm_day, tr.tm_hour, tr.tm_min, tr.tm_sec);
    }


	return 0;
}




#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

//print "telemetry-brige up" at start
//then at every second print a line with a counter

int main(void) {

    unsigned counter = 0;
    struct timespec next;
    const long period_s = 1L;

    //get current time
    clock_gettime(CLOCK_MONOTONIC, &next);

    setvbuf(stdout, NULL, _IOLBF, 0); //use NULL for buff
    
    printf("telemetry-bridge up\n");

    while(1) {

        next.tv_sec += period_s;

        printf("tick %u\n", counter);
        counter++;

        while(clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, NULL) == EINTR){}
        

    }


    return 0;
}
#include <systemd/sd-daemon.h>
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

    //tell systemd the daemon is ready
    int ret = sd_notify(0, "READY=1"); //ret > 0 == message sent
                                       //ret = 0 == no $NOTIFY_SOCKET
                                       //ret < 0 == error

    if(ret == 0){

        fprintf(stderr, "NOTIFY_SOCKET not set\n");

    } else if (ret < 0)
    {
        fprintf(stderr, "sd_notify(READY=1) failed: %s\n", strerror(-ret));
    }
    

    while(1) {

        next.tv_sec += period_s;

        printf("tick %u\n", counter);
        counter++;

        while(clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, NULL) == EINTR){}
        

    }


    return 0;
}
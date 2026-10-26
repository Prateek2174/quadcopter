#include <systemd/sd-daemon.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <string.h>

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

    //start watchdog code here
    //set watchdog interval then enable it

    uint64_t wd_usec = 0;
    int wd = sd_watchdog_enabled(0, &wd_usec);

    if (wd > 0) { //returns >0 if enabled

        printf("Watchdog is enabled, timeout %llu ms\n", (unsigned long long)(wd_usec / 1000));

    } else if (wd == 0) { //watchdog isnt expected/isnt enabled

        fprintf(stderr, "Watchdog NOT ENABLED\n");

    } else {

        fprintf(stderr, "sd_watchdog_enabled failed: %s\n", strerror(-wd));

    }

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

        //signal the watchdog so it resets its counter

        int wd_signal = sd_notify(0, "WATCHDOG=1");

        //check to see if the watchdog signal was sent

        if(wd_signal < 0){

            fprintf(stderr, "sd_notify(WATCHDOG=1) failed: %s\n", strerror(-wd_signal));

        }
        
        while(clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, NULL) == EINTR){}
        

    }


    return 0;
}
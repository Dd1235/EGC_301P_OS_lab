#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

// 53c
// itimer_prof
// counts more user mode and kernel mode cpu time
// includes time spent making system calls

void handler(int signum)
{
    printf("ITIMER_PROF triggered: SIGPROF\n");
    exit(0);
}

int main()
{
    struct itimerval timer;

    // Signal handler
    signal(SIGPROF, handler);

    // Set timer: 10s + 10us of **user CPU time**
    timer.it_value.tv_sec = 2;
    timer.it_value.tv_usec = 10;

    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;

    setitimer(ITIMER_PROF, &timer, NULL);

    printf("Consuming CPU until ITIMER_PROF expires...\n");
    // Busy loop to consume user CPU time
    while (1)
        ;
    return 0;
}
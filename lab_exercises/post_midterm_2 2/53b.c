#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

// 53b
// interval timer but using ITIMER_VIRTUAL
// until real time, it counts user mode cpu time
// Add exit call to terminate the program after handler is triggered

void handler(int signum)
{
    printf("ITIMER_VIRTUAL triggered: SIGVTALRM\n");
    exit(0);
}

int main()
{
    struct itimerval timer;

    // Signal handler
    signal(SIGVTALRM, handler);

    // Set timer: 10s + 10us of **user CPU time**
    timer.it_value.tv_sec = 2;
    timer.it_value.tv_usec = 10;

    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;

    setitimer(ITIMER_VIRTUAL, &timer, NULL);

    printf("Consuming CPU until ITIMER_VIRTUAL expires...\n");
    // Busy loop to consume user CPU time
    while (1)
        ;
    return 0;
}

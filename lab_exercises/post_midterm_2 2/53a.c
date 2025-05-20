#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

// 53 a, using ITIMER_REAL: decrements in real time, sends SIGALRM
// when timer expires kernel sends a signal tot eh process
// catches sginal using the signal() signal handler

void handler(int signum)
{
    printf("ITIMER_REAL triggered: SIGALRM\n");
}

int main()
{
    struct itimerval timer;

    // signal handler
    if (signal(SIGALRM, handler) == SIG_ERR)
    {
        perror("signal");
        exit(EXIT_FAILURE);
    }

    // set timer: 10 seconds + 10 microseconds
    timer.it_value.tv_sec = 10;
    timer.it_value.tv_usec = 10;

    // No repeating timer
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;

    if (setitimer(ITIMER_REAL, &timer, NULL) == -1)
    {
        perror("setitimer");
        exit(EXIT_FAILURE);
    }

    // wait
    printf("Waiting for ITIMER_REAL to expire...\n");
    pause(); // system call that suspends the process until a signal is received
    return 0;
}
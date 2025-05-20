// sigvtalrm using setitimer sys call
// fine grain timer
// sets a timer that decrements wall clock time
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
void handler(int s)
{
    if (s == SIGVTALRM)
    {
        printf("CAPTURED SIGVTALRM\n");
        exit(0);
    }
}

int main()
{
    if (signal(SIGVTALRM, &handler) == SIG_ERR)
    {
        perror("No Virtual Alarm");
        return 1;
    }
    struct itimerval t;
    t.it_interval.tv_sec = 2;
    t.it_interval.tv_usec = 0;
    t.it_value.tv_sec = 6;
    t.it_value.tv_usec = 0;
    setitimer(ITIMER_VIRTUAL, &t, NULL);
    while (1)
        ;
    return 0;
}

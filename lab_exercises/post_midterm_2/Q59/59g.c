// sigprof using setitimer sys call
// sent when profiling timer set using setimer() expires
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
void handler(int s)
{
    if (s == SIGPROF)
    {
        printf("CAPTURED SIGPROF\n");
        exit(0);
    }
}

int main()
{
    if (signal(SIGPROF, &handler) == SIG_ERR)
    {
        perror("No Alarm");
        return 1;
    }
    struct itimerval t;
    t.it_interval.tv_sec = 2;
    t.it_interval.tv_usec = 0;
    t.it_value.tv_sec = 6;
    t.it_value.tv_usec = 0;
    setitimer(ITIMER_PROF, &t, NULL);
    while (1)
        ;
    return 0;
}

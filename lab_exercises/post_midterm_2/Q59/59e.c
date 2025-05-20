// capture sigalrm using setitimer system call
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
void handler(int s)
{
    if (s == SIGALRM)
    {
        printf("CAPTURED SIGALRM\n");
        exit(0);
    }
}

int main()
{
    if (signal(SIGALRM, &handler) == SIG_ERR)
    {
        perror("No Alarm");
        return 1;
    }
    struct itimerval t;
    t.it_interval.tv_sec = 2;
    t.it_interval.tv_usec = 0;
    t.it_value.tv_sec = 6;
    t.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &t, NULL);
    while (1)
        ;
    return 0;
}

// capture sigalrm using alarm sys call
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

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
        printf("No Alarm");
        return 1;
    }
    alarm(3);
    while (1)
        ;
    return 0;
}

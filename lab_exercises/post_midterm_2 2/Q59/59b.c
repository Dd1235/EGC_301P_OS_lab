// capture sigint

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void handler(int s)
{
    if (s == SIGINT)
    {
        printf("CAPTURED SIGINT\n");
        exit(0);
    }
}

int main()
{
    if (signal(SIGINT, &handler) == SIG_ERR)
    {
        perror("Error");
        return 1;
    }
    while (1)
        ;
    return 0;
}

// capture sigfpe

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void handler(int s)
{
    if (s == SIGFPE)
    {
        printf("CAPTURED SIGFPE\n");
        exit(0);
    }
}

int main()
{
    if (signal(SIGFPE, &handler) == SIG_ERR)
    {
        perror("Floating Point Error");
        return 1;
    }
    float f = 1 / 0;
    return 0;
}

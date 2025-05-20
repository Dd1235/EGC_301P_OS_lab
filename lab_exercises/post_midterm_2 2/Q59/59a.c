// catch sigsegv

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void handler(int s)
{
    if (s == SIGSEGV)
    {
        printf("CAPTURED SIGSEGV\n");
        exit(0);
    }
}

int main()
{
    if (signal(SIGSEGV, &handler) == SIG_ERR)
    {
        perror("Invalid Memory");
        return 1;
    }
    int *p = NULL;
    printf("%d\n", *p);
    return 0;
}

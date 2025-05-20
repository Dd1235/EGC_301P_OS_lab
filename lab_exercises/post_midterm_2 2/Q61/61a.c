// catch sigsegv using sigaction
// sigaction() is more reliable than signal()
// you can set more flags iwht it
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
    struct sigaction new_action;
    new_action.sa_handler = &handler;
    if (sigaction(SIGSEGV, &new_action, NULL) != 0)
    {
        perror("Invalid Memory");
        return 1;
    }
    int *p = NULL;
    printf("%d\n", *p);
    return 0;
}

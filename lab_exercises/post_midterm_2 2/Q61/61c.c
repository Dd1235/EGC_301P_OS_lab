// sigfpe using sigaction
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

int main(void)
{
    struct sigaction new_action;
    new_action.sa_handler = &handler;
    if (sigaction(SIGFPE, &new_action, NULL) != 0)
    {
        perror("Floating Point Error");
        return 1;
    }
    float f = 1 / 0;
    return 0;
}

// catch sigint using sigaction
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
    struct sigaction new_action;
    new_action.sa_handler = &handler;
    if (sigaction(SIGINT, &new_action, NULL) != 0)
    {
        perror("Error");
        return 1;
    }
    while (1)
        ;
    return 0;
}

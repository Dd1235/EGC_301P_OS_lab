// signal handling wiht sigaction, ignore a sigint and reset to the default action using the sigaction syscall
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
int main()
{
        struct sigaction new_action;
        new_action.sa_handler = SIG_DFL;
        signal(SIGINT, SIG_IGN);
        for (int i = 0; i < 10; i++)
        {
                printf("%d\n", i);
                sleep(1);
        }
        sigaction(SIGINT, &new_action, NULL);
        printf("SIGINT Reset.\n");
        for (int i = 0; i < 10; i++)
        {
                printf("%d\n", i);
                sleep(1);
        }
}

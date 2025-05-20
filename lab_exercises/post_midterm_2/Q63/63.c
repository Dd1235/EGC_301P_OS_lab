// create an orphan process, sigkill signal from child to parent
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
int main()
{
        int f = fork();
        if (f == 0)
        {
                // child
                int s = getppid();
                kill(s, SIGKILL);
                sleep(5);
                printf("Child ID : %d & PPID : %d\n", getpid(), getppid());
        }
        else
        {
                printf("Parent ID : %d\n", getpid());
        }
}

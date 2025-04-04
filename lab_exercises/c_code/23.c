#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    pid_t child_pid = fork();

    if (child_pid < 0)
    {
        perror("Fork failed");
        exit(1);
    }

    if (child_pid == 0)
    {
        // somehow after parent exits it is terminating, and then getting stuck after child's prints are done
        printf("[Child] My parent PID (before sleep): %d\n", getppid());
        fflush(stdout);

        sleep(5);

        printf("[Child] After sleep, my new parent PID (should be 1): %d\n", getppid());
        printf("[Child] I have become an orphan and was reparented to init/systemd.\n");
        fflush(stdout);

        exit(0);
    }
    else
    {

        printf("[Parent] I am the parent. My PID: %d\n", getpid());
        sleep(2); // making parent sleep for parent pid will be printed from child before parent exits
        printf("[Parent] I will not wait for the child and will now exit.\n");
        fflush(stdout);
        exit(EXIT_SUCCESS);
    }
}

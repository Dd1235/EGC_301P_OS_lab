#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

// you will notice that bash/zsh will print the $/% base prompt after parent exits because bash created parent
// so even with 3 seconds of child process running bash will print this off
// please wait for 3 seconds after parent exits to see the prints of the child
// and when the child exits you will not observe the default shell prompt being printed because bash does not track this child terminating.
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

        // if you are not using mac, but Linux, the reparented PID will not be 1
        // 1 if root user systemd
        // you will get systemd of not root user, but the user that is running the program.
        // so not 1, but some otehr number
        printf("[Child] After sleep, my new parent PID (should be 1 in mac, and, the PID of systemd of user running it in Linux): %d\n", getppid());
        printf("[Child] I have become an orphan and was reparented to init/systemd.\n");
        fflush(stdout);

        // exit(0);
        return 0;
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

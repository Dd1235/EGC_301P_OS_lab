
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t child1, child2, child3;

    child1 = fork();
    if (child1 == 0)
    {
        printf("Child 1 (PID: %d) is running.\n", getpid());
        sleep(3);
        printf("Child 1 (PID: %d) exiting.\n", getpid());
        exit(1);
    }

    child2 = fork();
    if (child2 == 0)
    {
        printf("Child 2 (PID: %d) is running.\n", getpid());
        sleep(5);
        printf("Child 2 (PID: %d) exiting.\n", getpid());
        exit(2);
    }

    child3 = fork();
    if (child3 == 0)
    {
        printf("Child 3 (PID: %d) is running.\n", getpid());
        sleep(7);
        printf("Child 3 (PID: %d) exiting.\n", getpid());
        exit(3);
    }

    // Parent process waits for a specific child (e.g., child2)
    int status;
    pid_t waited_pid = waitpid(child2, &status, 0);

    if (WIFEXITED(status))
    {
        printf("Parent: Child with PID %d exited with status %d.\n", waited_pid, WEXITSTATUS(status));
    }

    while (wait(NULL) > 0)
        ;

    printf("Parent: All child processes finished. Exiting.\n");

    return 0;
}

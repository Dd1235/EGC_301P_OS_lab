#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

// fork and show parent child pid

int main()
{
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Fork failed");
        return EXIT_FAILURE;
    }
    else if (pid == 0)
    { // Child process
        printf("Child Process: PID = %d, Parent PID = %d\n", getpid(), getppid());
    }
    else
    { // Parent process
        printf("Parent Process: PID = %d, Child PID = %d\n", getpid(), pid);
    }

    return EXIT_SUCCESS;
}

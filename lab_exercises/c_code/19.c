
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

// to initiate running sleeping stopped programs

int main()
{
    pid_t child_pid = fork();
    clock_t start_time = clock();

    if (child_pid < 0)
    {
        perror("Fork failed");
        return EXIT_FAILURE;
    }
    else if (child_pid == 0)
    {
        printf("Child process started.\n");
        clock_t current_time;
        int cycle_count = 0;

        do
        {
            current_time = clock();
            cycle_count++;
        } while ((double)(current_time - start_time) / CLOCKS_PER_SEC <= 10);

        printf("Total Cycles: %d\n", cycle_count);
        printf("Child process going to sleep.\n");
        sleep(10);
        printf("Child process stopping.\n");
        raise(SIGSTOP);
    }
    else
    {
        waitpid(child_pid, NULL, WUNTRACED);
        printf("Parent detected child stop, resuming after 10 seconds.\n");
        sleep(10);
        kill(child_pid, SIGCONT);
        wait(NULL);
        printf("Child process has terminated.\n");
    }
    return EXIT_SUCCESS;
}

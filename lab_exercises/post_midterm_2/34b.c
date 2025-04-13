#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// usign dup2 do ls -l -wc

int main()
{
    int pipefd[2];
    pipe(pipefd);

    if (fork() == 0)
    {
        dup2(pipefd[1], STDOUT_FILENO); // redirect stdout to pipefd[1], so writing to fd 1 will now write to write end of pipe
        close(pipefd[0]);
        close(pipefd[1]);

        execlp("ls", "ls", "-l", NULL);
        perror("execlp ls");
        exit(1);
    }

    if (fork() == 0)
    {
        // Child 2 - wc
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[1]);
        close(pipefd[0]);

        execlp("wc", "wc", NULL);
        perror("execlp wc");
        exit(1);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    wait(NULL);
    wait(NULL);

    return 0;
}

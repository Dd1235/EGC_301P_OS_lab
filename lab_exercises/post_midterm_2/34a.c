// program for ls -l|wc using dup, dup2, fcntl

// using dup
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int pipefd[2];
    pipe(pipefd);

    if (fork() == 0)
    {
        // Child 1 - ls -l
        close(1);         // Close stdout,
        dup(pipefd[1]);   // Duplicate pipefd[1] to the lowest available fd that is fd 1, so writing to fd1 write to the write end of the pipe instead
        close(pipefd[0]); // Not needed, the read end of the pipe
        close(pipefd[1]);

        execlp("ls", "ls", "-l", NULL);
        perror("execlp ls");
        exit(1);
    }

    if (fork() == 0)
    {
        // Child 2 - wc
        close(0);         // Close stdin
        dup(pipefd[0]);   // now reading from fd0 calls read to pipefd[0] so read from the read end of the pipe instead of stdin
        close(pipefd[1]); // Not needed
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

// using fcntl ls -l -wc

int main()
{
    int pipefd[2];
    pipe(pipefd);

    if (fork() == 0)
    {

        fcntl(pipefd[1], F_DUPFD, STDOUT_FILENO); // Duplicate to stdout
        close(pipefd[0]);
        close(pipefd[1]);

        execlp("ls", "ls", "-l", NULL);
        perror("execlp ls");
        exit(1);
    }

    if (fork() == 0)
    {
        // Child 2 - wc
        fcntl(pipefd[0], F_DUPFD, STDIN_FILENO); // Duplicate to stdin
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

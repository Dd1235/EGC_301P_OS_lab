#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h> // for wait

int main()
{
    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return 1;
    }
    // 0  read 1 write
    pid_t pid;
    char message[] = "Hello this is the parent!";
    char buffer[100];

    pid = fork();
    if (pid > 0)
    {
        close(pipefd[0]); // close the read end in the parent as parent will write
        write(pipefd[1], message, strlen(message) + 1);
        close(pipefd[1]);
        wait(NULL); // wait for child to finish
    }
    else
    {
        close(pipefd[1]); // child closes the write end
        read(pipefd[0], buffer, sizeof(buffer));
        printf("Child with pid %d received: %s\n", getpid(), buffer);
        close(pipefd[0]); // Close read end
    }
    return 0;
}
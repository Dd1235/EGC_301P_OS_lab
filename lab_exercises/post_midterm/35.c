#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// ls -l | grep ^d | wc
// find the total number of directories in the pwd using only dup2

int main()
{
    int pipe1[2]; // between ls -l | grep
    int pipe2[2]; // between gred and wc
    pipe(pipe1);
    pipe(pipe2);
    printf("allocated file descriptors: %d %d %d %d\n", pipe1[0], pipe1[1], pipe2[0], pipe2[1]);

    pid_t pid1 = fork();

    if (pid1 == 0)
    {
        // child 1: ls -l
        dup2(pipe1[1], STDOUT_FILENO); // new, old
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);

        execlp("ls", "ls", "-l", NULL); // this will write some output to fd 1 ie stdout, so use dup2 to redirect this to pipe
        perror("execlp");
        exit(1); // we exit here so child one will not exit following code
    }

    pid_t pid2 = fork();

    if (pid2 == 0)
    {
        dup2(pipe1[0], 0); // write output of ls -l
        dup2(pipe2[1], 1); // write outputs to wc
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);

        execlp("grep", "grep", "^d", NULL);
        perror("execlp grep");
        exit(1);
    }
    pid_t pid3 = fork();

    if (pid3 == 0)
    {
        dup2(pipe2[0], STDIN_FILENO);
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);

        execlp("wc", "wc", "-l", NULL);
        perror("execlp wc");
        exit(1);
    }

    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);

    wait(NULL);
    wait(NULL);
    wait(NULL);
    return 0;
}
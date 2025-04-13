// two way communication between child and parent
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
    int pipe1[2]; // Parent to Child
    int pipe2[2]; // Child to Parent

    char parent_msg[] = "Hello from Parent!";
    char child_msg[] = "Hello from Child!";
    char buffer[100];

    if (pipe(pipe1) == -1)
    {
        perror("pipe1");
        exit(1);
    }

    if (pipe(pipe2) == -1)
    {
        perror("pipe2");
        exit(1);
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        exit(1);
    }

    if (pid > 0)
    {
        // Parent process
        close(pipe1[0]); // Close read end of pipe1, as parent uses 1 to communicate to child by writing
        close(pipe2[1]); // Close write end of pipe2

        // Send message to child
        write(pipe1[1], parent_msg, strlen(parent_msg) + 1);

        // Read message from child
        read(pipe2[0], buffer, sizeof(buffer));
        printf("Parent received: %s\n", buffer);

        close(pipe1[1]);
        close(pipe2[0]);

        wait(NULL); // Wait for child
    }
    else
    {
        // Child process
        close(pipe1[1]); // Close write end of pipe1
        close(pipe2[0]); // Close read end of pipe2

        // Read message from parent
        read(pipe1[0], buffer, sizeof(buffer));
        printf("Child received: %s\n", buffer);

        // Send message to parent
        write(pipe2[1], child_msg, strlen(child_msg) + 1);

        close(pipe1[0]);
        close(pipe2[1]);
    }

    return 0;
}

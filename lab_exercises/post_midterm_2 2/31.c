// create a pipe, write to the pipe, read and display
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main()
{
    char write_msg[] = "Hello, this is a sentence being written to the pipe";
    char read_msg[100]; // buffer

    int pipefd[2];

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return 1;
        // pipe creation unsuccessful
    }
    // pipefd[0] is fd of read end, 1 the write end
    write(pipefd[1], write_msg, strlen(write_msg) + 1);
    read(pipefd[0], read_msg, sizeof(read_msg));
    printf("Received from pipe: %s\n", read_msg);

    // like any other file descriptor allocation, they get fd sequentially, new one gets the next free one
    printf("Also fyi read %d write %d\n", pipefd[0], pipefd[1]);
    int fd = open("sample.txt", O_CREAT | O_WRONLY, 0644);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    printf("File descriptor: %d\n", fd);
    close(fd);
    return 0;
}
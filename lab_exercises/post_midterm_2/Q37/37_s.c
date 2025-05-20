// fifo one way sender
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#define FIFO_PATH "myfifo"

int main()
{
    int fd;
    char buf[100] = {0};

    if (mkfifo(FIFO_PATH, 0666) == -1)
    {
        perror("mkfifo");
        exit(1);
    }

    fd = open(FIFO_PATH, O_WRONLY);
    printf("Enter Message to be sent :");
    fgets(buf, sizeof(buf), stdin);
    buf[strlen(buf) - 1] = '\0';
    write(fd, buf, strlen(buf));
    close(fd);
    unlink(FIFO_PATH);
    return 0;
}
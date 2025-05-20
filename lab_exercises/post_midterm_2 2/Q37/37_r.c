// fifo one way reeceiver

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define FIFO_PATH "myfifo"

int main()
{
    int fd;
    char buf[100];

    fd = open(FIFO_PATH, O_RDONLY);
    read(fd, buf, sizeof(buf));
    printf("Message Received : %s\n", buf);
    return 0;
}
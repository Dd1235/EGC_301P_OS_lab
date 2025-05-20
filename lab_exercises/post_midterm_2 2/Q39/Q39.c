// 39 wait for data to be writtent o fifo within 10 seconds utilizing select system call
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>

#define FIFO_FILE "myfifo"

int main()
{
    int fd;
    fd_set rfds;
    struct timeval tv;
    int retval;
    char buff[256];
    fd = open(FIFO_FILE, O_RDONLY);
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);
    tv.tv_sec = 10;
    tv.tv_usec = 0;
    retval = select(fd + 1, &rfds, NULL, NULL, &tv);

    if (retval == -1)
    {
        perror("select()");
    }

    else if (retval)
    {
        read(fd, buff, sizeof(buff));
        printf("Received: %s\n", buff);
    }

    else
        printf("No data in 10s.\n");
    close(fd);
    unlink(FIFO_FILE);
    return 0;
}
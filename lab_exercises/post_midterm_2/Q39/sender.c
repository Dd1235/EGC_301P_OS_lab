// write to fifo

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_FILE "myfifo"
#define BUFFER_SIZE 256

int main()
{
    int fd;
    char buffer[BUFFER_SIZE];

    // Open the FIFO file in write-only mode
    fd = open(FIFO_FILE, O_WRONLY);

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    printf("Enter Message : ");
    fgets(buffer, BUFFER_SIZE, stdin);
    if (write(fd, buffer, strlen(buffer)) == -1)
    {
        perror("write");
        close(fd);
        return 1;
    }
    close(fd);
    return 0;
}
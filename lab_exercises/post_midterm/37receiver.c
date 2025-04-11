// receiver.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main()
{
    const char *fifo_path = "myfifo37";

    // Open FIFO for reading
    int fd = open(fifo_path, O_RDONLY);
    printf("The file descriptor on opening the fifo is %d\n", fd);
    if (fd == -1)
    {
        perror("open for read");
        exit(1);
    }

    char buffer[100];
    read(fd, buffer, sizeof(buffer)); // read just the size of the buffer
    printf("Received: %s\n", buffer);
    close(fd);

    return 0;
}

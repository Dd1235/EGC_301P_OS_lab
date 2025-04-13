// Write two programs to enable communication through FIFO using one-way communication.
// sender.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main()
{
    const char *fifo_path = "myfifo37";

    // Create FIFO if it doesn't exist
    if (mkfifo(fifo_path, 0666) == -1)
    {
        perror("mkfifo (ignored if already exists)");
    }

    int fd = open(fifo_path, O_WRONLY); // open FIFO for writing
    printf("The file descriptor on opening the fifo is %d\n", fd);
    if (fd == -1)
    {
        perror("open for write");
        exit(1);
    }

    char message[] = "Hello from the writer process!";
    write(fd, message, sizeof(message));
    close(fd);

    return 0;
}

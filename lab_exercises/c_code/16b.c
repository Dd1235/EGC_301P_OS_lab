
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int file = open(argv[1], O_RDWR);
    if (file == -1)
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    struct flock fileLock;
    fileLock.l_type = F_RDLCK; // Read Lock
    fileLock.l_whence = SEEK_SET;
    fileLock.l_start = 0;
    fileLock.l_len = 0;
    fileLock.l_pid = getpid();

    if (fcntl(file, F_SETLKW, &fileLock) != -1)
    {
        printf("Process %d acquired the lock.\n", fileLock.l_pid);

        // Instead of waiting indefinitely, wait for 5 seconds
        sleep(5);

        // Release the lock automatically after timeout
        fileLock.l_type = F_UNLCK;
        fcntl(file, F_SETLKW, &fileLock);
        printf("Lock released after timeout.\n");
    }
    else
    {
        perror("Locking failed");
    }

    close(file);
    return EXIT_SUCCESS;
}

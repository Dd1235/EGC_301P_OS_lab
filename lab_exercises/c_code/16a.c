
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];

    int file = open(filename, O_RDWR);
    if (file == -1)
    {
        perror("Error opening file");
        return 1;
    }

    struct flock lock = {0};
    lock.l_type = F_WRLCK; // Write lock
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0; // Lock the whole file
    lock.l_pid = getpid();

    if (fcntl(file, F_SETLKW, &lock) == -1)
    {
        perror("Locking failed");
        close(file);
        return 1;
    }

    printf("Process %d has locked the file.\n", getpid());

    // Instead of waiting for user input, wait for 5 seconds
    sleep(5);

    // Unlock the file
    lock.l_type = F_UNLCK;
    if (fcntl(file, F_SETLKW, &lock) == -1)
    {
        perror("Unlocking failed");
    }
    else
    {
        printf("File unlocked automatically after timeout.\n");
    }

    close(file);
    return 0;
}

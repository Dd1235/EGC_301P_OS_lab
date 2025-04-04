#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    int fd = open("ticket.txt", O_RDWR);
    if (fd == -1)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    struct flock lock;
    lock.l_type = F_WRLCK; // Write lock
    lock.l_whence = SEEK_SET;
    lock.l_start = 0; // Lock entire file
    lock.l_len = 0;

    printf("Trying to acquire write lock...\n");
    if (fcntl(fd, F_SETLKW, &lock) == -1)
    {
        perror("Error acquiring lock");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("Write lock acquired.\n");

    lseek(fd, 0, SEEK_SET);
    char buffer[32];
    int bytes = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes < 0)
    {
        perror("Error reading file");
        close(fd);
        exit(EXIT_FAILURE);
    }
    buffer[bytes] = '\0';

    int ticket_number;
    sscanf(buffer, "%d", &ticket_number);
    ticket_number++;

    lseek(fd, 0, SEEK_SET);
    dprintf(fd, "%d\n", ticket_number);
    ftruncate(fd, sizeof(int) + 1); // Ensure old content beyond this is removed

    printf("Your reserved ticket number is: %d\n", ticket_number);

    // Unlock
    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &lock) == -1)
    {
        perror("Error releasing lock");
    }

    close(fd);
    return 0;
}

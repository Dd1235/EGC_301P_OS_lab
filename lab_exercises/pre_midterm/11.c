#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

// open a file, duplicate the fd, append to it using both, and check if updation is correct

int main()
{
    int fd, fd_dup1, fd_dup2, fd_dup3;
    ssize_t bytes_written;

    fd = open("sample_11.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // use dup to duplicate the fd, so duplicate to lowest unused fd
    fd_dup1 = dup(fd);
    if (fd_dup1 < 0)
    {
        perror("Error using dup");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // assign the fd to fd 100
    fd_dup2 = dup2(fd, 100);
    if (fd_dup2 < 0)
    {
        perror("Error using dup2");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // int fcntl(int oldfd, int cmd, int arg);

    fd_dup3 = fcntl(fd, F_DUPFD, 200); // duplicate into first available fd >= 200
    if (fd_dup3 < 0)
    {
        perror("Error using fcntl");
        close(fd);
        exit(EXIT_FAILURE);
    }

    write(fd, "Original FD\n", 12);
    write(fd_dup1, "dup FD (dup)\n", 13);
    write(fd_dup2, "dup FD (dup2)\n", 14);
    write(fd_dup3, "dup FD (fcntl)\n", 15);

    close(fd);
    close(fd_dup1);
    close(fd_dup2);
    close(fd_dup3);

    printf("All writes done. Check 'sample_11.txt'.\n");

    printf("\nReading contents of file:\n--------------------------\n");

    int fd_read = open("sample_11.txt", O_RDONLY);
    if (fd_read < 0)
    {
        perror("Error reopening file for reading");
        exit(EXIT_FAILURE);
    }

    char buffer[256]; // buffer of 256 bytes
    ssize_t bytes_read;

    while ((bytes_read = read(fd_read, buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[bytes_read] = '\0';
        printf("%s", buffer);
    }

    if (bytes_read < 0)
    {
        perror("Error reading file");
    }

    close(fd_read);
    printf("\n--------------------------\n");

    return 0;
}

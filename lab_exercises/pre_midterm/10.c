// open in read write mode, write 10 bytes, move fp by 10 bytes using seek, write 10 more bytes
// check lseek return value, examine empty spaces using od

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int fd;
    // signed size type, signed version of size_t , so -1 on error
    ssize_t bytes_written;
    // off_t -> data type to represent file sizes and file offsets
    off_t offset;
    fd = open("sample_10.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    bytes_written = write(fd, "ABCDEFGHIJ", 10);
    if (bytes_written != 10)
    {
        perror("Error writing first 10 bytes");
        close(fd);
        exit(EXIT_FAILURE);
    }

    offset = lseek(fd, 10, SEEK_CUR);
    // if lseek returned -1
    if (offset == (off_t)-1)
    {
        perror("Error using lseek");
        close(fd);
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("lseek returned new offset: %lld\n", offset);
    }

    bytes_written = write(fd, "KLMNOPQRST", 10);
    if (bytes_written != 10)
    {
        perror("Error writing second 10 bytes");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);
    return 0;
}
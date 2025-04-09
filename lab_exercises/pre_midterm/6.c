#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024 // number of bytes to read/write at a time

int main()
{
    // we use file descriptor numbers 0 to access stdin file and 1 to access stdout file
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    ssize_t bytes_written;

    bytes_read = read(0, buffer, BUFFER_SIZE);
    if (bytes_read == -1)
    {
        perror("read");
        return 1;
    }
    buffer[bytes_read] = '\0';

    char message[] = "successfully read in the bytes from stdin, now writing to it\n";
    bytes_written = write(1, message, strlen(message));
    if (bytes_written == -1)
    {
        perror("write");
        return 1;
    }

    bytes_written = write(1, buffer, bytes_read);
    if (bytes_written == -1)
    {
        perror("write");
        return 1;
    }
    char message2[200];
    sprintf(message2, "bytes read in vs bytes written out comparison(including one extra for null terminator:  %ld vs %ld\n", bytes_read, bytes_written);
    bytes_written = write(1, message2, strlen(message2));
    if (bytes_written == -1)
    {
        perror("write");
        return 1;
    }
}
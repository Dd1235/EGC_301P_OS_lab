#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUFFER_SIZE 4096

void copy_file(const char *source, const char *destination)
{
    int src_fd, dest_fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;
    struct stat file_stat;

    // opening source in read only mode
    src_fd = open(source, O_RDONLY);
    if (src_fd == -1)
    {
        perror("Error opening source file");
        exit(EXIT_FAILURE);
    }

    // get status of source file to get file permissions
    if (fstat(src_fd, &file_stat) == -1)
    {
        perror("Error getting file status");
        close(src_fd);
        exit(EXIT_FAILURE);
    }

    // open in write only mode, create if not exists, truncate if exists, with same permissions as source
    dest_fd = open(destination, O_WRONLY | O_CREAT | O_TRUNC, file_stat.st_mode);
    if (dest_fd == -1)
    {
        perror("Error opening destination file");
        close(src_fd);
        exit(EXIT_FAILURE);
    }

    // 'chunk size' is chosen to be 4096, keep reading while you can from src_fd into buffer with chunk size, and write to dstination, bytes written should be same as bytes_read
    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0)
    {
        bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written != bytes_read)
        {
            perror("Error writing to destination file");
            close(src_fd);
            close(dest_fd);
            exit(EXIT_FAILURE);
        }
    }

    if (bytes_read == -1)
    {
        perror("Error reading from source file");
    }

    close(src_fd);
    close(dest_fd);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    copy_file(argv[1], argv[2]);
    printf("File copied successfully\n");

    return EXIT_SUCCESS;
}

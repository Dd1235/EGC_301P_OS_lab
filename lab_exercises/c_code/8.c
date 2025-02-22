#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

void read_file(const char *filename)
{
    int fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    int i = 0;

    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    while ((bytes_read = read(fd, buffer, BUFFER_SIZE - 1)) > 0)
    {
        buffer[bytes_read] = '\0';
        for (i = 0; i < bytes_read; i++)
        {
            putchar(buffer[i]); // using putchar since printf might not print unless new line if encountered since buffered output, as thought in theory class
            if (buffer[i] == '\n')
            {
                fflush(stdout);
            }
        }
    }

    if (bytes_read == -1)
    {
        perror("Error reading file");
    }

    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    read_file(argv[1]);

    return EXIT_SUCCESS;
}

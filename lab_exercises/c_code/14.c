// file type indentifier from the command line

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    // we use the struct stat that has all the information
    struct stat file_stat;

    if (argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Use lstat to also detect symbolic links themselves
    if (lstat(argv[1], &file_stat) < 0)
    {
        perror("lstat failed");
        exit(EXIT_FAILURE);
    }

    printf("File: %s\n", argv[1]);
    printf("File type: ");

    if (S_ISREG(file_stat.st_mode))
        printf("Regular file\n");
    else if (S_ISDIR(file_stat.st_mode))
        printf("Directory\n");
    else if (S_ISLNK(file_stat.st_mode))
        printf("Symbolic link\n");
    else if (S_ISCHR(file_stat.st_mode))
        printf("Character device\n");
    else if (S_ISBLK(file_stat.st_mode))
        printf("Block device\n");
    else if (S_ISFIFO(file_stat.st_mode))
        printf("FIFO (named pipe)\n");
    else if (S_ISSOCK(file_stat.st_mode))
        printf("Socket\n");
    else
        printf("Unknown file type\n");

    return 0;
}

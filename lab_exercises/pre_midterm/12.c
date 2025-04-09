// find opening mode of a file using fcntl call
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int fd, flags, access_mode;

    // open some sample file in read write mode,
    fd = open("../sample.txt", O_RDWR | O_CREAT, 0644);
    if (fd < 0)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // use fcntl to get its status
    flags = fcntl(fd, F_GETFL);
    if (flags == -1)
    {
        perror("fcntl failed");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // get the access mode within hte flags, O_APPEND is a different flag and can be checked for too. so non blocking, append etc need separate checks apart from access mode
    access_mode = flags & O_ACCMODE;

    printf("Access mode of the file: ");
    switch (access_mode)
    {
    case O_RDONLY:
        printf("Read Only (O_RDONLY)\n");
        break;
    case O_WRONLY:
        printf("Write Only (O_WRONLY)\n");
        break;
    case O_RDWR:
        printf("Read and Write (O_RDWR)\n");
        break;
    default:
        printf("Unknown mode\n");
    }

    close(fd);
    return 0;
}

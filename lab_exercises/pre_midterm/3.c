#include <stdio.h>
#include <fcntl.h> // creat system call
#include <unistd.h>

int main()
{
    const char *filename = "./created_file_for_3.txt";

    // Create the file with read & write permissions (rw-r--r--)
    // usr -> owner grp -> group oth - others
    // ir read permission
    // iw write permission
    int fd = creat(filename, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    if (fd == -1)
    {
        perror("Error creating file");
        return 1;
    }

    printf("File '%s' created!\n", filename);
    printf("File Descriptor: %d\n", fd);

    close(fd);

    return 0;
}

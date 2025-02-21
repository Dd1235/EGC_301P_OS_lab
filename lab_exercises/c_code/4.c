#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> // define the some of the requests and arguments for use by the functions fcntl() and open()
#include <sys/stat.h>

int main()
{
    // O_EXCL makes sure that if the file already exists, open() will fail
    // so this now gives you error if file already exists, EEXIST because of O_EXCL
    // success when file is created
    int fd = open("../file_4.txt", O_RDWR | O_CREAT | O_EXCL, 0744); // permissions matter when creating a new file
    if (fd == -1)
    {
        perror("Error");
        exit(1);
    }
    else
    {
        printf("File opened successfully, its file descriptor: %d \n", fd);
    }
    close(fd);
    return 0;
}
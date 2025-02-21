#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    int num_files = 5;
    for (int i = 1; i <= num_files; i++)
    {
        char filename[20];
        sprintf(filename, "../file_5_%d.txt", i);
        int fd = open(filename, O_RDWR | O_CREAT | O_EXCL, 0744);
        if (fd == -1)
        {
            perror("Error");
            // exit(1);
        }
        else
        {
            printf("File opened successfully, its file descriptor: %d \n", fd);
        }
    }
    while (1)
    {
        // infinite loop
    }
}
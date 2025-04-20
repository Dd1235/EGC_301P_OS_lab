#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

// generate 5 new files in infinite loop

int main()
{
    int num_files = 5;
    for (int i = 1; i <= num_files; i++)
    {
        char filename[20];
        sprintf(filename, "./file_5_%d.txt", i);
        // if it already exists give error since o_excl
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
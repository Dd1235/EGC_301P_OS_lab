#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
    const char *fifo_path = "../my_fifo";

    // Create the FIFO with read and write permissions (rw-r--r--)
    if (mkfifo(fifo_path, 0664) == -1)
    {
        perror("Error creating FIFO");
        return 1;
    }

    printf("FIFO '%s' created successfully!\n", fifo_path);

    return 0;
}

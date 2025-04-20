#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h> // for mkfifo()
#include <unistd.h>

int main()
{
    const char *fifo_path = "./my_fifo";

    // Create the FIFO with read and write permissions (rw-rw-r--)
    // owner group can read write, others can only read
    if (mkfifo(fifo_path, 0664) == -1)
    {
        perror("Error creating FIFO");
        return 1;
    }

    printf("FIFO '%s' created successfully!\n", fifo_path);

    // unlink(fifo_path);
    // we see with p at the beginning because named pipe

    // ls -l my_fifo

    return 0;
}

// wait for stdin for 10 seconds using select. include proper print statements to verify data availability within the specified time
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

int main()
{
    // this way input won't be considered if you do not press enter

    fd_set read_fds;
    struct timeval timeout; // contains hte seconds and micro seconds
    int retval;

    // Clear the set and add STDIN (fd 0) to the read set
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);

    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    printf("Waiting for input on STDIN for 10 seconds...\n");
    fflush(stdout);

    // select(max_fd + 1, &readfds, NULL, NULL, &timeout)
    retval = select(STDIN_FILENO + 1, &read_fds, NULL, NULL, &timeout);

    if (retval == -1)
    {
        perror("select() error");
        exit(EXIT_FAILURE);
    }
    else if (retval == 0)
    {
        printf("Timeout: No input received within 10 seconds.\n");
    }
    else
    {
        if (FD_ISSET(STDIN_FILENO, &read_fds))
        {
            char buffer[1024];
            ssize_t bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
            if (bytes_read > 0)
            {
                buffer[bytes_read] = '\0'; // Null-terminate
                printf("Input received: %s\n", buffer);
            }
            else
            {
                printf("Read error or no input data.\n");
            }
        }
    }

    return 0;
}

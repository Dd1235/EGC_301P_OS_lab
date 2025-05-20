

#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <sys/resource.h>

// RLIMIT_NOFILE: max number of files a process can have open
// _PC_PIPE_BUF: and the size of a pipe's circular buffer

int main()
{
    struct rlimit lim;                       // store resource limits
    if (getrlimit(RLIMIT_NOFILE, &lim) != 0) // retrieve resource limits
    {
        perror("Getrlimit.\n");
        return 1;
    }
    printf("The maximum number of files a process can have is: %ld\n", lim.rlim_cur);

    int fd[2];
    pipe(fd);
    printf("The size of a circular buffer: %ld\n", fpathconf(fd[0], _PC_PIPE_BUF));
}

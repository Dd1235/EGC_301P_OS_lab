#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

// 54
// getrlimit system call
// use to retrieve the resource limits of the current process

// limits can be soft or hard, soft: using right now, can increase or decrese upto the hard limit
// if the limit comes out to be the maximum number that llu can represent, it means that there is no limit

void print_rlimit(int resource, const char *name)
{
    struct rlimit rlim;

    if (getrlimit(resource, &rlim) == -1)
    {
        perror("getrlimit");
        exit(EXIT_FAILURE);
    }

    printf("%s: soft limit = %llu, hard limit = %llu\n", name, rlim.rlim_cur, rlim.rlim_max);
}

int main()
{
    print_rlimit(RLIMIT_CPU, "CPU time limit (seconds)");
    print_rlimit(RLIMIT_FSIZE, "File size limit (bytes)");
    print_rlimit(RLIMIT_DATA, "Data segment size limit (bytes)");
    print_rlimit(RLIMIT_STACK, "Stack size limit (bytes)");
    print_rlimit(RLIMIT_NOFILE, "Max open file descriptors");

    return 0;
}
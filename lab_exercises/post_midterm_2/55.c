#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>

// 55.c
// use the setrlimit system call to set the resource limits of the current process

void set_limit(int resource, rlim_t soft_limit, rlim_t hard_limit)
{
    struct rlimit rlim;

    // Set the soft and hard limits
    rlim.rlim_cur = soft_limit;
    rlim.rlim_max = hard_limit;

    if (setrlimit(resource, &rlim) == 0)
    {
        printf("Successfully set resource limit.\n");
        printf("Soft Limit: %lu, Hard Limit: %lu\n", rlim.rlim_cur, rlim.rlim_max);
    }
    else
    {
        perror("Failed to set resource limit");
    }
}

int main()
{
    // Set the stack size limit to 8MB (soft limit), and 16MB (hard limit)
    set_limit(RLIMIT_STACK, 8 * 1024 * 1024, 16 * 1024 * 1024);

    // Set the maximum number of open file descriptors to 512 (soft limit), and 1024 (hard limit)
    set_limit(RLIMIT_NOFILE, 512, 1024);

    return 0;
}

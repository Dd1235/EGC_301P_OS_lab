#include <stdio.h>
#include <unistd.h>

int main()
{
    printf("Process running... PID = %d\n", getpid());

    while (1)
    {
        sleep(1);
    }

    return 0;
}

// ./a.out & then kill <pid>
// cd /proc/pid
// cat status
// ls -l /proc/pid/exe
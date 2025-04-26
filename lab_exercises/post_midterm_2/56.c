#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <x86intrin.h> // For __rdtsc() intrinsic

// need to check this on a liniux machien with x86

// 56
// to look at the time taken to execute 100 getppid() calls;
// rdtsc is a CPU instruction that reads the time-stamp counter
// tsc - time stamp counter
// intel or amds x86
// this won't work for apples chip

int main()
{
    unsigned long long start, end;
    pid_t ppid;

    // Get the initial timestamp counter value
    start = __rdtsc();

    // Execute 100 getppid() system calls
    for (int i = 0; i < 100; i++)
    {
        ppid = getppid(); // Get the parent process ID
    }

    // Get the final timestamp counter value
    end = __rdtsc();

    // Calculate the time taken in clock cycles
    printf("Time taken for 100 getppid() calls: %llu clock cycles\n", end - start);

    return 0;
}
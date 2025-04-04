
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/types.h>

// find scheduling policy and modify it
// this code will not work on macos

void print_policy(int policy)
{
    switch (policy)
    {
    case SCHED_OTHER:
        printf("Current Scheduling Policy: SCHED_OTHER (Default)\n");
        break;
    case SCHED_FIFO:
        printf("Current Scheduling Policy: SCHED_FIFO (Real-time, First-In-First-Out)\n");
        break;
    case SCHED_RR:
        printf("Current Scheduling Policy: SCHED_RR (Real-time, Round Robin)\n");
        break;
    default:
        printf("Unknown Scheduling Policy.\n");
    }
}

int main()
{
    pid_t pid = getpid();

    int currentPolicy = sched_getscheduler(pid);
    if (currentPolicy == -1)
    {
        perror("Failed to get scheduling policy");
        return 1;
    }

    print_policy(currentPolicy);

    struct sched_param param;
    param.sched_priority = 10;
    if (sched_setscheduler(pid, SCHED_FIFO, &param) == -1)
    {
        perror("Failed to change scheduling policy. Run as root.");
        return 1;
    }

    currentPolicy = sched_getscheduler(pid);
    print_policy(currentPolicy);

    return 0;
}

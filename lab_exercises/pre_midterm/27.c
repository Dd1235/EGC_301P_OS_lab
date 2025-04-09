
#include <stdio.h>
#include <unistd.h>
#include <sched.h>

// get maximum and minimum real time priority

int main()
{
    int min_fifo = sched_get_priority_min(SCHED_FIFO);
    int max_fifo = sched_get_priority_max(SCHED_FIFO);

    int min_rr = sched_get_priority_min(SCHED_RR);
    int max_rr = sched_get_priority_max(SCHED_RR);

    printf("SCHED_FIFO: Min = %d, Max = %d\n", min_fifo, max_fifo);
    printf("SCHED_RR  : Min = %d, Max = %d\n", min_rr, max_rr);
}

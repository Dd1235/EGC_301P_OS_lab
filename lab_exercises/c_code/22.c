
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

// create zombie state, basically parent not terminated before child terminated + parent did not reap the child on termination, so child goes to zombie
// if parent killed before off, repareted by systemd and doesnt become zombie

int main()
{
    int PID = fork();
    if (PID == 0)
    {
        printf("Child/Zombie. Terminating without being reaped by parent\n");
        exit(0);
    }
    else
    {
        sleep(10); // sleeping for 10 sec.`
        printf("Parent.\n");
    }
}

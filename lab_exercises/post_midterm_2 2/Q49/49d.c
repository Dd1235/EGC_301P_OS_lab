// remove the created semaphore

#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main()
{
    int k = ftok(".", 'k');
    int sid = semget(k, 1, 0);
    if (semctl(sid, IPC_RMID, 0) == -1)
    {
        perror("");
        exit(1);
    }
    printf("Semaphore removed.\n");
    return 0;
}

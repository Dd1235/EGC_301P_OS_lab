#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

// shared memory there is no kernel copying unlike message queues, multiple processes map a common memory region to their address space
// but it does not handle synchronization
// usually a file name like /my_shm
// there till shm_unlink() is called

int main()
{
    key_t key;
    int shmid;
    char *shmaddr;
    const char *data = "some dummy data for 47";

    key = ftok(".", 'm');
    if (key == -1)
    {
        perror("ftok failed");
        exit(1);
    }

    // Create shared memory segment
    shmid = shmget(key, 1024, IPC_CREAT | 0666);
    if (shmid == -1)
    {
        perror("shmget failed");
        exit(1);
    }

    // Attach in read-write mode
    shmaddr = (char *)shmat(shmid, NULL, 0);
    if (shmaddr == (char *)-1)
    {
        perror("shmat failed");
        exit(1);
    }

    // (a) Write to shared memory
    strcpy(shmaddr, data);
    printf("Data written to shared memory: %s\n", data);

    // (b) Detach
    if (shmdt(shmaddr) == -1)
    {
        perror("shmdt failed");
        exit(1);
    }

    // (b) Reattach in read-only mode
    shmaddr = (char *)shmat(shmid, NULL, SHM_RDONLY);
    if (shmaddr == (char *)-1)
    {
        perror("shmat failed");
        exit(1);
    }

    // (c) Read safely in read-only mode
    printf("Data read from shared memory (read-only): %s\n", shmaddr);

    // (c) Detach again
    if (shmdt(shmaddr) == -1)
    {
        perror("shmdt failed");
        exit(1);
    }

    // (d) Remove shared memory segment
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        perror("shmctl IPC_RMID failed");
        exit(1);
    }

    printf("Shared memory segment removed successfully.\n");
    return 0;
}

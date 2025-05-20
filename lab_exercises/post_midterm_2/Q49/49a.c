// 49a program to implement semaphore to protect any critical section
// a: rewrite the tickket num ber creation using semaphore
#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>

#define SHM_K 111 // key for shared memory
#define SHM_SIZE 1024

int main()
{
    // create semaphore, initialize with 1 so that it acts as mutex
    int k = ftok(".", 'k');
    int para = 1;
    int sid = semget(k, 1, IPC_CREAT | 0644);
    semctl(sid, 0, SETVAL, para);

    int shmid;
    char *shmaddr;
    struct shmid_ds shminfo;

    shmid = shmget(SHM_K, SHM_SIZE, IPC_CREAT | 0644);
    if (shmid == -1)
    {
        perror("shmget");
        exit(1);
    }
    printf("SHARED MEMORY SEGMENT CREATED.\n");
    shmaddr = shmat(shmid, NULL, 0);
    if (shmaddr == (char *)-1)
    {
        perror("shmat");
        exit(1);
    }
    printf("SHARED MEMORY SEGMENT ATTATCHED.\n");
    printf("--------------------------------\n");
    printf("Please enter the number of tickets you want : \n");
    scanf("%s", shmaddr);
    int frk = fork();
    if (frk == 0)
    {
        struct sembuf buf = {0, -1, 0};
        int sid = semget(k, 1, 0);
        semop(sid, &buf, 1);
        printf("Child. Current ticket count : %s\n", shmaddr);
        printf("Child. Enter the new number of tickets :\n");
        getchar();
        scanf("%s", shmaddr);
        buf.sem_op = 1;
        semop(sid, &buf, 1);
    }
    else
    {
        struct sembuf buf = {0, -1, 0};
        int sid = semget(k, 1, 0);
        sleep(2);
        semop(sid, &buf, 1);
        printf("Parent. Current ticket count: %s\n", shmaddr);
        buf.sem_op = 1;
        semop(sid, &buf, 1);
    }

    if (shmdt(shmaddr) == -1)
    {
        perror("shmdt");
        exit(1);
    }
    printf("SHARED MEMORY SEGMENT DETATCHED.\n");
    return 0;
}

// 49b protech shared memory from concurrent write access

#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>

#define SHM_K 112
#define SHM_SIZE 1024

int main()
{
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
    printf("Please enter the number of tickets you want :\n");
    scanf("%s", shmaddr);
    struct sembuf buf = {0, -1, 0};
    sid = semget(k, 1, 0);
    semop(sid, &buf, 1);
    strcpy(shmaddr, "101");
    sleep(10);
    buf.sem_op = 1;
    semop(sid, &buf, 1);
    if (shmdt(shmaddr) == -1)
    {
        perror("shmdt");
        exit(1);
    }
    printf("SHARED MEMORY SEGMENT DETATCHED.\n");
    if (shmctl(shmid, IPC_RMID, &shminfo) == -1)
    {
        perror("shmctl");
        exit(1);
    }
    return 0;
}

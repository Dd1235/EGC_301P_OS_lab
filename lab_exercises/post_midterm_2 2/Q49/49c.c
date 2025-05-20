// 49c protext multiple pseudo resources by a counting semaphore

#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define SHM_K 121
#define SHM_SIZE 1024

int file_write(int k, int fd)
{
    struct sembuf buf = {0, -1, 0};
    int sid = semget(k, 1, 0);
    semop(sid, &buf, 1);
    printf("In Semaphore.\n");
    printf("Value of Semaphore : %d\n", semctl(sid, 0, GETVAL));
    fd = open("49c.txt", O_RDWR | O_CREAT, 0777);
    write(fd, "data", sizeof("data"));
    sleep(10);
    buf.sem_op = 1;
    semop(sid, &buf, 1);
}

int main()
{
    int k = ftok(".", 'k');
    int arg = 2;
    int sid = semget(k, 1, IPC_CREAT | 0644);
    semctl(sid, 0, SETVAL, arg);

    int fd = open("49c.txt", O_RDWR | O_CREAT, 0777);
    write(fd, "TestTest", sizeof("TestTest"));

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
    strcpy(shmaddr, "TestTest");
    int frk = fork();
    if (frk == 0)
    {
        if (fork() == 0)
        {
            file_write(k, fd);
        }
        else
        {
            file_write(k, fd);
        }
    }
    else
    {
        file_write(k, fd);
    }
}

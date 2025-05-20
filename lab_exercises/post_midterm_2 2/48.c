#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>    // for O_CREAT, O_EXCL
#include <sys/stat.h> // for mode constants
#include <unistd.h>

int main()
{
    sem_t *binary_sem;
    sem_t *counting_sem;

    // (a) Create a binary semaphore (value 1)
    binary_sem = sem_open("/binary_sem", O_CREAT | O_EXCL, 0644, 1);
    if (binary_sem == SEM_FAILED)
    {
        perror("Failed to create binary semaphore");
        exit(1);
    }
    printf("Binary semaphore created and initialized to 1.\n");

    // (b) Create a counting semaphore (value 5)
    counting_sem = sem_open("/counting_sem", O_CREAT | O_EXCL, 0644, 5);
    if (counting_sem == SEM_FAILED)
    {
        perror("Failed to create counting semaphore");
        sem_unlink("/binary_sem");
        exit(1);
    }
    printf("Counting semaphore created and initialized to 5.\n");

    sem_close(binary_sem);
    sem_close(counting_sem);
    sem_unlink("/binary_sem");
    sem_unlink("/counting_sem");
    printf("Semaphores unlinked and closed.\n");

    return 0;
}

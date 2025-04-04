#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        perror("Argumets not matching");
        return EXIT_FAILURE;
    }

    int record_num;
    int file_desc = open(argv[1], O_RDONLY);
    if (file_desc == -1)
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    printf("Enter the record number to lock: ");
    scanf("%d", &record_num);

    struct flock file_lock = {0};
    file_lock.l_type = F_RDLCK;
    file_lock.l_whence = SEEK_SET;
    file_lock.l_start = (record_num - 1) * 100;
    file_lock.l_len = 100;
    file_lock.l_pid = getpid();

    printf("Attempting to acquire lock\n");

    if (fcntl(file_desc, F_SETLKW, &file_lock) != -1)
    {
        printf("Locked Record: %d by PID: %d\n", record_num, file_lock.l_pid);

        char buffer[100];
        lseek(file_desc, (record_num - 1) * 100, SEEK_SET);
        read(file_desc, &buffer, sizeof(buffer));
        printf("Record Data: %s\n", buffer);

        char input;
        do
        {
            printf("Enter 'U' to unlock: ");
            scanf(" %c", &input);
        } while (input != 'U');

        file_lock.l_type = F_UNLCK;
        fcntl(file_desc, F_SETLK, &file_lock);
        printf("Record %d unlocked.\n", record_num);
    }
    else
    {
        perror("Locking failed");
    }

    close(file_desc);
    return EXIT_SUCCESS;
}

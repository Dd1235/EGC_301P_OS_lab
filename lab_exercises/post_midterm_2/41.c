#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// program to create a message queue and print its key and message queue id
// ipcs -q to see all message queues
// ipcrm -q <message queue id> to remove it

// key = (inode number of file) + (device ID of file) + (project ID)

int main()
{
    key_t key;
    int msgid;

    // Generate a unique key
    key = ftok(".", 65); // here whatever is the first argument, it will use its inode no in generating a msgqueue id

    if (key == -1)
    {
        perror("ftok");
        return 1;
    }

    // Create a message queue if it doesn't already exist
    msgid = msgget(key, 0666 | IPC_CREAT);

    if (msgid == -1)
    {
        perror("msgget");
        return 1;
    }

    // Print the key and message queue id
    printf("Key: %d\n", key);
    printf("Message Queue ID: %d\n", msgid);

    return 0;
}

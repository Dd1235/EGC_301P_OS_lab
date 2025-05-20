
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main()
{
    key_t k;
    int mid;
    k = ftok(".", 'm');

    mid = msgget(k, IPC_CREAT | 0744);
    printf("Message ID : %d\n", mid);
    msgctl(mid, IPC_RMID, NULL);
    execlp("ipcs", "ipcs", "-q", NULL);
    return 0;
}
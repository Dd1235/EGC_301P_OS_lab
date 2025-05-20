#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>

struct msgbuf
{
    long int type;
    char data[100];
} mBuff;

int main()
{
    int mid;
    key_t k;
    k = ftok(".", 'm');
    mid = msgget(k, 0);
    mBuff.type = 10;
    // ipc_nowait to make it non blocking, if no matching messages available instead of waiting it just returns -1
    int size = msgrcv(mid, &mBuff, sizeof(mBuff), 10, IPC_NOWAIT);

    if (size == -1)
    {
        printf("NO DATA AVAILABLE\n");
        return 1;
    }
    else
        printf("Data Recieved : %s\n", mBuff.data);

    execlp("ipcs", "ipcs", "-q", NULL);
    return 0;
}
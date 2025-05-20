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
    msgrcv(mid, &mBuff, sizeof(mBuff), 10, 0);
    printf("Data Recieved : %s\n", mBuff.data);
    execlp("ipcs", "ipcs", "-q", NULL);
    return 0;
}

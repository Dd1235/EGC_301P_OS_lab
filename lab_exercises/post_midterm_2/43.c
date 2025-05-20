
#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>

struct mbuf
{
    long int type;
    char data[100];
} mBuff;

int main()
{

    int mid;
    key_t k;
    k = ftok(".", 'm');
    mid = msgget(k, 0); // if no queue exists this will fail
    mBuff.type = 10;    // each message needs a type to distinguish
    fgets(mBuff.data, 100, stdin);
    msgsnd(mid, &mBuff, sizeof(mBuff), 0);
    execlp("ipcs", "ipcs", "-q", NULL);
    return 0;
}
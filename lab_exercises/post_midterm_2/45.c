#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main()
{
    key_t k;
    int mid;
    k = ftok(".", 'm');
    mid = msgget(k, 0744 | IPC_CREAT);

    struct msqid_ds ms_info;
    msgctl(mid, IPC_STAT, &ms_info);
    struct ipc_perm *permissions = &ms_info.msg_perm;
    printf("Permissions : %d\n", permissions->mode);
    permissions->mode = 511;
    ms_info.msg_perm = *permissions;
    msgctl(mid, IPC_SET, &ms_info);
    printf("\nUpdated Permissions\n");
    msgctl(mid, IPC_STAT, &ms_info);
    printf("Updated Permissions : %d\n", permissions->mode);
    return 0;
}

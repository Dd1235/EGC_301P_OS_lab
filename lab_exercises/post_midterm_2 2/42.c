#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>

int main()
{
    key_t k;
    int mid;
    k = ftok(".", 'm');
    mid = msgget(k, 0744 | IPC_CREAT);
    struct msqid_ds ms_info;
    msgctl(mid, IPC_STAT, &ms_info);
    struct ipc_perm permissions = ms_info.msg_perm;

    printf("Access permissions 			= %d\n", permissions.mode);
    printf("----------------------------------------\n");
    printf("UID 				= %d\n", permissions.uid);
    printf("----------------------------------------\n");
    printf("GID 				= %d\n", permissions.gid);
    printf("----------------------------------------\n");
    printf("Time when last message was sent 	= %ld\n", ms_info.msg_stime);
    printf("----------------------------------------\n");
    printf("Time when last message was received = %ld\n", ms_info.msg_rtime);
    printf("----------------------------------------\n");
    printf("Time when last changes took place 	= %ld\n", ms_info.msg_ctime);
    printf("----------------------------------------\n");
    printf("Message Queue Size 			= %ld\n", ms_info.msg_cbytes);
    printf("----------------------------------------\n");
    printf("No. of Messages 			= %ld\n", ms_info.msg_qnum);
    printf("----------------------------------------\n");
    printf("Maximum no. of bytes 		= %ld\n", ms_info.msg_qbytes);
    printf("----------------------------------------\n");
    printf("PID of last send message 		= %d\n", ms_info.msg_lspid);
    printf("----------------------------------------\n");
    printf("PID of last received messages 	= %d\n", ms_info.msg_lrpid);
    printf("----------------------------------------\n");

    return 0;
}
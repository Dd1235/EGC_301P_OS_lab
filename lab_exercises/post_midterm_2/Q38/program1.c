// two way fifo communication
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_SEND "fifo1"
#define FIFO_RECEIVE "fifo2"

int main()
{
    int fd_send, fd_receive;
    char send_msg[100] = {0};
    char receive_msg[100] = {0};

    mkfifo("fifo1", 0666);
    mkfifo("fifo2", 0666);

    fd_send = open(FIFO_SEND, O_WRONLY);
    fd_receive = open(FIFO_RECEIVE, O_RDONLY);
    printf("Program 1: Enter Message : ");
    scanf("%[^\n]", send_msg);
    write(fd_send, send_msg, sizeof(send_msg));
    read(fd_receive, receive_msg, sizeof(receive_msg));
    printf("Program 1: Received message : %s\n", receive_msg);
    close(fd_send);
    close(fd_receive);
    unlink(FIFO_SEND);
    unlink(FIFO_RECEIVE);
    return 0;
}
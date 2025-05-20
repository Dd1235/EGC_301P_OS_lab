// two way fifo communication second program
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

int main()
{
    int fd_send, fd_receive;
    char send_msg[100] = {0};
    char receive_msg[100] = {0};

    fd_receive = open("fifo1", O_RDONLY);
    fd_send = open("fifo2", O_WRONLY);

    read(fd_receive, receive_msg, sizeof(receive_msg));
    printf("Program 2: Message Recieved : %s\n", receive_msg);
    printf("Program 2: Enter Message : ");
    scanf("%[^\n]", send_msg);
    write(fd_send, send_msg, sizeof(send_msg));

    close(fd_send);
    close(fd_receive);
    return 0;
}

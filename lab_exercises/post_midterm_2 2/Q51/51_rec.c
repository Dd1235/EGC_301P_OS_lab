// inter machine communication using sockets

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

int main()
{
    struct sockaddr_in serv;
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == -1)
    {
        perror("Error: ");
        return -1;
    }
    int portno = 5555;
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(portno);
    if (connect(sd, (struct sockaddr *)&serv, sizeof(serv)) != -1)
    {
        write(sd, "TestTest", sizeof("TestTest"));
        char buf[100];
        read(sd, buf, sizeof(buf));
        printf("Server Ack : %s\n", buf);
    }
    else
    {
        printf("CONNECTION UNSSCCESSFUL.\n");
        perror("");
    }
    close(sd);
}
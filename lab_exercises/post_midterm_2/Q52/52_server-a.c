// using fork to create a concurrent server
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define MAX_NUM_CLIENTS 5
#define PORT 3001

int main()
{
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int serv, cli;
    char buffer[256];
    if ((serv = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Failed");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(serv, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Binding ailed");
        exit(EXIT_FAILURE);
    }
    if (listen(serv, MAX_NUM_CLIENTS) < 0)
    {
        perror("Listening Failed");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        cli = accept(serv, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (!fork())
        {
            close(serv);
            read(cli, &buffer, sizeof(buffer));
            printf("Client Data : %s\n", buffer);
            sleep(10);
            exit(0);
        }
        else
            close(cli);
    }
    close(serv);
    return 0;
}

// client for the concurrent server

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 3001

int main()
{
    int client_fd;
    struct sockaddr_in server_address;
    char message[256];
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);
    if (connect(client_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }
    printf("Enter message to be sent to server : ");
    fgets(message, sizeof(message), stdin);
    if (write(client_fd, message, strlen(message)) < 0)
    {
        perror("Failed");
        exit(EXIT_FAILURE);
    }
    char buffer[256];
    ssize_t num_bytes = read(client_fd, buffer, sizeof(buffer) - 1);
    if (num_bytes > 0)
    {
        buffer[num_bytes] = '\0';
        printf("Server Response : %s\n", buffer);
    }
    close(client_fd);
    return 0;
}

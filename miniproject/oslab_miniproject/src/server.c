// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#include <time.h>

#include "net_utils.h"
#include "auth_utils.h"
#include "admin_ops.h"
#include "student_ops.h"
#include "faculty_ops.h"

#define PORT 8080

// Logs server events to stdout with timestamp
void log_event(const char *username, const char *action)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    if (username)
        printf("[%02d:%02d:%02d] [%s] %s\n", t->tm_hour, t->tm_min, t->tm_sec, username, action);
    else
        printf("[%02d:%02d:%02d] %s\n", t->tm_hour, t->tm_min, t->tm_sec, action);
}

// Handles each connected client
void *client_handler(void *arg)
{
    int client_sock = *(int *)arg;
    free(arg);

    char buffer[BUFF_SIZE], user_role[16], user_name[64];
    int authenticated = 0;

    while (1)
    {
        memset(buffer, 0, BUFF_SIZE);
        int bytes_received = recv(client_sock, buffer, BUFF_SIZE, 0);
        if (bytes_received <= 0)
            break;

        buffer[bytes_received] = '\0';

        // First message must be AUTH
        if (!authenticated)
        {
            char *token = strtok(buffer, ":");
            if (token && strcmp(token, "AUTH") == 0)
            {
                char *role = strtok(NULL, ":");
                char *username = strtok(NULL, ":");
                char *password = strtok(NULL, ":");

                if (role && username && password &&
                    authenticate(role, username, password))
                {
                    char logmsg[128];
                    snprintf(logmsg, sizeof(logmsg), "Login successful (%s)", role);
                    log_event(username, logmsg);

                    authenticated = 1;
                    strncpy(user_role, role, sizeof(user_role) - 1);
                    strncpy(user_name, username, sizeof(user_name) - 1);
                    sender(client_sock, "AUTH_SUCCESS");
                }
                else
                {
                    char logmsg[128];
                    snprintf(logmsg, sizeof(logmsg), "Login failed (%s)", role);
                    log_event(username, logmsg);

                    sender(client_sock, "AUTH_FAIL");
                    break;
                }
            }
            else
            {
                sender(client_sock, "AUTH_FAIL");
                break;
            }
        }
        else
        {
            // Menu option
            char *token = strtok(buffer, ":");
            if (token && strcmp(token, "MENU") == 0)
            {
                char *role = strtok(NULL, ":");
                char *choice_str = strtok(NULL, ":");
                int choice = (choice_str) ? atoi(choice_str) : 0;

                char logmsg[128];
                snprintf(logmsg, sizeof(logmsg), "Selected menu option %d", choice);
                log_event(user_name, logmsg);

                if (choice == 5)
                {
                    sender(client_sock, "Goodbye!");
                    break;
                }

                if (strcmp(role, "admin") == 0)
                    process_admin(client_sock, choice);
                else if (strcmp(role, "student") == 0)
                    process_student(client_sock, choice, user_name);
                else if (strcmp(role, "faculty") == 0)
                    process_faculty(client_sock, choice, user_name);
                else
                    sender(client_sock, "Unknown role.");
            }
            else
            {
                sender(client_sock, "Unknown command");
            }
        }
    }
    log_event(user_name, "Disconnected.");

    close(client_sock);
    pthread_exit(NULL);
}

// Main function: Starts TCP server and listens for connections
int main()
{
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    pthread_t tid;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0)
    {
        perror("Socket error");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind error");
        exit(1);
    }

    if (listen(server_sock, 10) == 0)
        printf("Listening on port %d...\n", PORT);
    else
    {
        perror("Listen error");
        exit(1);
    }

    while (1)
    {
        addr_size = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_size);
        if (client_sock < 0)
        {
            perror("Accept error");
            continue;
        }
        log_event(NULL, "Client connected.");

        int *pclient = malloc(sizeof(int));
        *pclient = client_sock;
        pthread_create(&tid, NULL, client_handler, pclient);
        pthread_detach(tid); // So we don’t have to pthread_join()
    }

    close(server_sock);
    return 0;
}

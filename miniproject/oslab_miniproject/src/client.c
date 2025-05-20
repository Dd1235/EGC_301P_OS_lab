// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFF_SIZE 1024
#define PORT 8080

// Flush stdin after scanf
void clear_input_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

// Prints the role-specific menu
void print_menu(const char *role)
{
    if (strcmp(role, "admin") == 0)
    {
        printf("\nAdmin Menu:\n");
        printf("1. Add Student\n");
        printf("2. Add Faculty\n");
        printf("3. Activate/Deactivate Student\n");
        printf("4. Update Student/Faculty details\n");
        printf("5. Exit\n");
    }
    else if (strcmp(role, "student") == 0)
    {
        printf("\nStudent Menu:\n");
        printf("1. Enroll to new Courses\n");
        printf("2. Unenroll from already enrolled Courses\n");
        printf("3. View enrolled Courses\n");
        printf("4. Password Change\n");
        printf("5. Exit\n");
    }
    else if (strcmp(role, "faculty") == 0)
    {
        printf("\nFaculty Menu:\n");
        printf("1. Add new Course\n");
        printf("2. Remove offered Course\n");
        printf("3. View enrollments in Courses\n");
        printf("4. Password Change\n");
        printf("5. Exit\n");
    }
}

// Sends a line of text input from user to the server
void send_text(int sock, char *buf)
{
    fgets(buf, BUFF_SIZE, stdin);
    buf[strcspn(buf, "\n")] = 0; // Remove newline
    send(sock, buf, strlen(buf), 0);
}

// Handles multi-step interactions (e.g., add user, add course)
void handle_multistep(int sock)
{
    char buffer[BUFF_SIZE];
    while (1)
    {
        memset(buffer, 0, sizeof(buffer));
        int n = recv(sock, buffer, sizeof(buffer), 0);
        if (n <= 0)
            break;

        buffer[n] = '\0';
        printf("Server: %s\n", buffer);

        // Check if it's a prompt
        if (strncmp(buffer, "Enter", 5) == 0)
        {
            send_text(sock, buffer);
        }
        else
        {
            break;
        }
    }
}

// Main entry point
int main()
{
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFF_SIZE];
    char role[16], username[64], password[64];
    int choice;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }

    // Authentication prompt
    printf("Enter role (admin/student/faculty): ");
    fgets(role, sizeof(role), stdin);
    role[strcspn(role, "\n")] = 0;
    printf("Enter username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;
    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;

    // Send AUTH message
    snprintf(buffer, sizeof(buffer), "AUTH:%s:%s:%s", role, username, password);
    send(sock, buffer, strlen(buffer), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);

    if (strcmp(buffer, "AUTH_SUCCESS") != 0)
    {
        printf("Authentication failed. Exiting.\n");
        close(sock);
        return 1;
    }

    printf("Login successful!\n");

    while (1)
    {
        print_menu(role);
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clear_input_buffer();

        if (choice == 5)
        {
            printf("Exiting...\n");
            break;
        }

        snprintf(buffer, sizeof(buffer), "MENU:%s:%d", role, choice);
        send(sock, buffer, strlen(buffer), 0);

        // Decide whether to handle multi-step input
        int multistep = 0;
        if (strcmp(role, "admin") == 0 && (choice >= 1 && choice <= 4))
            multistep = 1;
        if (strcmp(role, "student") == 0 && (choice == 1 || choice == 2 || choice == 4))
            multistep = 1;
        if (strcmp(role, "faculty") == 0 && (choice == 1 || choice == 2 || choice == 4))
            multistep = 1;

        if (multistep)
        {
            handle_multistep(sock);
        }
        else
        {
            memset(buffer, 0, sizeof(buffer));
            int n = recv(sock, buffer, sizeof(buffer), 0);
            if (n > 0)
            {
                buffer[n] = '\0';
                printf("Server: %s\n", buffer);
            }
        }
    }

    close(sock);
    return 0;
}

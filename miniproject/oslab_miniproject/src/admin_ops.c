// admin_ops.c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "net_utils.h"

// F_WRLCK is used

// Admin function to add a user (student or faculty)
void add_user(const char *role, int client_sock)
{
    char buffer[BUFF_SIZE], username[64], password[64];

    sender(client_sock, "Enter username:");
    receiver(client_sock, buffer, sizeof(buffer));
    strncpy(username, buffer, sizeof(username) - 1);

    sender(client_sock, "Enter password:");
    receiver(client_sock, buffer, sizeof(buffer));
    strncpy(password, buffer, sizeof(password) - 1);

    // Open users file in append mode and lock it
    int fd = open(USER_FILE, O_WRONLY | O_APPEND);
    if (fd < 0)
    {
        sender(client_sock, "File error.");
        return;
    }

    struct flock lock = {F_WRLCK, SEEK_SET, 0, 0};
    fcntl(fd, F_SETLKW, &lock);

    // Write user entry: username|role|password|1 (active)
    dprintf(fd, "%s|%s|%s|1\n", username, role, password);
    fsync(fd);

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);

    sender(client_sock, "User added successfully.");
}

// Toggle active/inactive status for a student/faculty
void activate_deactivate_user(int client_sock)
{
    char buffer[BUFF_SIZE], username[64], role[16];

    sender(client_sock, "Enter role (student/faculty):");
    receiver(client_sock, buffer, sizeof(buffer));
    strncpy(role, buffer, sizeof(role) - 1);

    sender(client_sock, "Enter username:");
    receiver(client_sock, buffer, sizeof(buffer));
    strncpy(username, buffer, sizeof(username) - 1);

    int fd = open(USER_FILE, O_RDWR);
    if (fd < 0)
    {
        sender(client_sock, "File error.");
        return;
    }

    struct flock lock = {F_WRLCK, SEEK_SET, 0, 0};
    fcntl(fd, F_SETLKW, &lock);

    FILE *fp = fdopen(fd, "r+");
    char lines[100][256];
    int count = 0, found = 0;

    // Read all lines and look for matching user
    while (fgets(lines[count], sizeof(lines[count]), fp))
    {
        char frole[16], funame[64], fpass[64];
        int active;
        sscanf(lines[count], "%63[^|]|%15[^|]|%63[^|]|%d", funame, frole, fpass, &active);

        if (strcmp(frole, role) == 0 && strcmp(funame, username) == 0)
        {
            found = 1;
            active = !active; // toggle
            sprintf(lines[count], "%s|%s|%s|%d\n", funame, frole, fpass, active);
        }
        count++;
    }

    if (!found)
    {
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        fclose(fp);
        sender(client_sock, "User not found.");
        return;
    }

    // Overwrite entire file
    fseek(fp, 0, SEEK_SET);
    for (int i = 0; i < count; i++)
        fputs(lines[i], fp);
    fflush(fp);
    ftruncate(fd, ftell(fp));
    fsync(fd);

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    fclose(fp);

    sender(client_sock, "User activation status toggled.");
}

// Update password for a student/faculty
void update_user(int client_sock)
{
    char buffer[BUFF_SIZE], username[64], role[16], newpass[64];

    sender(client_sock, "Enter username:");
    receiver(client_sock, buffer, sizeof(buffer));
    strncpy(username, buffer, sizeof(username) - 1);

    sender(client_sock, "Enter role (student/faculty):");
    receiver(client_sock, buffer, sizeof(buffer));
    strncpy(role, buffer, sizeof(role) - 1);

    sender(client_sock, "Enter new password:");
    receiver(client_sock, buffer, sizeof(buffer));
    strncpy(newpass, buffer, sizeof(newpass) - 1);

    int fd = open(USER_FILE, O_RDWR);
    if (fd < 0)
    {
        sender(client_sock, "File error.");
        return;
    }

    struct flock lock = {F_WRLCK, SEEK_SET, 0, 0};
    fcntl(fd, F_SETLKW, &lock);

    FILE *fp = fdopen(fd, "r+");
    char lines[100][256];
    int count = 0, found = 0;

    while (fgets(lines[count], sizeof(lines[count]), fp))
    {
        char frole[16], funame[64], fpass[64];
        int active;
        sscanf(lines[count], "%63[^|]|%15[^|]|%63[^|]|%d", funame, frole, fpass, &active);

        if (strcmp(frole, role) == 0 && strcmp(funame, username) == 0)
        {
            found = 1;
            sprintf(lines[count], "%s|%s|%s|%d\n", funame, frole, newpass, active);
        }
        count++;
    }

    if (!found)
    {
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        fclose(fp);
        sender(client_sock, "User not found.");
        return;
    }

    fseek(fp, 0, SEEK_SET);
    for (int i = 0; i < count; i++)
        fputs(lines[i], fp);
    fflush(fp);
    ftruncate(fd, ftell(fp));
    fsync(fd);

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    fclose(fp);

    sender(client_sock, "User updated.");
}

// Admin dispatcher: called by server.c based on choice
void process_admin(int client_sock, int choice)
{
    switch (choice)
    {
    case 1:
        add_user("student", client_sock);
        break;
    case 2:
        add_user("faculty", client_sock);
        break;
    case 3:
        activate_deactivate_user(client_sock);
        break;
    case 4:
        update_user(client_sock);
        break;
    default:
        sender(client_sock, "Invalid admin choice.");
        break;
    }
}

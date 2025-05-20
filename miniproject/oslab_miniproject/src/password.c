// password.c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include "net_utils.h"

// Changes the password for a given user and role
void change_user_password(int client_sock, const char *username, const char *role)
{
    char buffer[BUFF_SIZE], newpass[64];

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

    // Read and find matching user
    while (fgets(lines[count], sizeof(lines[count]), fp))
    {
        char frole[16], funame[64], fpass[64];
        int active;

        sscanf(lines[count], "%63[^|]|%15[^|]|%63[^|]|%d", funame, frole, fpass, &active);

        if (strcmp(frole, role) == 0 && strcmp(funame, username) == 0)
        {
            found = 1;
            snprintf(lines[count], sizeof(lines[count]), "%s|%s|%s|%d\n", funame, frole, newpass, active);
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

    // Overwrite full file
    fseek(fp, 0, SEEK_SET);
    for (int i = 0; i < count; i++)
        fputs(lines[i], fp);
    fflush(fp);
    ftruncate(fd, ftell(fp));
    fsync(fd);

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    fclose(fp);

    sender(client_sock, "Password changed.");
}

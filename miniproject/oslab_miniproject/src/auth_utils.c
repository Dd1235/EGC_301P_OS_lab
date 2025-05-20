// auth.c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "net_utils.h"
// file locking is used, F_RDLCK

// Checks if the given credentials match an active user in users.txt
int authenticate(const char *role, const char *username, const char *password)
{
    int fd = open(USER_FILE, O_RDONLY);
    if (fd < 0)
        return 0;

    // Apply read lock to avoid conflicts with admin edits
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    fcntl(fd, F_SETLKW, &lock);

    FILE *fp = fdopen(fd, "r");
    char line[256];
    int found = 0;

    while (fgets(line, sizeof(line), fp))
    {
        char frole[16], funame[64], fpass[64];
        int active;

        if (sscanf(line, "%63[^|]|%15[^|]|%63[^|]|%d", funame, frole, fpass, &active) == 4)
        {
            if (strcmp(frole, role) == 0 &&
                strcmp(funame, username) == 0 &&
                strcmp(fpass, password) == 0 &&
                active == 1)
            {
                found = 1;
                break;
            }
        }
    }

    // Unlock and clean up
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    fclose(fp);

    return found;
}

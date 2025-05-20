// faculty_ops.c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include "net_utils.h"
#include "password.h" // for change_user_password()

// Faculty adds a course (they become the instructor)
void add_course(int client_sock, const char *faculty)
{
    char buffer[BUFF_SIZE], course_id[32], course_name[64], seats[8];

    sender(client_sock, "Enter course id:");
    receiver(client_sock, buffer, sizeof(buffer));
    strncpy(course_id, buffer, sizeof(course_id) - 1);

    sender(client_sock, "Enter course name:");
    receiver(client_sock, buffer, sizeof(buffer));
    strncpy(course_name, buffer, sizeof(course_name) - 1);

    sender(client_sock, "Enter total seats:");
    receiver(client_sock, buffer, sizeof(buffer));
    strncpy(seats, buffer, sizeof(seats) - 1);

    int fd = open(COURSE_FILE, O_WRONLY | O_APPEND | O_CREAT, 0666);
    if (fd < 0)
    {
        sender(client_sock, "File error.");
        return;
    }

    struct flock lock = {F_WRLCK, SEEK_SET, 0, 0};
    fcntl(fd, F_SETLKW, &lock);

    // Format: course_id|course_name|faculty|seats|
    dprintf(fd, "%s|%s|%s|%s|\n", course_id, course_name, faculty, seats);
    fsync(fd);

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);

    sender(client_sock, "Course added.");
}

// Faculty removes their own course
void remove_course(int client_sock, const char *faculty)
{
    char buffer[BUFF_SIZE], course_id[32];

    sender(client_sock, "Enter course id to remove:");
    receiver(client_sock, buffer, sizeof(buffer));
    strncpy(course_id, buffer, sizeof(course_id) - 1);

    int fd = open(COURSE_FILE, O_RDWR);
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

    // Read and remove the course (if owned by this faculty)
    while (fgets(lines[count], sizeof(lines[count]), fp))
    {
        char cid[32], cname[64], cfac[64], cseats[8], enrolled[256];
        sscanf(lines[count], "%31[^|]|%63[^|]|%63[^|]|%7[^|]|%255[^\n]", cid, cname, cfac, cseats, enrolled);

        if (strcmp(cid, course_id) == 0 && strcmp(cfac, faculty) == 0)
        {
            found = 1; // skip this line
            continue;
        }
        count++;
    }

    if (!found)
    {
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        fclose(fp);
        sender(client_sock, "Course not found or not owned by you.");
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

    sender(client_sock, "Course removed.");
}

// View enrollments in all courses offered by this faculty
void view_course_enrollments(int client_sock, const char *faculty)
{
    int fd = open(COURSE_FILE, O_RDONLY);
    if (fd < 0)
    {
        sender(client_sock, "File error.");
        return;
    }

    struct flock lock = {F_RDLCK, SEEK_SET, 0, 0};
    fcntl(fd, F_SETLKW, &lock);

    FILE *fp = fdopen(fd, "r");
    char line[256], msg[BUFF_SIZE] = "";

    while (fgets(line, sizeof(line), fp))
    {
        char cid[32], cname[64], cfac[64], cseats[8], enrolled_list[256];
        sscanf(line, "%31[^|]|%63[^|]|%63[^|]|%7[^|]|%255[^\n]", cid, cname, cfac, cseats, enrolled_list);

        if (strcmp(cfac, faculty) == 0)
        {
            char tmp[256];
            snprintf(tmp, sizeof(tmp), "%s (%s): %s\n", cname, cid, enrolled_list);
            strcat(msg, tmp);
        }
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    fclose(fp);

    if (strlen(msg) == 0)
        sender(client_sock, "No courses or enrollments.");
    else
        sender(client_sock, msg);
}

// Dispatcher for faculty menu actions
void process_faculty(int client_sock, int choice, const char *username)
{
    switch (choice)
    {
    case 1:
        add_course(client_sock, username);
        break;
    case 2:
        remove_course(client_sock, username);
        break;
    case 3:
        view_course_enrollments(client_sock, username);
        break;
    case 4:
        change_user_password(client_sock, username, "faculty");
        break;
    default:
        sender(client_sock, "Invalid faculty choice.");
        break;
    }
}

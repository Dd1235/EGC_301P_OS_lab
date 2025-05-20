// student_ops.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include "net_utils.h"
#include "password.h"

// Enroll student in a course (if seats available and not already enrolled)
void enroll_course(int client_sock, const char *student)
{
    char buffer[BUFF_SIZE], course_id[32];

    sender(client_sock, "Enter course id to enroll:");
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

    while (fgets(lines[count], sizeof(lines[count]), fp))
    {
        char cid[32], cname[64], cfac[64], cseats[8], enrolled_list[256] = "";
        sscanf(lines[count], "%31[^|]|%63[^|]|%63[^|]|%7[^|]|%255[^\n]", cid, cname, cfac, cseats, enrolled_list);

        if (strcmp(cid, course_id) == 0)
        {
            found = 1;
            int seats = atoi(cseats);
            int n = 0, already = 0;
            char new_enrolled[256] = "", enrolled_copy[256];

            strncpy(enrolled_copy, enrolled_list, sizeof(enrolled_copy) - 1);

            if (strlen(enrolled_copy) > 0)
            {
                char *tok = strtok(enrolled_copy, ",");
                while (tok)
                {
                    if (strcmp(tok, student) == 0)
                        already = 1;
                    if (strlen(tok) > 0)
                    {
                        if (strlen(new_enrolled) > 0)
                            strcat(new_enrolled, ",");
                        strcat(new_enrolled, tok);
                        n++;
                    }
                    tok = strtok(NULL, ",");
                }
            }

            if (already)
            {
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);
                fclose(fp);
                sender(client_sock, "Already enrolled.");
                return;
            }

            if (n >= seats)
            {
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);
                fclose(fp);
                sender(client_sock, "No seats available.");
                return;
            }

            // Append student to enrollment list
            if (strlen(new_enrolled) > 0)
                snprintf(new_enrolled + strlen(new_enrolled), sizeof(new_enrolled) - strlen(new_enrolled), ",%s", student);
            else
                snprintf(new_enrolled, sizeof(new_enrolled), "%s", student);

            // Rewrite this line
            snprintf(lines[count], sizeof(lines[count]), "%s|%s|%s|%d|%s\n", cid, cname, cfac, seats, new_enrolled);
        }

        count++;
    }

    if (!found)
    {
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        fclose(fp);
        sender(client_sock, "Course not found.");
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
    sender(client_sock, "Enrolled successfully.");
}

// Unenroll from a course
void unenroll_course(int client_sock, const char *student)
{
    char buffer[BUFF_SIZE], course_id[32];

    sender(client_sock, "Enter course id to unenroll:");
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
    int count = 0, found = 0, changed = 0;

    while (fgets(lines[count], sizeof(lines[count]), fp))
    {
        char cid[32], cname[64], cfac[64], cseats[8], enrolled_list[256] = "";
        sscanf(lines[count], "%31[^|]|%63[^|]|%63[^|]|%7[^|]|%255[^\n]", cid, cname, cfac, cseats, enrolled_list);

        if (strcmp(cid, course_id) == 0)
        {
            found = 1;
            char new_enrolled[256] = "", enrolled_copy[256];
            strncpy(enrolled_copy, enrolled_list, sizeof(enrolled_copy) - 1);

            int first = 1, was_enrolled = 0;
            char *tok = strtok(enrolled_copy, ",");

            while (tok)
            {
                if (strcmp(tok, student) != 0)
                {
                    if (!first)
                        strcat(new_enrolled, ",");
                    strcat(new_enrolled, tok);
                    first = 0;
                }
                else
                {
                    was_enrolled = 1;
                }
                tok = strtok(NULL, ",");
            }

            if (!was_enrolled)
            {
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);
                fclose(fp);
                sender(client_sock, "Not enrolled in this course.");
                return;
            }

            snprintf(lines[count], sizeof(lines[count]), "%s|%s|%s|%s|%s\n", cid, cname, cfac, cseats, new_enrolled);
            changed = 1;
        }

        count++;
    }

    if (!found)
    {
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        fclose(fp);
        sender(client_sock, "Course not found.");
        return;
    }

    if (!changed)
    {
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        fclose(fp);
        sender(client_sock, "Not enrolled in this course.");
        return;
    }

    fseek(fp, 0, SEEK_SET);
    for (int i = 0; i < count; i++)
        fputs(lines[i], fp);
    fflush(fp);
    ftruncate(fd, ftell(fp));
    // fsync(fp);
    fsync(fileno(fp));

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    fclose(fp);
    sender(client_sock, "Unenrolled successfully.");
}

// View enrolled courses
void view_enrolled_courses(int client_sock, const char *student)
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

        char *tok = strtok(enrolled_list, ",");
        while (tok)
        {
            if (strcmp(tok, student) == 0)
            {
                char tmp[256];
                snprintf(tmp, sizeof(tmp), "%s (%s)\n", cname, cid);
                strcat(msg, tmp);
                break;
            }
            tok = strtok(NULL, ",");
        }
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    fclose(fp);

    if (strlen(msg) == 0)
        sender(client_sock, "No enrolled courses.");
    else
        sender(client_sock, msg);
}

// Dispatches student choice
void process_student(int client_sock, int choice, const char *username)
{
    switch (choice)
    {
    case 1:
        enroll_course(client_sock, username);
        break;
    case 2:
        unenroll_course(client_sock, username);
        break;
    case 3:
        view_enrolled_courses(client_sock, username);
        break;
    case 4:
        change_user_password(client_sock, username, "student");
        break;
    default:
        sender(client_sock, "Invalid student choice.");
        break;
    }
}

// net_utils.h
#ifndef NET_UTILS_H
#define NET_UTILS_H

#include <stddef.h>

// Buffer size for socket communication
#define BUFF_SIZE 1024

// run from the root directory
#define USER_FILE "data/users.txt"
#define COURSE_FILE "data/courses.txt"

// Sends a null-terminated string to the given socket
void sender(int sock, const char *msg);

// Receives a message from the socket into buf (with newline removed)
void receiver(int sock, char *buf, size_t buflen);

#endif

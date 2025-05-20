// comm.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "net_utils.h"

// Sends a message to the client
void sender(int sock, const char *msg)
{
    send(sock, msg, strlen(msg), 0);
}

// Receives a message from the client
void receiver(int sock, char *buf, size_t buflen)
{
    memset(buf, 0, buflen);
    int n = recv(sock, buf, buflen - 1, 0); // Reserve 1 byte for '\0'
    if (n <= 0)
    {
        buf[0] = '\0';
        return;
    }
    buf[n] = '\0';

    // Remove any trailing newline or carriage return
    buf[strcspn(buf, "\r\n")] = 0;
}

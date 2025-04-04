#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *fp = fopen("ticket.txt", "w");
    if (fp == NULL)
    {
        perror("Error creating file");
        exit(EXIT_FAILURE);
    }

    int ticket_number = 1000;
    fprintf(fp, "%d\n", ticket_number);
    fclose(fp);

    printf("Ticket system initialized with ticket number: %d\n", ticket_number);
    return 0;
}

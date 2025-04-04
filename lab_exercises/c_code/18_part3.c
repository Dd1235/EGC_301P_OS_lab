#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int f1 = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, 0777);
    if (f1 == -1)
    {
        perror("Error opening file");
        return 1;
    }

    char data1[] = "Locking File\n";
    char data2[] = "Unlocking File\n";
    char data3[] = "Modifying File\n";

    write(f1, data1, strlen(data1));
    write(f1, data2, strlen(data2));
    write(f1, data3, strlen(data3));

    close(f1);
    return 0;
}

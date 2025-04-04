
#include <stdio.h>
#include <unistd.h>

int main()
{
    printf("using execlp:\n");
    execlp("/bin/ls", "-R", "-l", NULL);
}

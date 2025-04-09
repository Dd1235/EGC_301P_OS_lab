
#include <stdio.h>
#include <unistd.h>

int main()
{
    printf("unsing execl:\n");
    execl("/bin/ls", "-R", "-l", NULL);
}

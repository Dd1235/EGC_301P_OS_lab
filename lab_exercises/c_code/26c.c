
#include <stdio.h>
#include <unistd.h>

// using execle

int main()
{
    extern char **environ;
    printf("using execle:\n");
    // for linux both __environ and environ run I believe
    execle("/bin/ls", "-R", "-l", NULL, __environ);
    // for mac please use: execle("/bin/ls", "ls", "-R", "-l", NULL, environ);
}

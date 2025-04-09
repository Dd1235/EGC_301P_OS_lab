#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
// execute using exec sys call, pass input into an executable program
// please run gcc exec_for_25a.c -o exec_for_25a before running this one

int main()
{
    char *programPath = "./exec_for_25a";
    char *execArgs[] = {programPath, NULL};

    execv(execArgs[0], execArgs);

    perror("Execution failed");
    return 1;
}

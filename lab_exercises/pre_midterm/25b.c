
#include <stdio.h>
#include <unistd.h>

// please run gcc -o exec_for_25b exec_for_25b.c before running this so create the executable

int main()
{
    char *programPath = "./exec_for_25b"; // Path to the executable
    char *arguments[] = {"exec_for_25b", "ArgOne", "ArgTwo", NULL};

    // Execute the program
    if (execv(programPath, arguments) == -1)
    {
        perror("Execution error");
    }

    return 1;
}

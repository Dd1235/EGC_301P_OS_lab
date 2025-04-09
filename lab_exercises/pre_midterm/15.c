// display users environment variables using environ

#include <stdio.h>

// environ is a global variable provided by the os, is it an array of strings

extern char **environ;

int main()
{
    char **env = environ;

    printf("Environment variables:\n");
    while (*env)
    {
        printf("%s\n", *env);
        env++;
    }

    return 0;
}

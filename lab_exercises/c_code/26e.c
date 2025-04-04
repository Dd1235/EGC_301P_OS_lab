

#include <stdio.h>
#include <unistd.h>

int main()
{
    char *args_arr[] = {"/bin/ls", "-R", "-l", NULL};
    printf("using execvp:\n");
    execvp(args_arr[0], args_arr);
}

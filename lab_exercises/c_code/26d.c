
#include <stdio.h>
#include <unistd.h>

int main()
{
    char *args_arr[] = {"/bin/ls", "-R", "-l", NULL};
    printf("using execv:\n");
    execv(args_arr[0], args_arr);
}

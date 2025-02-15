#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>

int main()
{
    const char *source_file_rel = "../sample.txt"; // here not the path relative to destination but path relative to executable
    const char *dest_file_rel = "../sample_hardlink_copy.txt";

    if (link(source_file_rel, dest_file_rel) == 0)
    {
        printf("Hard link created: %s -> %s\n", dest_file_rel, source_file_rel);
    }
    else
    {
        perror("hardlink");
    }

    return 0;
}

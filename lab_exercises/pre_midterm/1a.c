#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h> // for real path

int main()
{
    const char *source_file_rel = "./sample.txt";    // relative to destination
    const char *dest_file_rel = "./sample_copy.txt"; // relative to where executable is

    // char source_file[PATH_MAX];
    // char dest_file[PATH_MAX];

    // realpath("./sample.txt", source_file);
    // realpath("../sample_copy.txt", dest_file);

    // printf("Source file: %s\n", source_file);
    // printf("Destination file: %s\n", dest_file);

    // symlink returns 0 on success, -1 and errno to set to indicate error
    if (symlink(source_file_rel, dest_file_rel) == 0)
    {
        printf("symlink created, %s -> %s\n", dest_file_rel, source_file_rel);
    }
    else
    {
        perror("symlink"); // perror appends to errno
    }
}
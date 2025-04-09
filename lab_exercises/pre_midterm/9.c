#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

void print_file_info(const char *filename)
{
    struct stat file_stat;

    if (stat(filename, &file_stat) == -1)
    {
        perror("Error getting file status");
        exit(EXIT_FAILURE);
    }

    printf("File Information for: %s\n", filename);
    printf("----------------------------------------------------\n");
    printf("a. Inode Number        : %lu\n", file_stat.st_ino);
    printf("b. Number of Hard Links: %lu\n", file_stat.st_nlink);
    printf("c. User ID (UID)       : %u\n", file_stat.st_uid);
    printf("d. Group ID (GID)      : %u\n", file_stat.st_gid);
    printf("e. File Size (bytes)   : %ld\n", file_stat.st_size);
    printf("f. Block Size (bytes)  : %ld\n", file_stat.st_blksize);
    printf("g. Number of Blocks    : %ld\n", file_stat.st_blocks);
    printf("h. Last Access Time    : %s", ctime(&file_stat.st_atime));
    printf("i. Last Modification Time: %s", ctime(&file_stat.st_mtime));
    printf("j. Last Change Time    : %s", ctime(&file_stat.st_ctime));
    printf("----------------------------------------------------\n");
    printf("Time values without formatting:\n");
    printf("Last Access Time (Raw)     : %ld\n", file_stat.st_atime);
    printf("Last Modification Time (Raw): %ld\n", file_stat.st_mtime);
    printf("Last Change Time (Raw)     : %ld\n", file_stat.st_ctime);
    printf("----------------------------------------------------\n");
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    print_file_info(argv[1]);

    return EXIT_SUCCESS;
}

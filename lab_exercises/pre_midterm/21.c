

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

// open file, fork , both parent and child write to file

int main(int argc, char *argv[])
{
    // if (argc != 2)
    // {
    //     perror("Usage: <executable> <file to write to>");
    //     return 1;
    // }

    // char *filename = argv[1]; // uncomments this line to instead pass in the file name as an argument
    char *filename = "sample_file_for_21.txt";
    // will create if not exists, otherwise appaents
    int f1 = open(filename, O_CREAT | O_RDWR | O_APPEND, 0777);

    if (f1 < 0)
    {
        perror("File open failed");
        return 1;
    }

    int PID = fork();

    if (PID == 0)
    { // Child Process
        write(f1, "Child writing\n", 14);
    }
    else
    { // Parent Process
        write(f1, "Parent writing\n", 15);
    }

    fsync(f1);
    close(f1);

    return 0;
}

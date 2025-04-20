#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PROC_DIR "/proc"
#define LOG_FILE "process_info.log"

// opens proc directory, go through numeric entries that is numeric and for each pid read /proc/pid/stat
void get_process_info()
{
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(PROC_DIR)) == NULL)
    {
        perror("Failed to open /proc");
        exit(1);
    }

    FILE *log = fopen(LOG_FILE, "w");
    if (!log)
    {
        perror("Error opening log file");
        closedir(dir);
        exit(1);
    }

    fprintf(log, "PID\tCMD\tSTATE\tPPID\tVM SIZE (KB)\n");
    fprintf(log, "-----------------------------------------------------\n");

    while ((entry = readdir(dir)) != NULL)
    {
        if (!isdigit(entry->d_name[0]))
        {
            continue;
        }

        char stat_path[256], line[512];
        snprintf(stat_path, sizeof(stat_path), PROC_DIR "/%s/stat", entry->d_name);

        FILE *stat_file = fopen(stat_path, "r");
        if (!stat_file)
        {
            continue; // Process might have terminated
        }

        if (fgets(line, sizeof(line), stat_file))
        {
            int pid, ppid, vsize;
            char comm[256], state;

            sscanf(line, "%d %s %c %d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %d",
                   &pid, comm, &state, &ppid, &vsize);

            char *cmd_start = strchr(comm, '(') + 1;
            char *cmd_end = strrchr(comm, ')');
            *cmd_end = '\0';

            fprintf(log, "%d\t%s\t%c\t%d\t%d KB\n", pid, cmd_start, state, ppid, vsize / 1024);
        }

        fclose(stat_file);
    }

    fclose(log);
    closedir(dir);
}

int main()
{
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("Fork failed");
        return 1;
    }

    if (pid > 0)
    {
        // Parent process exits, leaving the child in the background
        printf("Process running in background with PID: %d\n", pid);
        return 0;
    }

    setsid(); // creates a new session id
    chdir("/");
    umask(0); // file creation mode to 0 to prevent any unexpected permission restrictions

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1)
    {
        get_process_info(); // Gather and log process info
        sleep(5);           // Update every 5 seconds
    }

    return 0;
}

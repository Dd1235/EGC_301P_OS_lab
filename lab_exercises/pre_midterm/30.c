
// dameon process runs in the background without interfering wtih us programs
// cron is daemon, nm not using cron jobs
// wait 30 seconds after running for it to log a message to daemon_task.log
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>

void daemonize()
{
    pid_t pid = fork();

    if (pid < 0)
        exit(EXIT_FAILURE);
    if (pid > 0)
        exit(EXIT_SUCCESS); // Parent exits, child continues

    // Child becomes session leader
    if (setsid() < 0)
        exit(EXIT_FAILURE);

    // Fork again to prevent acquiring a terminal
    pid = fork();
    if (pid < 0)
        exit(EXIT_FAILURE);
    if (pid > 0)
        exit(EXIT_SUCCESS);

    // Set file permissions
    umask(0);

    // chdir("/"); logging to current dir not from root

    // Redirect stdin, stdout, stderr to /dev/null
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    open("/dev/null", O_RDONLY); // stdin
    open("/dev/null", O_RDWR);   // stdout
    open("/dev/null", O_RDWR);   // stderr
}

int main()
{
    daemonize();

    // Get the current time
    time_t start_time = time(NULL);
    struct tm *start_tm = localtime(&start_time);

    // Calculate target time 8 seconds later
    time_t target_time = start_time + 8;
    struct tm *target_tm = localtime(&target_time);

    int target_hour = target_tm->tm_hour;
    int target_minute = target_tm->tm_min;
    int target_second = target_tm->tm_sec;

    while (1)
    {
        time_t now = time(NULL);
        struct tm *current_time = localtime(&now);

        if (current_time->tm_hour == target_hour &&
            current_time->tm_min == target_minute &&
            current_time->tm_sec == target_second)
        {
            // Task to execute at specific time
            FILE *log = fopen("daemon_task.log", "a+");
            if (log)
            {
                fprintf(log, "Task executed at: %02d:%02d:%02d\n",
                        current_time->tm_hour,
                        current_time->tm_min,
                        current_time->tm_sec);
                fclose(log);
            }
            break; // Task done, break the loop (or keep running if you want it to be recurring)
        }

        sleep(1); // Check every second
    }

    return 0;
}

// sends the signal
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/signal.h>

int main()
{
	int pid;
	printf("Enter the process ID of the process you want to kill:\n");
	scanf("%d", &pid);
	kill(pid, SIGSTOP);
	return 0;
}
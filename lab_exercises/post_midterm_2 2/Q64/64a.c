// await for the sigstop signal
// second program will send signal using kill system call
// need to determine if it catches or not
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/signal.h>

void sig_handler(int signo)
{
	printf("SIGSTOP received.\n");
	exit(0);
}

int main()
{
	signal(SIGSTOP, sig_handler);
	printf("PID of this process: %d\n", getpid());
	while (1)
	{
	}
	return 0;
}
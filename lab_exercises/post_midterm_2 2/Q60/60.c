// program to ignore sigint signal and reset it to the default action using signal system call
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
int main()
{
	signal(SIGINT, SIG_IGN);
	for (int i = 0; i < 10; i++)
	{
		printf("%d\n", i);
		sleep(1);
	}
	signal(SIGINT, SIG_DFL);
	printf("SIGINT Reset\n");
	for (int i = 0; i < 10; i++)
	{
		printf("%d\n", i);
		sleep(1);
	}
}

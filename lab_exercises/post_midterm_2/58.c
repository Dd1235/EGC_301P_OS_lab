// create three threads and prints the ids

#include <stdio.h>
#include <pthread.h>
void *thread_func()
{
	printf("Thread ID : %lu\n", pthread_self());
}

int main()
{
	pthread_t mythread;
	for (int i = 0; i < 3; i++)
		pthread_create(&mythread, NULL, thread_func, NULL);
	pthread_exit(NULL);
}

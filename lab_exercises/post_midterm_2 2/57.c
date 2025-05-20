// to get the system limits
// sysconf dynamically retrieves the limits so not hte hard limits but the soft limits
// sysconf uses posix

#include <unistd.h>
#include <stdio.h>

int main()
{
	printf("a Max Length of Arguments (exec family) 				: %ld\n", sysconf(_SC_ARG_MAX));
	printf("b Max no. of users per UID 					: %ld \n", sysconf(_SC_CHILD_MAX));
	printf("c No. of Clock Tickes per Second 					: %ld \n", sysconf(_SC_CLK_TCK));
	printf("d Max no. of open files 						: %ld \n", sysconf(_SC_OPEN_MAX));
	printf("e Page Size							: %ld \n ", sysconf(_SC_PAGESIZE));
	printf("f Total no. of pages in physical mem.				: %ld\n", sysconf(_SC_PHYS_PAGES));
	printf("g No. of avaiable pages right now					: %ld\n", sysconf(_SC_AVPHYS_PAGES));

	return 0;
}

#include <sys/types.h>
#include <sys/stat.h>
// 36e mkfifo library function
int main()
{
    mkfifo("./fifoe", 0744);
}
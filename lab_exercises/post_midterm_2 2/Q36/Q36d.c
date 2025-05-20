// 36d mknod sys call
#include <sys/stat.h>

int main()
{
    mknod("./fifod", S_IFIFO | 0744, 0);
}
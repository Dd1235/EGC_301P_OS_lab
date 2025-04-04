

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>

int main()
{
    int updatedNiceValue;

    int currentPriority = getpriority(PRIO_PROCESS, getpid());
    printf("Current Process Priority: %d\n", currentPriority);

    printf("Enter the new nice value: ");
    scanf("%d", &updatedNiceValue);

    int modifiedPriority = nice(updatedNiceValue);
    if (modifiedPriority == -1)
    {
        perror("Failed to update priority");
    }
    printf("Updated Process Priority: %d\n", getpriority(PRIO_PROCESS, getpid()));

    return 0;
}

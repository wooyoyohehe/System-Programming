#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <string.h>
#include <poll.h>

int main()
{
    struct pollfd fds[2];
    char buf[10];
    int length;
    fds[0].fd = 1;
    fds[0].events = POLLIN;
    fds[1].fd = -1;
    fds[1].events = POLLIN;
    int timeout = -1; 
    poll(fds, 2, timeout);
    printf("stuck here?\n");

    while(1)
    {
        if(!poll(fds, 2, timeout))
        {
            printf("timeout\n");
        }
        else
        {
            length = read(STDIN_FILENO, &buf, 10);
            printf("the length of read is %d\n", length-1);
        }
    }
    return 0;
}
// This code is my own work. It was writtern without cosulting a tutor or code -Shuang Wu
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>

void signal_handler(int signum)
{
    printf("the signum is %d\n", signum);
    if (signum == SIGINT) printf("Interrupt \n");
    if (signum == SIGQUIT) printf("QUIT \n");
    exit(0);
}

int main(int argc, char* argv[])
{
   if (argc != 4) 
   {
       perror("Number of parameter is not right\n");
       exit(1);
   }


   struct sigaction action;

   sigset_t mask;
   sigemptyset(&mask);
   action.sa_flags=0;
   action.sa_mask=mask;
   action.sa_handler = signal_handler;

   sigaction(SIGINT, &action, NULL);
   sigaction(SIGQUIT, &signal, NULL);
   sigaction(SIGHUP, &signal, NULL);



   printf("line 31\n");


   struct sockaddr_in sin;
   int socket_fd;

   memset(&sin, 0, sizeof(sin));
   char *ptr;
   char str[30];
   strcpy(str, gethostbyname(argv[1])->h_addr);

   sin.sin_addr.s_addr = strtol(str, &ptr, 10);
   sin.sin_port = atol(argv[2]);
   sin.sin_family = AF_INET; 
   int connect_result;
   while(1)
   {
        socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_fd == -1) exit(1);
        connect_result = connect (socket_fd, (struct sockaddr *) &sin, sizeof (sin));
        if (connect_result != 0) 
        {
            printf("Connect failed but still trying every 5 seconds\n");
            close(socket_fd);
            sleep(5);
            continue;
        }
        else break;
   }
   printf("Connect success\n");

   char read_buffer[100];
   memset(&read_buffer, 0, sizeof(read_buffer));


   char send_buffer[30];
   memset(&send_buffer, 0, sizeof(send_buffer));

   return 0;
}
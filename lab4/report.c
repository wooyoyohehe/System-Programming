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


int main(int argc, char* argv[])
{
   if (argc<3 || argc >4) 
   {
       perror("Number of parameter is not right\n");
       exit(1);
   }

   char send_buffer[30];
   memset(&send_buffer, 0, sizeof(send_buffer));

   if(argc == 4)
   {
       if(strcmp("-k", argv[3])!=0)
       {
           perror("Wrong terminate parameter\n");
           exit(1);
       }
       else strcpy(send_buffer, "quit|");
   }

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
   if(argc == 3)
   {
       strcpy(send_buffer, "report|");
   }

   write(socket_fd, send_buffer, sizeof(send_buffer));

   char read_buffer[500];
   memset(&read_buffer, 0, sizeof(read_buffer));
   read(socket_fd, read_buffer, sizeof(read_buffer)-1);

   printf("%s", read_buffer);
   return 0;
}
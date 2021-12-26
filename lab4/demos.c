#include <sys/ipc.h>
//#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

//static jmp_buf buf;

void signal_handler(int sig_num)
{
    printf("\nFuck you! I will not die!\n Signal_number is %d", sig_num);

}
int main()
{
//    struct sigaction sa;
//    sa.sa_handler = &signal_handler;
//    sa.sa_flags = SA_RESTART;
//    sigaction(SIGTSTP, &sa,NULL);

//    signal(SIGTSTP, &signal_handler);
//
//    while(1)
//    {
//        printf("hehe\n");
//        sleep(1);
//    }


//    //创建套接字
//    int sock = socket(AF_INET, SOCK_STREAM, 0);
//    //向服务器（特定的IP和端口）发起请求
//    struct sockaddr_in serv_addr;
//    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
//    serv_addr.sin_family = AF_INET;  //使用IPv4地址
//    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
//    serv_addr.sin_port = htons(1234);  //端口
//    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
//
//    //读取服务器传回的数据
//    char buffer[40];
//    read(sock, buffer, sizeof(buffer)-1);
//
//    printf("Message form server: %s\n", buffer);
//
//    //关闭套接字
//    close(sock);
//    return 0;



    //创建套接字
    int serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    //将套接字和IP、端口绑定
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    serv_addr.sin_port = htons(1234);  //端口
    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    //进入监听状态，等待用户发起请求
    listen(serv_sock, 20);
    //接收客户端请求
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    //向客户端发送数据
    char str[] = "Hello World!";
    write(clnt_sock, str, sizeof(str));

    //关闭套接字
    close(clnt_sock);
    close(serv_sock);
    return 0;






//    if(setjmp(buf) != 0)
//    {
//        exit(0);
//    }
//    printf("fuck pussy\n");
//    longjmp(buf,3);
//    printf("bye bye\n");


//
//
//    int shmid;
//    if ( (shmid = shmget((key_t)0x5005, 1024, 0640|IPC_CREAT)) == -1)
//    {
//        printf("shmat(0x5005) failed\n");
//        return -1;
//    }
//    char *p_shm=NULL;
//    p_shm = (char *)shmat(shmid, 0, 0);
//    printf("The address of shared memory is %p\n", p_shm);
//    printf("Before %p\n",p_shm);
//    printf("pid is %d\n",getpid());
//    printf("After %p\n",p_shm);
//    printf("pid is %d\n",getpid());

}
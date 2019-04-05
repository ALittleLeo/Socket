#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <strings.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

/******** �궨�� *********/
#define BUFFSIZE 100

int main()
{
    int sockFd;
    int n;
    struct sockaddr_in serverAddr;
    char sendBuf[BUFFSIZE], recvBuf[BUFFSIZE];

    //�����׽���
    if((sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket error");
        exit(1);
    }

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8888);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    //�����׽���
    if(connect(sockFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("connect error");
        exit(1);
    }

    //�ӱ�׼�����ж�ȡһ������
    while(fgets(sendBuf, BUFFSIZE, stdin) != NULL)
    {
        //������д�뵽�׽��֣����͸�������
        write(sockFd, sendBuf, strlen(sendBuf));

        //���׽����ж�ȡ�ӷ���������������
        n = read(sockFd, recvBuf, BUFFSIZE);

        //�����ܵ�������д����׼�����
        write(STDIN_FILENO, recvBuf, n);
    }

    return 0;
}

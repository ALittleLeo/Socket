#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 6015                              /*�����˿ڵ�ַ*/
#define IP "127.0.0.1"                              /*������IP��ַ*/

int main(int argc, char *argv[])
{
    int s;                                      /*sΪsocket������*/
    struct sockaddr_in server_addr;         /*��������ַ�ṹ*/

    s = socket(AF_INET, SOCK_STREAM, 0);        /*����һ����ʽ�׽��� */
    if(s < 0){                                  /*����*/
        printf("socket error\n");
        return -1;
    }

    /*���÷�������ַ*/
    bzero(&server_addr, sizeof(server_addr));   /*����*/
    server_addr.sin_family = AF_INET;                   /*Э����*/
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);    /*���ص�ַ*/
    server_addr.sin_port = htons(PORT);             /*�������˿�*/

    /*���û�������ַ������͵�IP��ַתΪ����*/
    inet_pton(AF_INET, IP, &server_addr.sin_addr);
    /*���ӷ�����*/
    connect(s, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
    //process_conn_client(s);                       /*�ͻ��˴������*/


    ssize_t size = 0;
    char buffer[1024];                          /*���ݵĻ�����*/

    for(;;){                                    /*ѭ���������*/
        /*�ӱ�׼�����ж�ȡ���ݷŵ�������buffer��*/
        size = read(0, buffer, 1024);

        if(size > 0){

	                           /*��������*/
            write(s, buffer,size);             /*���͸�������*/
            size = read(s, buffer, 1024);       /*�ӷ�������ȡ����*/
           // write(1, buffer, size);             /*д����׼���*/
	    write(1, buffer, size);
	  //  printf("%s\n",buffer);
        }
    }


    close(s);                                   /*�ر�����*/
    return 0;
}

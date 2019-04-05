//����socket
//ȷ��������scokaddr_in�ṹ��
//    ���ʮ����IPת��
//ʹ��bind���׽���
//ʹ��listen����
//ʹ��accept������������
//    accept�����µ��׽���������
//ʹ��recv���մ��������ݣ��ļ�·����
//���ļ���������Ҫ�ļ���
//    �Ӹ��ַ�����ȡ�ļ���
//ʹ��recv�����ļ�����
//    �ж�recv�������ص�״̬
//�����յ������ݷ��뻺����
//������������д���ļ�
//�ر��ļ�
//�ر�socket

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <errno.h>
#include <memory.h>
#include <stdlib.h>  //for malloc

#define MAXLINE 4096
#define BUFFER_SIZE 1024

int main()
{
    int sockfd;  //��������Socket��������
    int temp,n;
    struct sockaddr_in server,client; //��������ַ�ṹ�Ϳͻ��˵�ַ�ṹ
    char filename[100];  //�ļ���
    char filepath[100];   //�ļ�·��

    char *buffer;//file buffer  �ļ�������
    int fileTrans;

    buffer = (char *)malloc(sizeof(char)*BUFFER_SIZE);
    bzero(buffer,BUFFER_SIZE);
    //memset(buffer,0,sizeof(buffer));
    int lenfilepath;
    FILE *fp;
    int writelength;

	//����һ����ʽ�׽��� TCP����
    if((sockfd = socket(AF_INET,SOCK_STREAM,0))<0)
    {
        printf("socket build error!\n");
    }
    else
    {
        printf("socket build success!\n");
    }

	//���÷�������ַ
    memset(&server,0,sizeof(server));  //����  ���� bzero(&server,sizeof(server))
    server.sin_family= AF_INET;  //Э����
    server.sin_addr.s_addr = htonl(INADDR_ANY); //������ַ
    server.sin_port = htons(6666);  //�˿ں�


	//�󶨵�ַ�ṹ���׽���������
    if((bind(sockfd,(struct sockaddr*)&server,sizeof(server)))==-1)
    {
        printf("bind error!\n");
    }
    else
    {
        printf("bind success!\n");
    }

	//���ü���
    if(listen(sockfd,10)==-1)
    {
        printf("listen error!\n");
    }
    else
    {
        printf("listen success!\n");
    }
    printf("waiting..\n");
    while(1)
    {
        if((temp = accept(sockfd,(struct sockaddr*)NULL,NULL))>0)
        {
            //printf("client IP:   %d\n",ntohl(client.sin_addr.s_addr));
            //printf("client PORT: %d\n",ntohs(client.sin_port));

            memset(filename,'\0',sizeof(filename));
            memset(filepath,'\0',sizeof(filepath));

            lenfilepath = recv(temp,filepath,100,0);
            printf("filepath :%s\n",filepath);
            if(lenfilepath<0)
            {
                printf("recv error!\n");
            }
            else
            {
                int i=0,k=0;
                for(i=strlen(filepath);i>=0;i--)
                {
                    if(filepath[i]!='/')
                    {
                        k++;
                    }
                    else
                        break;
                }
                strcpy(filename,filepath+(strlen(filepath)-k)+1);
            }
            printf("filename :%s\n",filename);
            fp = fopen(filename,"w");
            if(fp!=NULL)
            {
                while(fileTrans =recv(temp,buffer,BUFFER_SIZE,0))
                {
                    if(fileTrans<0)
                    {
                        printf("recv error!\n");
                        break;
                    }
                    writelength = fwrite(buffer,sizeof(char),fileTrans,fp);
                    if(writelength <fileTrans)
                    {
                        printf("write error!\n");
                        break;
                    }
                    bzero(buffer,BUFFER_SIZE);
                    //memset(buffer,0,sizeof(buffer));
                }
                printf("recv finished!\n");
                fclose(fp);
            }
            else
            {
                printf("filename is null!\n");

            }
            close(temp);


        }
        else
        {
            printf("accept error!\n");
            close(temp);
        }

    }

    close(sockfd);

    return 0;

}

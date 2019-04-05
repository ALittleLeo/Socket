// ��windows�ϱ����#define WIN
#define WIN
#ifdef WIN
#include <winsock2.h>
//#pragma comment( lib, "ws2_32.lib" )
#else
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<errno.h>
#include<string.h>
#include <fcntl.h>
#define SOCKET int
#define INVALID_SOCKET -1
#endif

#define ERRORCODE -1
#include<stdio.h>
#define BUFFSIZE 10240

void get_filename(char *name, char *filename)
{
	int len;
	char *tmp = NULL;
	if (name == NULL)
		return;
	len = strlen(name);
	tmp = name + len - 1; //ָ���ַ���ĩ��
	while (tmp != name && *tmp != '\\' && *tmp != '/')
	{
		tmp--;
	} // ���ܸ���·���µ��ļ�
	if(tmp == name)
	{
		strcpy(filename, name);
	}
	else
	{
		strcpy(filename, tmp + 1);
	}
}
int init_socket()
{
#ifdef WIN
    WSADATA wsaData;
    int Ret;
   // Initialize Winsock version 2.2
	if ((Ret = WSAStartup(MAKEWORD(2,2), &wsaData)) != 0)
	{
		printf("WSAStartup failed with error %d\n", Ret);
		if (WSACleanup() == SOCKET_ERROR)
		{
			printf("WSACleanup failed with error %d\n", WSAGetLastError());
		}
		return ERRORCODE;
	}
   // When your application is finished call WSACleanup
#endif
	return 0;
}
SOCKET socket_connect(char *ip_str, int i_port)
{
	SOCKET st;
	struct sockaddr_in client_sockaddr; //����IP��ַ�ṹ
	int i_connect_rv;
	if (init_socket() == -1)
	{
		printf("init socket fail! \n");
		return INVALID_SOCKET;
	}
	st = socket(AF_INET, SOCK_STREAM, 0); //����socket
	if (st == INVALID_SOCKET)
	{
		printf("%s,%d:create socket error!",__FILE__,__LINE__);
		return INVALID_SOCKET;
	}

	memset(&client_sockaddr, 0, sizeof(client_sockaddr));
	client_sockaddr.sin_port = htons(i_port); //ָ��һ���˿ںŲ���hosts�ֽ��ʹ�����Inet���ֽ��ͣ���˻����С�����⣩
	client_sockaddr.sin_family = AF_INET;	//���ýṹ����ΪTCP/IP
	client_sockaddr.sin_addr.s_addr = inet_addr(ip_str);
	i_connect_rv = connect(st, (struct sockaddr*) &client_sockaddr,
			sizeof(client_sockaddr));
	if (i_connect_rv == -1)
	{
		printf("%s,%d:connect error!",__FILE__,__LINE__);
		return  INVALID_SOCKET;
	}

	return st;
}
int send_work(char *ip_str, int i_port, char *filename)
{
	int rv =0;
	FILE *fp = NULL ;
	char buf[BUFFSIZE];
	SOCKET st = socket_connect(ip_str, i_port);
	if (st == INVALID_SOCKET)
	{
		return ERRORCODE;
	}
	printf("connect success,start send file! \n");


	memset(buf, 0, BUFFSIZE);

	get_filename(filename, buf);

	fp = fopen(filename, "rb");//����û�п����ļ�����ͬ�����
	if(fp ==NULL)
	{
		printf("%s,%d:open file error! \n",__FILE__,__LINE__);
		return ERRORCODE;
	}

	printf("send file: %s..... \n", buf); //��ӡ���ļ���
	/* ���ļ���������������Ȼ��ȴ��������������ڷ����ļ����ݣ�
	����ͻ��˷��͵��ļ������ļ����ݻ���һ���޷����� */
	rv = send(st, buf, strlen(buf), 0);

	if (rv <= 0)
	{
		printf("%s,%d:send filename error! \n",__FILE__,__LINE__);
		return ERRORCODE;
	}
	else
    {
        printf("send filename success!!\n");
    }
	memset(buf, 0, BUFFSIZE);
	rv = recv(st, buf, BUFFSIZE, 0);//���շ������ķ��� ��OK
	if (rv <= 0)
	{
		printf("%s,%d:recv error! \n",__FILE__,__LINE__);
		return ERRORCODE;
	}
	else
    {
        printf("�ͻ����յ�����˽����ļ����ɹ��ķ�����\n");
    }
	//�յ����Է���˵�ok ��ʼ�����ļ�����


	printf("start send file....\n");
	if (strncmp(buf, "OK", 2) == 0)
	{
		while (1)
		{
			memset(buf, 0, BUFFSIZE);
			rv = fread(buf, 1, BUFFSIZE, fp);
			if (rv <= 0)// ����0˵���ļ�������
			{
				if (rv < 0)
				{
					printf("%s,%d:fread file error! \n",__FILE__,__LINE__);
					return ERRORCODE;
				}
				break;
			}
			rv = send(st, buf, rv, 0);
			if (rv <= 0)
			{
				printf("%s,%d:send error! \n",__FILE__,__LINE__);
				return ERRORCODE;
			}
		}
	}
// �ͷ���Դ
#ifdef WIN
	closesocket(st);
	if (WSACleanup() == SOCKET_ERROR)
	{
		printf("WSACleanup failed with error %d\n", WSAGetLastError());
		return ERRORCODE;
	}

#else
	close(st);
#endif
	if (fp)
	{
		fclose(fp);
	}
	return 0;
}

SOCKET create_listensocket(int i_port)
{
	SOCKET listen_socket;
	struct sockaddr_in sockaddr; //����IP��ַ�ṹ
	//struct sockaddr_in clinet_sockaddr; //����client IP��ַ�ṹ
	if (init_socket() == -1)
	{
		printf("%s,%d:init socket error!",__FILE__,__LINE__);
		return INVALID_SOCKET;
	}

	listen_socket = socket(AF_INET, SOCK_STREAM, 0);  //��ʼ��socket
	if (listen_socket == INVALID_SOCKET)
	{
		printf("%s,%d:socket create error!",__FILE__,__LINE__);
		return INVALID_SOCKET;
	}

	memset(&sockaddr, 0, sizeof(sockaddr));

	sockaddr.sin_port = htons(i_port); //ָ��һ���˿ںŲ���hosts�ֽ��ʹ�����Inet���ֽ��ͣ���˻����С�����⣩
	sockaddr.sin_family = AF_INET;	//���ýṹ����ΪTCP/IP
	sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);	//������ǵȴ���������������Ҫ��˭��ip
	//����дһ������INADDR_ANY��ʾserver������ip�����һ��server�����ж��ip��ַ����Ϊ�����ж������
	if (bind(listen_socket, (struct sockaddr *) &sockaddr, sizeof(sockaddr))
			== -1)
	{
		printf("%s,%d:recv error!",__FILE__,__LINE__);
		return INVALID_SOCKET; //
	}
	if(listen(listen_socket,10)== -1) //bind ��listen����
	{
		printf("%s,%d:listen error! \n",__FILE__,__LINE__);
		return INVALID_SOCKET;
	}
	return listen_socket;
}

SOCKET accept_socket(SOCKET listen_socket) // ��ȡ�������Ŀͻ���
{
	struct sockaddr_in clinet_sockaddr;
	SOCKET client_socket;
	//windows ��linx ��accept�������һ��������Щ����
    //����ʹ������������
#ifdef WIN
	int len = 0;
#else
	socklen_t len = 0;
#endif
	len = sizeof(struct sockaddr);
	memset(&clinet_sockaddr, 0, sizeof(clinet_sockaddr));
	client_socket = accept(listen_socket,
			(struct sockaddr*) &clinet_sockaddr, &len);
	if (client_socket == INVALID_SOCKET)
	{
		printf("%s,%d:accpet error! \n",__FILE__,__LINE__);
		return INVALID_SOCKET;
	}
	return client_socket;
}
int recv_work(int i_port) // ����˽����ļ�
{
	char buf[BUFFSIZE];
	FILE *fp = NULL;
	int rv = 0;
	SOCKET client_socket ;
	SOCKET listen_socket = create_listensocket(i_port);

	if (listen_socket == INVALID_SOCKET)
	{
		return ERRORCODE;
	}
	printf("listen success port: %d \n",i_port);

	while(1)
    {
        client_socket = accept_socket(listen_socket);
        if (client_socket == INVALID_SOCKET)
        {
            return ERRORCODE;
        }

        memset(buf, 0, BUFFSIZE);
        rv = recv(client_socket, buf, BUFFSIZE, 0);//��ȡ�ļ���
        if (rv <= 0)
        {
            printf("%s,%d:recv error! \n",__FILE__,__LINE__);
            return ERRORCODE;
        }
        fp = fopen(buf, "wb");
        if(fp == NULL)
        {
            printf("%s,%d:fopen file error! \n",__FILE__,__LINE__);
            return ERRORCODE;
        }
        printf("accpet file: %s \n",buf);
        memset(buf, 0, BUFFSIZE);
        strcpy(buf, "OK");
        rv = send(client_socket, buf, strlen(buf), 0);//�ظ�OK �����ļ����Ѿ��յ�
        if (rv <= 0)
        {
            printf("%s,%d:send error! \n",__FILE__,__LINE__);
            return  ERRORCODE;
        }
        else
        {
            printf("receive filename success, send back success!\n");
        }

        printf("start receive file......\n");
        while (1) // �����ļ�
        {
            printf("ѭ�������ļ�����\n");
            memset(buf, 0, BUFFSIZE);
            rv = recv(client_socket, buf, BUFFSIZE, 0);//�����ļ�����
            if (rv <= 0)
            {
                printf("read over!!\n");
                if (rv < 0)
                {
                    printf("%s,%d:recv error!\n",__FILE__,__LINE__);
                    return  ERRORCODE;
                }

                break;
            }
            rv = fwrite(buf, 1, rv, fp);//д���ļ�
            printf("%s  ",buf);
            if (rv <= 0)
            {
                printf("%s,%d:file write error!",__FILE__,__LINE__);
                return ERRORCODE;
            }
            else
                printf("file write\n");
        }
#ifdef WIN
        closesocket(client_socket);
        printf("close client_socket.\n");
        if (fp)
        {
            fclose(fp);
        }

#else
        close(client_socket);
#endif // WIN
	}
#ifdef WIN
	closesocket(listen_socket);
	if (WSACleanup() == SOCKET_ERROR)
	{
		printf("WSACleanup failed with error %d\n", WSAGetLastError());
		return ERRORCODE;
	}
#else
	close(listen_socket);
#endif
	if (fp)
	{
		fclose(fp);
	}
	return 0;
}

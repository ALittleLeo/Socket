#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <errno.h>
#include <memory.h>
#include <stdlib.h>  //for malloc
#define IP "192.168.134.148"				//��һ̨������ϲ��ԣ�IPΪ���ػ��͵�ַ
#define PORT 6000					//ע�⣺�ͻ�������ͨ�ŵĶ˿� = ����˵Ķ˿�
#define BUFFER_SIZE 1024			//���ݷ��ͻ�������С

#define LISTENQ 20
#define BUFFSIZE 4096
#define FILE_NAME_MAX_SIZE 512


void recvTCP(int clientfd){
	//recv file imformation
	char buff[BUFFSIZE];
	char filename[FILE_NAME_MAX_SIZE];
	int count;
	//bzero(buff,BUFFSIZE);��/
	memset(buff,0,BUFFSIZE);
	memset(filename,0,FILE_NAME_MAX_SIZE);

	count=recv(clientfd,buff,BUFFSIZE,0);
	if(count<0)
	{
		perror("recv");
		exit(1);
	}
	strncpy(filename,buff,strlen(buff)>FILE_NAME_MAX_SIZE?FILE_NAME_MAX_SIZE:strlen(buff));

	printf("Preparing recv file : %s  \n",filename);


	//recv file
	FILE *fd=fopen(filename,"wb+");
	if(NULL==fd)
	{
		perror("open");
		exit(1);
	}
	//bzero(buff,BUFFSIZE);
	memset(buff,0,BUFFSIZE);

	int length=0;
	int i=0;
	while(length=recv(clientfd,buff,BUFFSIZE,0))
	{
	    i++;
		if(length<0)
		{
			perror("recv");
			exit(1);
		}
        if(i%25600==0)
		printf("recving:100MB\n");
		int writelen=fwrite(buff,sizeof(char),length,fd);
		if(writelen<length)
		{
			perror("write");
			exit(1);
		}
		//bzero(buff,BUFFSIZE);
		memset(buff,0,BUFFSIZE);
	}
	printf("Receieved file:%s  finished!\n",filename);
	fclose(fd);
}


int connectTCP(const char *host, const char *port){
       WSADATA WSAData;
		if(WSAStartup(MAKEWORD(2,0),&WSAData)==SOCKET_ERROR)  //WSAStartup()������Winsock DLL���г�ʼ��
		{
			printf("Socket initialize fail!\n");
			//continue;
		}
		SOCKET sock;											//�ͻ��˽��̴����׽���
		if((sock=socket(AF_INET,SOCK_STREAM,0))==SOCKET_ERROR)  //�������׽��֣������˱���һ�£�
		{
			printf("Socket create fail!\n");
			WSACleanup();
			//continue;
		}

		struct sockaddr_in ClientAddr;				//sockaddr_in�ṹ������ʶTCP/IPЭ���µĵ�ַ����ǿ��ת��Ϊsockaddr�ṹ
		ClientAddr.sin_family=AF_INET;				//ָInternet��
		ClientAddr.sin_port=htons(PORT);			//ָ���������Ԥ���Ķ˿�
		ClientAddr.sin_addr.s_addr=inet_addr(IP);	//ָ����������󶨵�IP��ַ
		if(connect(sock,(LPSOCKADDR)&ClientAddr,sizeof(ClientAddr))==SOCKET_ERROR)  //����connect()����������������̷�����������
		{
			printf("Connect fail!\n");
			closesocket(sock);
			WSACleanup();
			//continue;
		}
		return sock;
}


int main()
{
    SOCKET sock;											//�ͻ��˽��̴����׽���
	char buf[BUFFER_SIZE];								//buf�����ſͻ��˷��͵���Ϣ
	int inputLen;										//���������ַ���������
	while(1)
	{
		printf("Socket\\Client>");
		inputLen=0;
		memset(buf,0,sizeof(buf));
		while((buf[inputLen++]=getchar())!='\n')		//�����Իس���Ϊ������ʶ
		{
			;
		}
		if(buf[0]=='e' && buf[1]=='x' && buf[2]=='i' && buf[3]=='t')
		{
			printf("The End.\n");
			break;
		}

        sock=connectTCP(IP,PORT);

		//send(sock,buf,BUFFER_SIZE,0);				 //���������������

		recvTCP(sock);



		closesocket(sock);							 //�ر��׽���
		WSACleanup();								//��ֹ��Winsock DLL��ʹ�ã����ͷ���Դ���Ա���һ��ʹ��
	}
	return 0;
}

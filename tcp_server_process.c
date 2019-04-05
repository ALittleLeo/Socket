#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define PORT 8888						/*�����˿ڵ�ַ*/
#define BACKLOG 2						/*�������г���*/

void process_conn_server(int);

int main(int argc, char *argv[])
{
	int ss,sc;		/*ssΪ��������socket��������scΪ�ͻ��˵�socket������*/
	struct sockaddr_in server_addr;	/*��������ַ�ṹ*/
	struct sockaddr_in client_addr;	/*�ͻ��˵�ַ�ṹ*/
	int err;							/*����ֵ*/

	/*����һ����ʽ�׽���*/
	ss = socket(AF_INET, SOCK_STREAM, 0);
	if(ss < 0){							/*����*/
		printf("socket error\n");
		return -1;
	}
	/*���÷�������ַ*/
	bzero(&server_addr, sizeof(server_addr));			/*����*/
	server_addr.sin_family = AF_INET;					/*Э����*/
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);	/*���ص�ַ*/
	server_addr.sin_port = htons(PORT);				/*�������˿�*/
	/*�󶨵�ַ�ṹ���׽���������*/
	err = bind(ss, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(err < 0){/*����*/
		printf("bind error\n");
		return -1;
	}
	/*��������*/
	err = listen(ss, BACKLOG);
	if(err < 0){										/*����*/
		printf("listen error\n");
		return -1;
	}

	printf("Wait link...\n");
   	/*��ѭ������*/
	for(;;)	{
		socklen_t addrlen = sizeof(struct sockaddr);
		sc = accept(ss, (struct sockaddr*)&client_addr, &addrlen);
		/*���տͻ�������*/
		if(sc < 0){							/*����*/
			continue;						/*��������ѭ��*/
		}

		/*����һ���µĽ��̴�����������*/
		pid_t pid = fork();						/*�ֲ����*/
		if( pid == 0 ){						/*�ӽ�����*/
			process_conn_server(sc);		/*��������*/
			close(ss);						/*���ӽ����йرշ�����������*/
			return 0;
		}else{
			close(sc);						/*�ڸ������йرտͻ��˵�����*/
		}
	}
}

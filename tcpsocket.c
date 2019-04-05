#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>


#define BUFFSIZE 100

int main()
{
    int listenFd, connectFd;
    socklen_t len;
    int n;
    struct sockaddr_in srcAddr, cliAddr;
    char buf[BUFFSIZE];

    //create socket
//    ����ԭ��
//    int socket(int family,int type,int protocol); ������
//  ���� �����أ��Ǹ������֣������ɹ�������-1������ʧ��
//��һ������ָ����Э��أ�Ŀǰ֧��5��Э��أ���õ���AF_INET(IPv4Э��)��AF_INET6(IPv6Э��)��
//�ڶ�������ָ���׽ӿ����ͣ����������Ϳ�ѡ��SOCK_STREAM(�ֽ����׽ӿڣ� TCP)��SOCK_DGRAM(���ݱ��׽ӿڣ�UDP)
//��SOCK_RAW(ԭʼ�׽ӿ�)������׽ӿ����Ͳ���ԭʼ�׽ӿڣ���ô������������Ϊ0��


    if((listenFd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket error");
        exit(1);
    }

    bzero(&srcAddr, sizeof(srcAddr));
    srcAddr.sin_family = AF_INET;
    srcAddr.sin_port = htons(8888);
    srcAddr.sin_addr.s_addr = htonl(INADDR_ANY);
//    ͨ���ַʵ�֣�htonl(INADDR_ANY)
//    ͨ���ַ���ں˽��ȵ��׽���������TCP���Ѿ��������ݱ�(UDP)ʱ��ָ����

    //���׽���
    if(bind(listenFd, (struct sockaddr*)&srcAddr, sizeof(srcAddr)) == -1)
    {
        perror("bind error");
        exit(1);
    }

    //�����׽���
    if(listen(listenFd, 5) == -1)
    {
        perror("listen error");
        exit(1);
    }

    len = sizeof(cliAddr);

    printf("listening ...\n");

    while(1)
    {
        //������ɶ�����ץȡһ���׽��ֽ��з���
//        accept��������������ӵĶ�����ȡ��һ���׽��֣�����ö���Ϊ�գ���accept����������
//        accept�����ķ���ֵ��Ϊ�������׽��֣������ӵ��׽��־ͽ���һ��������TCP���ӣ�
//        ԴIP��ַ��Դ�˿ںţ�Ŀ��IP��ַ��Ŀ�Ķ˿ںŶ���Ψһȷ���ˡ�
        if((connectFd = accept(listenFd, (struct sockaddr*)&cliAddr, &len)) == -1)
        {
            perror("accept error");
            exit(1);
        }

        printf("accepted successful\n");

        //���׽����ж�ȡ�ӿͻ��˷���������
        while((n = read(connectFd, buf, BUFFSIZE)) > 0)
        {
            if(buf[n] != '\0')
            {
                buf[n] = '\0';
            }
            printf("receive a message: %s", buf);

            //����ȡ������д���׽��֣������ͻ���
            if(write(connectFd, buf, n) < 0)
            {
                perror("write error");
                exit(1);
            }
        }
    }

    //�����˳������д򿪵��ļ����������ᱻ�رգ���˴򿪵��׽����ļ�Ҳ���ر�
    return 0;
}

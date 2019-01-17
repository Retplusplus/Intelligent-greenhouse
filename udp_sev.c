#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
int main()
{
	//创建套接字
	//为套接字绑定地址信息
	//接收数据
	//发送数据
	//关闭套接字
	int sockfd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(sockfd<0)
	{
		perror("socket err");
		return -1;
	}
	
	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(9000);
	addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	//addr.sin_addr.s_addr=htonl(0xc0a87a87);
	socklen_t len=sizeof(struct sockaddr_in);
	int ret=bind(sockfd,(struct sockaddr*)&addr,len);
	if(ret<0)
	{
		perror("bind error");
		return -1;
	}

	while(1)
	{
	char buff[1024]={0};
	struct sockaddr_in cli_addr;
	recvfrom(sockfd,buff,1023,0,(struct sockaddr*)&cli_addr,&len);
	printf("client say:%s\n",buff);

	memset(buff,0x00,1024);
	scanf("%s",buff);
	sendto(sockfd,buff,strlen(buff),0,(struct sockaddr*)&cli_addr,len);
	printf("server say:\n");
	}
	close(sockfd);

	return 0;
}


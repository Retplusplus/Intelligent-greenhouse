#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<string>
#include<arpa/inet.h>
class UdpSocket
{
private:
	int _sockfd;
public:
	UdpSocket():_sockfd(-1){}
	bool Socket()
	{
		_sockfd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
		if(_sockfd<0)
		{
			perror("socketerr");
			return false;
		}
		return true;
	}
	bool Bind(std::string& ip,uint16_t port)
	{
		sockaddr_in addr;
		addr.sin_family=AF_INET;
		addr.sin_port=htons(port);
		addr.sin_addr.s_addr=inet_addr(ip.c_str());
		socklen_t len=sizeof(sockaddr_in);
		int ret=bind(_sockfd,(sockaddr*)&addr,len);
		if(ret<0)
		{
			perror("bind error");
			return false;
		}
		return true;
	}
		

	ssize_t Recv(char *buf,sockaddr_in *addr=NULL)
	{
		sockaddr_in _addr;
		ssize_t rlen;
		socklen_t len=sizeof(sockaddr_in);
		rlen=recvfrom(_sockfd,buf,1024,0,(sockaddr*)&_addr,&len);
		if(rlen<0)
		{
			perror("recvfrom error");
			return -1;
		}
		if(addr)
		{
			memcpy((void*)addr,(void*)&_addr,len);
		}
		return rlen;
	}

	bool send(char *buf,size_t len,sockaddr_in &addr)
	{
		int slen=0;
		slen=sendto(_sockfd,buf,len,0,(sockaddr*)&addr,sizeof(sockaddr));
		if(slen<0);
		{
			perror("sendto error");
			return false;
		}
		return true;
	}


	bool Close()
	{
		if(_sockfd>=0)
		{
			close(_sockfd);
			_sockfd=-1;
		}
		return true;
	}
};

#define CHECK_RET(q) if(q==false) {return -1;}
int main()
{
	UdpSocket sock;
	CHECK_RET(sock.Socket());
	sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(9000);
	addr.sin_addr.s_addr=inet_addr("127.0.0.1");

#if 0
	//发送视频
	int fd = open("./***.rmvb", O_WRONLY);//打开文件，只读方式打开
	while(1) {
		int ret = read(fd, buf, 1472);
		if (ret == 0) {//read返回值等于0发送完了，小于0则出错
			break;
		}else if (ret < 0) {
			return -1;
		}
		sock.Send(buf, ret, &addr);
	}
	while(1) {//接收视频
		int ret = sock.Recv(buf, 1472);
		if (ret <= 0) {
			return -1;
		}
		write(fd, buf, ret);
	}
#endif
	while(1)
	{
		char buff[1024]={0};
		scanf("%[^\n]",buff);
		sock.send(buff,strlen(buff),addr);
		printf("client say:%s\n",buff);

		memset(buff,0x00,1024);
		sock.Recv(buff);
		printf("recv buff:%s\n",buff);
		
	}
	sock.Close();
}
	





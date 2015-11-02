#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>

#define PORT 8888
#define SERVER_ADDR localhost
void get_print(int *sockfd);

int main()
{
	int sockfd;
	int len;
	struct sockaddr_in address;
	int result;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = PORT;
	len = sizeof(address);
	result = connect(sockfd, (struct sockaddr *)&address,len);
	pthread_t pt;
	pthread_create(&pt,NULL,(void *)get_print,(void *)&sockfd);
	if(result == -1)
	{
		printf("connect failed!\n");
		return 1;
	}else{
		printf("connect successed!\n");
	}
	char buffer[1024];
	int hasWrite;
	while(1)
	{
		fgets(buffer,1024,stdin);
		hasWrite = write(sockfd,buffer,strlen(buffer));
		if(hasWrite < 0)
		{
			break;
		}
		printf("hasWrite --%d\n",hasWrite);
	}
	printf("程序运行完毕\n");
}
void get_print(int *sockfd)
{
	char buffer[1024];
	int hasRead;
	while(1)
	{
		hasRead = read(*sockfd,buffer,1024);
		if(hasRead < 0)
		{
			printf("从客户端读取失败,程序结束运行\n");
			break;
		}
		printf("%s",buffer);
		memset(buffer,0,1024);
	}
}

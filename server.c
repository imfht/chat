#include<sys/types.h>
#include<sys/socket.h>
#include <stdlib.h>
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#define PORT 8888
struct client_node
{
    struct sockaddr_in client_address;
    int ID;
    char usrName[1024];
    struct client_node* next;
};
void send_to_all(struct client_node*);
void get_out(int sockfd);
struct client_node* phead;
int main()
{
    phead = malloc(sizeof(phead));
	int server_sockfd,client_sockfd;
	int server_len, client_len;
	struct sockaddr_in  server_address;
	server_sockfd = socket(AF_INET, SOCK_STREAM,0);//定义套接字类型
	server_address .sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_address.sin_port = PORT;
	server_len = sizeof(server_address);
    client_len = server_len;
	if(bind(server_sockfd,(struct sockaddr *)&server_address, server_len) < 0)
	{
		printf("bind error");
        return 0;
	}
    phead = malloc(sizeof(phead));
    struct client_node* pthis = phead; 
    pthread_t pt;
	while(1)
	{
		listen(server_sockfd,5);
        printf("listening\n");
        pthis->next = malloc(sizeof(struct client_node));
        pthis = pthis->next;
		pthis->ID = accept(server_sockfd,(struct sockaddr *)&(pthis->client_address),(socklen_t *__restrict)&client_len);
        /*看pt数组是否有设定的必要*/
        pthis->next = NULL;
		pthread_create(&pt,NULL,(void *)send_to_all,&pthis);
        printf("玩家%d连接成功\n",pthis->ID);
	}
	printf("client connect successed!\n ");
}

int remove_node(int sockfd)
{
    struct client_node* p1 = phead;
    struct client_node* for_free;
    while(p1->next!=NULL)
    {
        if(p1->next->ID == sockfd)
        {
            for_free = p1->next;
            free(for_free);/*清理内存,完善内存管理机制*/
            p1->next = p1->next->next;
            return 1;
        }
        p1 = p1->next;
    }
    return 0;
}


void enterSetting(struct client_node* pclientNode)
{
    char buffer[1024]={"进入设置模式,版本1,0只支持更改你的聊天马甲\n现在输入你的(新)马甲"};
    int hasRead;
    write(pclientNode->ID,buffer,strlen(buffer));
    memset(buffer,0,1024);
    hasRead = read(pclientNode->ID,buffer,1024);
    strncpy(pclientNode->usrName,buffer,1024);
    printf("马甲适配完毕\n");
}
/*every client use a thread */
void send_to_all(struct client_node* pclientNode)
{
    int *sockfd = &(pclientNode->ID);
	char buffer[1024];
	int i,hasRead;
    struct client_node* p1 = phead;
	while(1)
	{
		hasRead = read(*sockfd,buffer,1024);
	//	if(hasRead <= 0)
		//{
          //  if(remove_node(*sockfd))
                printf("玩家%d掉线,移出群聊\n",*sockfd);
   //         else 
     //           printf("玩家掉线,移出失败<没有从链表中找到该元素>\n");
		//	break;
		//}
        if(buffer[0] == '#')
        {
            enterSetting(pclientNode);
        }
        while(1)
        {
            if(p1==NULL)
            {
                printf("NULL");
                break;
            }
            p1 = p1->next;
            printf("读到了尝试写出去\n");
            write(p1->ID,p1->usrName,strlen(p1->usrName));
			write(p1->ID,buffer,hasRead);
            if(p1->next == NULL)
            {
                break;
            }
        }
        printf("here\n");
        p1 = phead;
	}
		memset(buffer,0,1024);
}

#include <netinet/in.h> // sockaddr_in
#include <netinet/tcp.h>
#include <unistd.h>
#include <sys/types.h>  // socket
#include <sys/socket.h> // socket
#include <stdio.h>    // printf
#include <stdlib.h>   // exit
#include <string.h>   // bzero
#include <pthread.h>

#define SERVER_PORT 5679

#define LENGTH_OF_LISTEN_QUEUE 20
#define BUFFER_SIZE 1024
#define FILE_NAME_MAX_SIZE 512
char send_buffer[BUFFER_SIZE];
char send_msg[FILE_NAME_MAX_SIZE + 1] = "\0";
char send_name[FILE_NAME_MAX_SIZE];
char recv_buffer[BUFFER_SIZE];
char recv_msg[FILE_NAME_MAX_SIZE + 1] = "\0";
char recv_name[FILE_NAME_MAX_SIZE];
struct sockaddr_in server_addr;
struct sockaddr_in client_addr;
int server_socket_fd = 0;

typedef struct MySocketInfo {
	int socketCon;
	char *ipaddr;
	uint16_t port;
}_MySocketInfo;

// 客户端数组
struct MySocketInfo arrConSocket[10];
int conClientCount = 0;

// 接受客户端线程列表
pthread_t arrThrReceiveClient[10];
int thrReceiveClientCount = 0;


void send_file(char* file_name, int socket);
void recv_file(char* file_name, int sockfd);
int begain_with(char *str1, char *str2);
void connection();
int senddata(int sockfd, char* send_msg);
void* recvdata(int count);
void *fun_thrAcceptHandler(void *socketListen);

int main(void)
{
	
	connection();

		// 定义客户端的socket地址结构
		socklen_t client_addr_length = sizeof(client_addr);

		// 接受连接请求，返回一个新的socket(描述符)，这个新socket用于同连接的客户端通信
		// accept函数会把连接到的客户端信息写到client_addr中

		pthread_t thrAccept;
		pthread_create(&thrAccept, NULL, fun_thrAcceptHandler, &server_socket_fd);//创建多线程


		/* 实时发送数据 */
		while (1) {

			//判断线程存活多少
			printf(" thread number: %d\n", thrReceiveClientCount);

			//输入消息，分发给客户机
			while (1)
			{
				bzero(send_msg, FILE_NAME_MAX_SIZE + 1);
				scanf("%s", send_msg);

				for (int i = 0;i < conClientCount;i++)
				{
					senddata(arrConSocket[10].socketCon, send_msg);
				}
			}

			sleep(0.5);
		}

		// 等待子进程退出
		printf(" waiting child thread to quit\n");
		char *message;
		pthread_join(thrAccept, (void *)&message);
		printf("%s\n", message);

		return 0;
}



void send_file(char* file_name, int socket)
{
	// 打开文件并读取文件数据
	FILE *fp = fopen(file_name, "r");
	if (NULL == fp)
	{
		printf(" File:%s Not Found\n", file_name);
	}
	else
	{
		bzero(recv_buffer, BUFFER_SIZE);
		int length = 0;
		// 每读取一段数据，便将其发送给客户端，循环直到文件读完为止
		while ((length = fread(recv_buffer, sizeof(char), BUFFER_SIZE, fp)) > 0)
		{
			if (send(socket, recv_buffer, length, 0) < 0)
			{
				printf(" Send File:%s Failed./n", file_name);
				break;
			}
			bzero(recv_buffer, BUFFER_SIZE);
		}

		// 关闭文件
		fclose(fp);
		printf(" File:%s Transfer Successful!\n", file_name);
	}
}

void recv_file(char* file_name, int sockfd)
{	// 打开文件，准备写入 
	FILE *fp = fopen(file_name, "w");
	if (NULL == fp)
	{
		printf(" File:%s Can Not Open To Write\n", file_name);
		exit(1);
	}

	// 从服务器接收数据到recv_buffer中 
	// 每接收一段数据，便将其写入文件中，循环直到文件接收完并写完为止 
	bzero(recv_buffer, BUFFER_SIZE);
	int length = 0;
	while ((length = recv(sockfd, recv_buffer, BUFFER_SIZE, 0)) > 0)
	{
		if (fwrite(recv_buffer, sizeof(char), length, fp) < length)
		{
			printf(" File:\t%s Write Failed\n", file_name);
			break;
		}
		bzero(recv_buffer, BUFFER_SIZE);
	}

	// 接收成功后，关闭文件
	printf(" Receive File:\t%s From Server IP Successful\n", file_name);
	fclose(fp);
}

int begain_with(char *str1, char *str2)
{
	if (str1 == NULL || str2 == NULL)
		return -1;
	int len1 = strlen(str1);
	int len2 = strlen(str2);
	if ((len1 < len2) || (len1 == 0 || len2 == 0))
		return -1;
	char *p = str2;
	int i = 0;
	while (*p != '\0')
	{
		if (*p != str1[i])
			return 0;
		p++;
		i++;
	}
	return 1;
}

void connection()
{

	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	server_addr.sin_port = htons(SERVER_PORT);

	// 创建socket，若成功，返回socket描述符
	server_socket_fd = socket(PF_INET, SOCK_STREAM, 0);
	if (server_socket_fd < 0)
	{
		perror(" Create Socket Failed:");
		exit(1);
	}
	int opt = 1;
	setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	// 绑定socket和socket地址结构
	if (-1 == (bind(server_socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))))
	{
		perror(" Server Bind Failed:");
		exit(1);
	}

	// socket监听
	if (-1 == (listen(server_socket_fd, LENGTH_OF_LISTEN_QUEUE)))
	{
		perror(" Server Listen Failed:");
		exit(1);
	}
}

int senddata(int sockfd,char* send_msg)
{
	    // 输入文件名 并放到缓冲区send_buffer中等待发送 
		bzero(send_buffer, BUFFER_SIZE);
		strncpy(send_buffer, send_msg, strlen(send_msg) > BUFFER_SIZE ? BUFFER_SIZE : strlen(send_msg));

		// 向客户机发送send_buffer中的数据 
		if (send(sockfd, send_buffer, BUFFER_SIZE, 0) < 0)
		{
			perror(" File Error:");
			exit(1);
		}

		if (begain_with(send_msg, "file:") == 1)
		{
			strncpy(send_name, send_msg + 5, FILE_NAME_MAX_SIZE - 4);
			send_file(send_name, sockfd);//send file
		}

}

void* recvdata(int sockfd)
{

	while (1)
	{
		// recv函数接收数据到缓冲区recv_buffer中
		bzero(recv_buffer, BUFFER_SIZE);
		if (recv(sockfd, recv_buffer, BUFFER_SIZE, MSG_WAITALL) < 0)
		{
			perror("Server Recieve Data Failed:");
			return((void*)0);
		}
		// 然后从recv_buffer(缓冲区)拷贝到recv_msg中
		bzero(recv_msg, FILE_NAME_MAX_SIZE + 1);
		strncpy(recv_msg, recv_buffer, strlen(recv_buffer) > FILE_NAME_MAX_SIZE ? FILE_NAME_MAX_SIZE : strlen(recv_buffer));
		
		printf("client %d: %s\n",sockfd,recv_msg);

		if (begain_with(recv_msg, "file:") == 1)
		{
			//recv files
			strncpy(recv_name, recv_msg + 5, FILE_NAME_MAX_SIZE - 4);
			recv_file(recv_name, sockfd);
		}

		// 关闭与客户端的连接
		if (!strcmp(recv_msg,"exit"))
		{
			printf("client %d exit", sockfd);
			return 0;
		}
	}
}

void *fun_thrAcceptHandler(void *socketListen) {
	while (1) {
		int sockaddr_in_size = sizeof(struct sockaddr_in);
		struct sockaddr_in client_addr;
		int _socketListen = *((int *)socketListen);
		int socketCon = accept(_socketListen, (struct sockaddr *)(&client_addr), (socklen_t *)(&sockaddr_in_size));
		if (socketCon < 0) {
			printf(" connect fail\n");
		}
		printf(" socket %d connect\n", socketCon);
		//开启新的通讯线程，负责同连接上来的客户端进行通讯
		_MySocketInfo socketInfo;
		socketInfo.socketCon = socketCon;
		socketInfo.ipaddr = inet_ntoa(client_addr.sin_addr);
		socketInfo.port = client_addr.sin_port;
		arrConSocket[conClientCount] = socketInfo;
		conClientCount++;
		printf(" client number: %d\n", conClientCount);
		pthread_t thrReceive = 0;
		pthread_create(&thrReceive, NULL, recvdata, socketCon);//创建收消息的子进程
		arrThrReceiveClient[thrReceiveClientCount] = thrReceive;
		thrReceiveClientCount++;
		//让进程休息1秒
		sleep(0.5);
	}
	char *s = " safe quit";
	pthread_exit(s);
}
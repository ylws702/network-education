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

// �ͻ�������
struct MySocketInfo arrConSocket[10];
int conClientCount = 0;

// ���ܿͻ����߳��б�
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

		// ����ͻ��˵�socket��ַ�ṹ
		socklen_t client_addr_length = sizeof(client_addr);

		// �����������󣬷���һ���µ�socket(������)�������socket����ͬ���ӵĿͻ���ͨ��
		// accept����������ӵ��Ŀͻ�����Ϣд��client_addr��

		pthread_t thrAccept;
		pthread_create(&thrAccept, NULL, fun_thrAcceptHandler, &server_socket_fd);//�������߳�


		/* ʵʱ�������� */
		while (1) {

			//�ж��̴߳�����
			printf(" thread number: %d\n", thrReceiveClientCount);

			//������Ϣ���ַ����ͻ���
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

		// �ȴ��ӽ����˳�
		printf(" waiting child thread to quit\n");
		char *message;
		pthread_join(thrAccept, (void *)&message);
		printf("%s\n", message);

		return 0;
}



void send_file(char* file_name, int socket)
{
	// ���ļ�����ȡ�ļ�����
	FILE *fp = fopen(file_name, "r");
	if (NULL == fp)
	{
		printf(" File:%s Not Found\n", file_name);
	}
	else
	{
		bzero(recv_buffer, BUFFER_SIZE);
		int length = 0;
		// ÿ��ȡһ�����ݣ��㽫�䷢�͸��ͻ��ˣ�ѭ��ֱ���ļ�����Ϊֹ
		while ((length = fread(recv_buffer, sizeof(char), BUFFER_SIZE, fp)) > 0)
		{
			if (send(socket, recv_buffer, length, 0) < 0)
			{
				printf(" Send File:%s Failed./n", file_name);
				break;
			}
			bzero(recv_buffer, BUFFER_SIZE);
		}

		// �ر��ļ�
		fclose(fp);
		printf(" File:%s Transfer Successful!\n", file_name);
	}
}

void recv_file(char* file_name, int sockfd)
{	// ���ļ���׼��д�� 
	FILE *fp = fopen(file_name, "w");
	if (NULL == fp)
	{
		printf(" File:%s Can Not Open To Write\n", file_name);
		exit(1);
	}

	// �ӷ������������ݵ�recv_buffer�� 
	// ÿ����һ�����ݣ��㽫��д���ļ��У�ѭ��ֱ���ļ������겢д��Ϊֹ 
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

	// ���ճɹ��󣬹ر��ļ�
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

	// ����socket�����ɹ�������socket������
	server_socket_fd = socket(PF_INET, SOCK_STREAM, 0);
	if (server_socket_fd < 0)
	{
		perror(" Create Socket Failed:");
		exit(1);
	}
	int opt = 1;
	setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	// ��socket��socket��ַ�ṹ
	if (-1 == (bind(server_socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))))
	{
		perror(" Server Bind Failed:");
		exit(1);
	}

	// socket����
	if (-1 == (listen(server_socket_fd, LENGTH_OF_LISTEN_QUEUE)))
	{
		perror(" Server Listen Failed:");
		exit(1);
	}
}

int senddata(int sockfd,char* send_msg)
{
	    // �����ļ��� ���ŵ�������send_buffer�еȴ����� 
		bzero(send_buffer, BUFFER_SIZE);
		strncpy(send_buffer, send_msg, strlen(send_msg) > BUFFER_SIZE ? BUFFER_SIZE : strlen(send_msg));

		// ��ͻ�������send_buffer�е����� 
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
		// recv�����������ݵ�������recv_buffer��
		bzero(recv_buffer, BUFFER_SIZE);
		if (recv(sockfd, recv_buffer, BUFFER_SIZE, MSG_WAITALL) < 0)
		{
			perror("Server Recieve Data Failed:");
			return((void*)0);
		}
		// Ȼ���recv_buffer(������)������recv_msg��
		bzero(recv_msg, FILE_NAME_MAX_SIZE + 1);
		strncpy(recv_msg, recv_buffer, strlen(recv_buffer) > FILE_NAME_MAX_SIZE ? FILE_NAME_MAX_SIZE : strlen(recv_buffer));
		
		printf("client %d: %s\n",sockfd,recv_msg);

		if (begain_with(recv_msg, "file:") == 1)
		{
			//recv files
			strncpy(recv_name, recv_msg + 5, FILE_NAME_MAX_SIZE - 4);
			recv_file(recv_name, sockfd);
		}

		// �ر���ͻ��˵�����
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
		//�����µ�ͨѶ�̣߳�����ͬ���������Ŀͻ��˽���ͨѶ
		_MySocketInfo socketInfo;
		socketInfo.socketCon = socketCon;
		socketInfo.ipaddr = inet_ntoa(client_addr.sin_addr);
		socketInfo.port = client_addr.sin_port;
		arrConSocket[conClientCount] = socketInfo;
		conClientCount++;
		printf(" client number: %d\n", conClientCount);
		pthread_t thrReceive = 0;
		pthread_create(&thrReceive, NULL, recvdata, socketCon);//��������Ϣ���ӽ���
		arrThrReceiveClient[thrReceiveClientCount] = thrReceive;
		thrReceiveClientCount++;
		//�ý�����Ϣ1��
		sleep(0.5);
	}
	char *s = " safe quit";
	pthread_exit(s);
}
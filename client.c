#include<netinet/in.h>  // sockaddr_in 
#include<unistd.h>
#include<sys/types.h>  // socket 
#include<sys/socket.h>  // socket 
#include<stdio.h>    // printf 
#include<stdlib.h>    // exit 
#include<string.h>    // bzero 
#include<arpa/inet.h>
#include<pthread.h>

#define SERVER_PORT 5679
#define BUFFER_SIZE 1024 
#define FILE_NAME_MAX_SIZE 512 
char send_buffer[BUFFER_SIZE];
char send_msg[FILE_NAME_MAX_SIZE + 1] = "\0";
char send_name[FILE_NAME_MAX_SIZE];
char recv_buffer[BUFFER_SIZE];
char recv_msg[FILE_NAME_MAX_SIZE + 1] = "\0";
char recv_name[FILE_NAME_MAX_SIZE];
struct sockaddr_in client_addr;
struct sockaddr_in server_addr;
int client_socket_fd = 0;
socklen_t server_addr_length = sizeof(server_addr);

void send_file(char* file_name, int socket);
void recv_file(char* file_name, int sockfd);
int begain_with(char* str1, char *str2);
void connection();
int senddata();
void* recvdata();

int main()
{

	connection();

	pthread_t thread_id;

	pthread_create(&thread_id, NULL, recvdata,NULL);  //�����߳�
	pthread_detach(thread_id); // �̷߳��룬����ʱ�Զ�������Դ

	senddata();

	close(client_socket_fd);

	return 0;
}


void send_file(char* file_name, int socket)
{
	// ���ļ�����ȡ�ļ�����
	FILE *fp = fopen(file_name, "r");
	if (NULL == fp)
	{
		printf("File:%s Not Found\n", file_name);
	}
	else
	{
		bzero(send_buffer, BUFFER_SIZE);
		int length = 0;
		// ÿ��ȡһ�����ݣ��㽫�䷢�͸��ͻ��ˣ�ѭ��ֱ���ļ�����Ϊֹ
		while ((length = fread(send_buffer, sizeof(char), BUFFER_SIZE, fp)) > 0)
		{
			if (send(socket, send_buffer, length, 0) < 0)
			{
				printf("Send File:%s Failed./n", file_name);
				break;
			}
			bzero(send_buffer, BUFFER_SIZE);
		}

		// �ر��ļ�
		fclose(fp);
		printf("File:%s Transfer Successful\n", file_name);
	}
}

void recv_file(char* file_name, int sockfd)
{	// ���ļ���׼��д�� 
	FILE *fp = fopen(file_name, "w");
	if (NULL == fp)
	{
		printf("File:%s Can Not Open To Write\n", file_name);
		exit(1);
	}

	// �ӷ������������ݵ�send_buffer�� 
	// ÿ����һ�����ݣ��㽫��д���ļ��У�ѭ��ֱ���ļ������겢д��Ϊֹ 
	bzero(send_buffer, BUFFER_SIZE);
	int length = 0;
	while ((length = recv(sockfd, send_buffer, BUFFER_SIZE, 0)) > 0)
	{
		if (fwrite(send_buffer, sizeof(char), length, fp) < length)
		{
			printf("File:\t%s Write Failed\n", file_name);
			break;
		}
		bzero(send_buffer, BUFFER_SIZE);
	}

	// ���ճɹ��󣬹ر��ļ�
	printf("Receive File:\t%s Successful\n", file_name);
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
	bzero(&client_addr, sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = htons(INADDR_ANY);
	client_addr.sin_port = htons(0);

	// ����socket�����ɹ�������socket������ 
	client_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (client_socket_fd < 0)
	{
		perror("Create Socket Failed:");
		exit(1);
	}

	// �󶨿ͻ��˵�socket�Ϳͻ��˵�socket��ַ�ṹ �Ǳ��� 
	if (-1 == (bind(client_socket_fd, (struct sockaddr*)&client_addr, sizeof(client_addr))))
	{
		perror("Client Bind Failed:");
		exit(1);
	}

	// ����һ���������˵�socket��ַ�ṹ�����÷������Ǳߵ�IP��ַ���˿ڶ�����г�ʼ�������ں�������� 
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) == 0)
	{
		perror("Server IP Address Error:");
		exit(1);
	}
	server_addr.sin_port = htons(SERVER_PORT);

	// ��������������ӣ����ӳɹ���client_socket_fd�����˿ͻ��˺ͷ�������һ��socket���� 
	if (connect(client_socket_fd, (struct sockaddr*)&server_addr, server_addr_length) < 0)
	{
		perror("Can Not Connect To Server IP:");
		exit(0);
	}
}

int senddata()
{
	while (1)
	{
		// �����ļ��� ���ŵ�������send_buffer�еȴ����� 
		bzero(send_msg, FILE_NAME_MAX_SIZE + 1);
		scanf("%s", send_msg);
		bzero(send_buffer, BUFFER_SIZE);
		strncpy(send_buffer, send_msg, strlen(send_msg) > BUFFER_SIZE ? BUFFER_SIZE : strlen(send_msg));

		// �����������send_buffer�е����� 
		if (send(client_socket_fd, send_buffer, BUFFER_SIZE, 0) < 0)
		{
			perror("File Error:");
			exit(1);
		}

		if (begain_with(send_msg, "file:") == 1)
		{
			strncpy(send_name, send_msg + 5, FILE_NAME_MAX_SIZE - 4);
			send_file(send_name, client_socket_fd);//send file
		}

		if (!strcmp(send_msg, "exit"))
		{
			puts("Connection closed.");
			close(client_socket_fd);
			return 0;
		}
	}

}

void* recvdata()
{
	while (1) {
		// recv�����������ݵ�������recv_buffer��
		bzero(recv_buffer, BUFFER_SIZE);
		if (recv(client_socket_fd, recv_buffer, BUFFER_SIZE, MSG_WAITALL) < 0)
		{
			perror("Server Recieve Data Failed:");
			return((void*)0);
		}
		// Ȼ���recv_buffer(������)������recv_msg��
		bzero(recv_msg, FILE_NAME_MAX_SIZE + 1);
		strncpy(recv_msg, recv_buffer, strlen(recv_buffer) > FILE_NAME_MAX_SIZE ? FILE_NAME_MAX_SIZE : strlen(recv_buffer));

		printf("server: %s\n", recv_msg);
		if (begain_with(recv_msg, "file:") == 1)
		{
			//send files
			strncpy(recv_name, recv_msg + 5, FILE_NAME_MAX_SIZE - 4);
			recv_file(recv_name, client_socket_fd);
		}
	}
	return((void*)0);
}

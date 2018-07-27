#include <netinet/in.h>         // for sockaddr_in
#include <sys/types.h>          // for socket
#include <sys/socket.h>         // for socket
#include <stdio.h>              // for printf
#include <stdlib.h>             // for exit
#include <string.h> 
#include <sys/stat.h>
#include <malloc.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "mysocket.h"
#define RES_LENGTH 1024

int connect_socket(char* server,int serverPort)
{
    //int serverPort=6666;
    int sockfd=0;
    struct sockaddr_in addr;
    struct hostent *phost;
    //向系统注册，通知系统建立一个通信端口
    //AF_INET表示使用IPV4协议
    //SOCK_STREAM表示使用TCP协议
    if( (sockfd=socket(AF_INET,SOCK_STREAM,0))<0 )
    {
	herror("Init socket erro\n");
	return -1;
    }
    bzero(&addr,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(serverPort);
    addr.sin_addr.s_addr=inet_addr(server);

    //如果输入的是域名
    if(addr.sin_addr.s_addr == INADDR_NONE )
    {
         phost= (struct hostent*)gethostbyname(server);
	 if(phost == NULL)
         {
	    herror("Init socket s_addr erro \n");
	    return -1;
          }
	 addr.sin_addr.s_addr=((struct in_addr*)phost->h_addr)->s_addr;
    }

    if( connect(sockfd,(struct sockaddr*)&addr,sizeof(addr))<0 )
    {
	return -1;
    }
    else
    {
	return sockfd;
    }

}
int send_msg(int sockfd,char* sendBuff,int length)
{
    int sendSize=0;
    if( (sendSize=send(sockfd,sendBuff,length,0))<=0 )
    {
       herror("Send msg erro\n");
       return -1;
    }
    else
    {
	return sendSize;
    }
}
int  recv_msg(int sockfd,char* buffer,int bufflength)
{
    int flag=0;
    int reclength=0;
    memset(buffer,0,bufflength);
    
    for(flag=0;;)
    {
       if( (reclength=recv(sockfd,buffer+flag,bufflength-flag,0))==-1 )
       {
	   herror("Recv msg erro\n");
	   return -1;
       }
       else if(reclength==0)
       {
	   break;
       }
       else
       {
	   flag+=reclength;
	   reclength=0;
       }
       printf("flaginside=%d\n",flag);
    }

    buffer[flag]='\0';
    return flag;
}
void writeto (int new_fd, char *buffer)
{
	if (write (new_fd, buffer, strlen (buffer)) == -1)
	{
		fprintf (stderr, "write error%s\n", strerror (errno));
	}
}
int send_file (char *file_name,int pcEmailId, int sockfd)
{
	char buffer[FILE_BUFFER_SIZE]="filefrcli|";
    sprintf(buffer,"filefrcli|%d|",pcEmailId);
    char *buf=strrchr(buffer,'|')+1;
	FILE *fp = fopen (file_name, "r");
	if (NULL == fp)
	{
		printf ("File:\t%s Not Found\n", file_name);
        return 1;
	}
	else
	{
		//bzero (buffer, BUFFER_SIZE);
		int file_block_length = 0;
		//            while( (file_block_length = read(fp,buffer,BUFFER_SIZE))>0)
		while ((file_block_length = fread (buf, sizeof (char), FILE_BUFFER_SIZE, fp)) > 0)
		{
			printf ("file_block_length = %d\n", file_block_length);
			//发送buffer中的字符串到new_server_socket,实际是给客户端
			/*if (send (sockfd, buffer, file_block_length, 0) < 0)
			{
				printf ("Send File:\t%s Failed\n", file_name);
				break;
			}*/
            strcat(buffer,"\0");
            printf("string sended to server:%s\n",buffer);
			writeto(sockfd,buffer);
			bzero (buffer, FILE_BUFFER_SIZE);
		}
		//            close(fp);
	}
	fclose (fp);
	printf ("File:%s Transfer Finished\n", file_name);
    return 0;
}
int send_attachedfile (char *file_name,int pcEmailId, int sockfd)
{
	char buffer[FILE_BUFFER_SIZE]="attafrcli|";
    sprintf(buffer,"attafrcli|%d|",pcEmailId);
    char *buf=strrchr(buffer,'|')+1;
	FILE *fp = fopen (file_name, "rb");
	if (NULL == fp)
	{
		printf ("File:\t%s Not Found\n", file_name);
        return 1;
	}
	else
	{
		//bzero (buffer, BUFFER_SIZE);
		int file_block_length = 0;
		//            while( (file_block_length = read(fp,buffer,BUFFER_SIZE))>0)
		while ((file_block_length = fread (buf, sizeof (char), FILE_BUFFER_SIZE, fp)) > 0)
		{
			printf ("file_block_length = %d\n", file_block_length);
			//发送buffer中的字符串到new_server_socket,实际是给客户端
			/*if (send (sockfd, buffer, file_block_length, 0) < 0)
			{
				printf ("Send File:\t%s Failed\n", file_name);
				break;
			}*/
            strcat(buffer,"\0");
            printf("string sended to server:%s\n",buffer);
			writeto(sockfd,buffer);
			bzero (buffer, FILE_BUFFER_SIZE);
		}
		//            close(fp);
	}
	fclose (fp);
	printf ("File:%s Transfer Finished\n", file_name);
    return 0;
}
int close_socket(int sockfd)
{
    close(sockfd);
    return 0;
}

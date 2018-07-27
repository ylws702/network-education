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
#include "../include/mysocket.h"
#include "../include/localfunc.h"

void CreateName(char* emailId,char* title,char* target,char* time,char* out)
{
     sprintf(out,"%s|%s|%s|%s|.txt",emailId,title,target,time);
}

int SaveSendFile(char* text,char* userid,char* emailId,char* title,char* target,char* time,int type,char *outputpath)
{ 
      printf("text:%s\t%d\n",text,strlen(text));
      char chType[10];
	   if(1==type)
		{
	   		strcpy(chType,"send");
		}
	 	else if(2==type)
		{
	   		strcpy(chType,"recv");
		}
	 	else if(3==type)
		{
	   		strcpy(chType,"copy");
		}
		else if(4==type)
		{
	   		strcpy(chType,"draft");
		}
		else
		{
	  		return -1;
		}
   FILE* fp=NULL;
   char path[100];
   char filename[50];
   CreateName( emailId,title,target,time,filename);
   sprintf(path,"./data/%s/%s/%s",userid,chType,filename);
   strcpy(outputpath,path);
   printf("%s\n",path);
   fp=fopen(path,"w");
   if(fp==NULL)
   {
      return -1;
   }
   //fprintf(fp,"%s",text);
   //int write_length = fwrite (text, sizeof (char), strlen(text), fp);
   int write_length = fwrite (text, sizeof (char),FILE_BUFFER_SIZE, fp);
	if (write_length < 0)
	{
		printf ("File:\t%s Write Failed\n", path);
	}
   close(fp);
   return 0;
}
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
       if( (reclength=read(sockfd,buffer+flag,bufflength-flag))==-1 )
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
    //printf("buffer:%d\n",buffer);
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





/***************************************************************************************
* Function Name :  GetNewMailText
*
* Description   :  接收邮件的文本内容，并把邮件文件保存在本地
*
* Date          :  2012－9－8
*
* Parameter     : mailId:邮件的ID;file_name:接收文件存放的路径;
*
* Return Code   :  >0 socket套接字标示符，
*				   -1 表示socket发送失败
*                  -2 表示socket接收失败
＊                                     1 表示打开文件路径失败
＊                                     2 表示写入文件出错
* Author        :  王明生
****************************************************************************************/
int SendMailText(int mailId,char* file_name)
{
    char buffer[BUFFERSIZE];
    int length=0;

    int sockfd=connect_socket(SERVERIP,SERVERPORT);

   //发送命令
   sprintf(buffer,"filefrcli|%d",mailId);
   if( send_msg(sockfd,buffer,BUFFERSIZE)==-1)
   {
       return -1;//发送失败
   }

   //打开文件做准备
   FILE *fp = fopen (file_name, "r");
   if (NULL == fp)
    {

      printf ("File:\t%s Not Found\n", file_name);
    }
    else
    {
       bzero (buffer, BUFFERSIZE);
       int file_block_length = 0;
       while ((file_block_length =fread (buffer, sizeof (char), BUFFERSIZE, fp)) > 0)
        {
              printf ("file_block_length = %d\n", file_block_length);
                //发送buffer中的字符串到new_server_socket,实际是给客户端
              if (send_msg (sockfd, buffer, file_block_length) < 0)
                {
                   printf ("Send File:\t%s Failed\n", file_name);
                   break;
                }
              bzero (buffer, BUFFERSIZE);
         }
         fclose (fp);
         printf ("File:\t%s Transfer Finished\n", file_name);     
     }

     return 0;
}

  /***************************************************************************************
* Function Name :  ChangeEmailState
*
* Description   :  修改iState
*
* Date          :  2012－9－9
*
* Parameter     : 
*
* Return Code   :  >0 表示添加成功，受影响的记录数
*				   -1 表示服务器处理失败
*                  0  表示受影响的记录数为0
                   -2 表示发送失败 
* Author        :  张龙德
****************************************************************************************/
int ChangeEmailState(char * pcEmailId,int iState)
{
    char buffer[BUFFER_SIZE]={0};
	int length=0;
    int client_socket=0;
    if(pcEmailId==NULL)
    {
		return -1;
    }
	client_socket= connect_socket(SERVERIP,SERVERPORT);
	sprintf(buffer,"sqlupdate|update EmailTable set state=%d where emailId=%s",iState,pcEmailId);
  
	printf ("string sended to server:%s\n",buffer);
	if(send_msg(client_socket,buffer,BUFFER_SIZE)<0)
    {
		return -2;
    }
    bzero (buffer, BUFFER_SIZE);
    length = recv_msg(client_socket,buffer,BUFFER_SIZE);
	printf("string received from server: %s length=%d\n",buffer,length);
    close_socket(client_socket);
    return length;   
}

int  DeleteServerEmail(char * pcEmailId)
{
    char buffer[BUFFER_SIZE]={0};
	int length=0;
    int client_socket=0;
    if(pcEmailId==NULL)
    {
		return -1;
    }
	client_socket= connect_socket(SERVERIP,SERVERPORT);
	sprintf(buffer,"sqldelete|delete from EmailTable where emailId=%s",pcEmailId);
  
	printf ("delete string sended to server:%s\n",buffer);
	if(send_msg(client_socket,buffer,BUFFER_SIZE)<0)
    {
		return -2;
    }
    bzero (buffer, BUFFER_SIZE);
    length = recv_msg(client_socket,buffer,BUFFER_SIZE);
	printf("string received from server: %s length=%d\n",buffer,length);
    close_socket(client_socket);
    return length;
}
  /***************************************************************************************
* Function Name :  GetNewMail
*
* Description   :  获得未读邮件信息
*
* Date          :  2012－9－8
*
* Parameter     : pstNewMail:存储邮件信息的输出参数;iType:获取邮件类型
*
* Return Code   :  >0 获得邮件信息的条数，
*				   -1 表示发送socket失败
*                  -2 表示接收socket失败
* Author        :  王明生
****************************************************************************************/
int GetMail(stMail* pstNewMail,char * pcUserId,int iType,int state)
{  g_print("pcUserId:%s\n",pcUserId);
    char buffer[BUFFERSIZE];
    if(iType==1)
    {
        sprintf(buffer,"sqlselect|select emailId,title,content,type,userId,emaiTime,sendTargetId,copyTargetId from EmailTable where sendTargetId='%s' and state=%d and type=%d",pcUserId,state,iType);
	}
    else
    {
   sprintf(buffer,"sqlselect|select emailId,title,content,type,userId,emaiTime,sendTargetId,copyTargetId from EmailTable where sendTargetId='%s' and type=%d",pcUserId,iType);
    }
   int sockfd=connect_socket(SERVERIP,SERVERPORT);
   if( send_msg(sockfd,buffer,BUFFERSIZE)==-1)
   {
       return -1;//发送失败
   }
   if( recv_msg(sockfd,buffer,BUFFERSIZE)<1)
   {
       return -2;//接收失败
   }
   if(strchr(buffer,'|')==NULL)
		return 0;
   char* phead=buffer;
   char* plast=buffer;
   char tmp[50];
   int i=0;
   int j=1;
   while(*phead!='\0')
   {
    plast=strchr(phead,'|');
   strncpy(tmp,phead,plast-phead);
   tmp[plast-phead]='\0';
   switch(j)
   {
       case 1:
           strcpy( pstNewMail[i].acEmailID,tmp);
	   j++;
	   break;
       case 2:
           strcpy( pstNewMail[i].acTitle,tmp);
	   j++;
	   break;
       case 3:
           strcpy( pstNewMail[i].acContent,tmp);
	   j++;
	   break;
       case 4:
           pstNewMail[i].iType=atoi(tmp);
	   j++;
	   break;
	   case 5:
           strcpy( pstNewMail[i].acUserID,tmp);
	   j++;
	   break;
       case 6:
           strcpy( pstNewMail[i].acEmailTime,tmp);
	   j++;
	   break;
	   case 7:
           strcpy( pstNewMail[i].acSendTargetID,tmp);
	   j++;
	   break;
       case 8:
           strcpy( pstNewMail[i].acCopyTargetID,tmp);
	   j=1;
	   break;
       default:
	   break;
   }
   phead=plast+1;
    if(*phead=='~')
    {
	phead++;
	i++;
    }
 }
   close_socket(sockfd);
   return i;
    
}

/***************************************************************************************
* Function Name :  GetMailText
*
* Description   :  接收邮件的文本内容，并把邮件文件保存在本地
*
* Date          :  2012－9－8
*
* Parameter     : mailId:邮件的ID;file_name:接收文件存放的路径;
*
* Return Code   :  >0 socket套接字标示符，
*				   -1 表示socket发送失败
*                  -2 表示socket接收失败
＊                                     1 表示打开文件路径失败
＊                                     2 表示写入文件出错
* Author        :  王明生
****************************************************************************************/
int GetMailText(int mailId,char* file_name)
{
   char buffer[BUFFERSIZE];
   int length=0;
   int sockfd=connect_socket(SERVERIP,SERVERPORT);

//发送命令
   sprintf(buffer,"filetocli|%d",mailId);
   printf("buffer:%s\n",buffer);
   if( send_msg(sockfd,buffer,BUFFERSIZE)==-1)
   {
       return -1;//发送失败
   }
   
//打开文件，准备接收文件
    FILE *fp = fopen (file_name, "w");
    if (NULL == fp)
     {
       printf ("File:\t%s Can Not Open To Write\n", file_name);
       return 1;
     }
char buf[FILEBUFFERSIZE]={0};
//接收文件内容
length = recv_msg(sockfd,buf,FILEBUFFERSIZE); 

//length = recv_msg(sockfd,buffer,BUFFERSIZE);		   
if (length < 0)
{
	printf ("Recieve Data From Server  Failed!\n");
	return -2;
}

	int write_length = fwrite (buf, sizeof (char), FILEBUFFERSIZE, fp);
	if (write_length < length)
	{
		printf ("File:\t%s Write Failed\n", file_name);
		return 2;
	}
	printf("string from server:%s\n",buf);
	printf("length:%d\n",write_length);
	close(fp);
	close_socket(sockfd);
   return 0;
}

  /***************************************************************************************
* Function Name :  GetMailattach
*
* Description   :  接收邮件的附件内容，并把附件文件保存在本地
*
* Date          :  2012－9－8
*
* Parameter     : mailId:邮件的ID;file_name:接收文件存放的路径;
*
* Return Code   :  >0 socket套接字标示符，
*				   -1 表示socket发送失败
*                  -2 表示socket接收失败
＊                                     1 表示打开文件路径失败
＊                                     2 表示写入文件出错
* Author        :  王明生
****************************************************************************************/
int GetMailattach(int mailId,char* file_name)
{
   char buffer[BUFFERSIZE];
   int length=0;
   int sockfd=connect_socket(SERVERIP,SERVERPORT);

//发送命令
   sprintf(buffer,"attatocli|%d",mailId);
   if( send_msg(sockfd,buffer,BUFFERSIZE)==-1)
   {
       return -1;//发送失败
   }

//打开文件，准备接收文件
    FILE *fp = fopen (file_name, "wb");
    if (NULL == fp)
     {
       printf ("File:\t%s Can Not Open To Write\n", file_name);
       return 1;
     }
char buf[FILEBUFFERSIZE]={0};
//接收文件内容
length = recv_msg(sockfd,buf,FILEBUFFERSIZE);
			if (length < 0)
		     {
		       printf ("Recieve Data From Server  Failed!\n");
		       return -2;
		     }
		   int write_length = fwrite (buf, sizeof (char), length, fp);
		   if (write_length < length)
		     {
		       printf ("File:\t%s Write Failed\n", file_name);
		       return 2;
		     }
   fclose(fp);
   close_socket(sockfd);
   return 0;
}

/***************************************************************************************
* Function Name :  DeleteSeverMail
*
* Description   :  删除服务器邮件
*
* Date          :  2012－9－9
*
* Parameter     : emailId:邮件id;
*
* Return Code   :   0代表删除成功
*				  
* Author        :  王明生
****************************************************************************************/
int DeleteSeverMail (char * emailId)
{
      //删除服务器中的邮件
      char buffer[BUFFERSIZE];
      sprintf(buffer,"deletmail|%s",emailId);
      int socketfd=connect_socket(SERVERIP,SERVERPORT);
      int sendlength=send_msg(socketfd,buffer,BUFFERSIZE);
      int recvlength=recv_msg(socketfd,buffer,BUFFERSIZE);
      printf("%s\n",buffer);
      close_socket(socketfd);
      return 0;
}

/***************************************************************************************
* Function Name :  DeleteSeverMail
*
* Description   :  获取联系人姓名,id信息
*
* Date          :  2012－9－12
*
* Parameter     : char *pcUserId, char *pacContact[50], int *item
*
* Return Code   :   0代表执行成功
					1代表查找不到
*				  
* Author        :  张文跃
****************************************************************************************/
int GetContactInfo(char *pcUserId, char *pacContact[50], int *item)
{
	int iBuffer = 0;
	int contactItem = 0;
	int realItem = 0;
	char temp[50];
	memset(temp, '\0', 50);
	int iTemp = 0;
	char buffer[BUFFER_SIZE];
	int length=0;
    int client_socket=0;
	int recvResult = 0;    
	memset (buffer,'\0', BUFFER_SIZE);
	client_socket= connect_socket(SERVERIP,SERVERPORT);
	sprintf(buffer, "sqlselect|SELECT contactName,contactId,teamId,itemId FROM ContactTable WHERE userId='%s'", pcUserId);
	if(send_msg(client_socket,buffer,BUFFER_SIZE)<0)
    {
		return -2;
    }
	memset(buffer,'\0',BUFFER_SIZE);
    length = recv_msg(client_socket,buffer,BUFFER_SIZE);
	//还没有联系人
	if (3 == length)
	{
		return 1;
	}
	close_socket(client_socket);
	if (-1 == length)
	{
		return -1;
	}
	else
	{
		while(buffer[iBuffer] != '\0')
		{
			if ('~' == buffer[iBuffer])
			{
				++realItem;
			}
			++iBuffer;
		}
		*item = realItem;

		printf("联系人条目数量%d\n", realItem);
		iBuffer = 0;
		for (; contactItem < 50; ++contactItem)
		{
			pacContact[contactItem] = (char *)malloc(sizeof(char) * 50);

		}
		contactItem = 0;
		while (buffer[iBuffer] != '\0')
		{
			if ( buffer[iBuffer] == '~')
			{
				strcpy(pacContact[contactItem], temp);
				++contactItem;
				iTemp = 0;
				memset(temp, '\0', 50);
			}
			else
			{
				temp[iTemp] = buffer[iBuffer];
				++iTemp;
			}
			++iBuffer;
		}
		printf("%s\n", pacContact[0]);
		return 0;
	}
}
/***************************************************************************************
* Function Name :  AddContact
*
* Description   :  添加联系人
*
* Date          :  2012－9－9
*
* Parameter     : pcUserId:用户Id;pcContactId:联系人Id；pcContactName 联系人名字，iTeamId：组名
*
* Return Code   :  >0 表示添加成功，受影响的记录数
*				   -1 表示服务器处理失败
*                  0  表示受影响的记录数为0
                   -2 表示发送失败 
* Author        :  张龙德
* Reviser		:  张文跃
****************************************************************************************/
int AddContact (char * pcUserId,char * pcContactId, char * pcContactName, int iTeamId)
{
    char buffer[BUFFER_SIZE]={0};
	int length=0;
    int client_socket=0;
	int recvResult;    
	memset (buffer,'\0', BUFFER_SIZE);
	client_socket= connect_socket(SERVERIP,SERVERPORT);
    sprintf(buffer,"sqlinsert|insert into ContactTable values(NULL,'%s','%s','%s',%d)",pcUserId, pcContactName, pcContactId, iTeamId);
	printf ("string sended to server:%s\n",buffer);
	if(send_msg(client_socket,buffer,BUFFER_SIZE)<0)
    {
		return -2;
    }
	memset(buffer,'\0',BUFFER_SIZE);
    length = recv_msg(client_socket,buffer,BUFFER_SIZE);
	printf("发送返回值%s\n", buffer);
	close_socket(client_socket);
	if (-1 == length)
	{
		return -1;
	}
	else
	{
		recvResult = strcmp(buffer, "1");
		if (recvResult == 0)
			return 0;
		else
			return 1;
	}
}

/***************************************************************************************
* Function Name :  DeleteContact
*
* Description   :  删除联系人
*
* Date          :  2012－9－9
*
* Parameter     : pcUserId:用户Id;pcContactId:联系人Id；
*
* Return Code   :  >0 表示添加成功，受影响的记录数
*				   -1 表示服务器处理失败
*                  0  表示受影响的记录数为0
                   -2 表示发送失败 
* Author        :  张龙德
* Reviser       :  张文跃
****************************************************************************************/
int DeleteContact (char * pcUserId, int itemId)
{
    char buffer[BUFFER_SIZE]={0};
    int teamId=1;
	int length=0;
    int client_socket=0;
	int recvResult;
	memset(buffer, '\0', BUFFER_SIZE);
	client_socket= connect_socket(SERVERIP,SERVERPORT);
    sprintf(buffer,"sqldelete|delete from ContactTable where userId='%s' and itemId=%d",pcUserId,itemId);
	printf ("string sended to server:%s\n",buffer);
	if(send_msg(client_socket,buffer,BUFFER_SIZE)<0)
    {
		return -2;
    }
	memset(buffer, '\0', BUFFER_SIZE);
    length = recv_msg(client_socket,buffer,BUFFER_SIZE);
    close_socket(client_socket);
	if (-1 == length)	
	{
		printf("接受数据失败!\n");
		return -1;
	}
	recvResult = strcmp(buffer, "1");
	printf("返回添加联系人影响的行数：%d\n", recvResult);
	if (recvResult == 0)
		return 0;
	else
		return 1;
}

  /***************************************************************************************
* Function Name :  ReviseContact
*
* Description   :  修改contactName或者teamId
*
* Date          :  2012－9－9
*
* Parameter     : pcUserId:用户Id;pcContactId:联系人Id；pcTarget：要修改字段的内容；pcFieldType：要修改的字段名；
*
* Return Code   :  >0 表示添加成功，受影响的记录数
*				   -1 表示服务器处理失败
*                  0  表示受影响的记录数为0
                   -2 表示发送失败 
* Author        :  张龙德
＊ Revise        :  张文跃
****************************************************************************************/
int ReviseContact(char * pcUserId,int iItemId, char * pcTargetID,char * pcTargetName)
{
    char buffer[BUFFER_SIZE]={0};
    int teamId=1;
	int length=0;
    int client_socket=0;
	int recvResult;
	memset(buffer, '\0', BUFFER_SIZE);
	client_socket= connect_socket(SERVERIP,SERVERPORT);
	sprintf(buffer,"sqlupdate|update ContactTable set contactId = '%s',contactName = '%s' where userId='%s' and itemId=%d", pcTargetID, pcTargetName, pcUserId, iItemId);
	printf ("string sended to server:%s\n",buffer);
	if(send_msg(client_socket,buffer,BUFFER_SIZE)<0)
    {
		return -2;
    }
    memset(buffer, '\0', BUFFER_SIZE);
    length = recv_msg(client_socket,buffer,BUFFER_SIZE);
	close_socket(client_socket);
	if (-1 == length)
	{
		return -1;
	}
	else
	{
		recvResult = strcmp(buffer, "1");
		printf("返回更新联系人影响的行数：%d\n", recvResult);
		if (recvResult == 0)
		{
			return 0;
		}
		else
			return 1; 
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
	{   printf ("File:\t%s Found\n", file_name);
		//bzero (buffer, BUFFER_SIZE);
		int file_block_length = 0;
		//            while( (file_block_length = read(fp,buffer,BUFFER_SIZE))>0)
		//while ((file_block_length = fread (buf, sizeof (char), FILE_BUFFER_SIZE, fp)) > 0)
		//{
			
			//发送buffer中的字符串到new_server_socket,实际是给客户端
			/*if (send (sockfd, buffer, file_block_length, 0) < 0)
			{
				printf ("Send File:\t%s Failed\n", file_name);
				break;
			}*/
            file_block_length = fread (buf, sizeof (char), FILE_BUFFER_SIZE, fp);
            printf ("file_block_length = %d\n", file_block_length);
            //strcat(buffer,"\0");
             printf("string sended to server:%s\n",buffer);
			writeto(sockfd,buffer);
			//bzero (buffer, FILE_BUFFER_SIZE);
		//}
		//            close(fp);
	}
	fclose (fp);
    
   
	printf ("File:%s Transfer Finished\n", file_name);
    return 0;
}

/***************************************************************************************
* Function Name :  SendEmail
*
* Description   :  向服务器端发送邮件信息和邮件文本，如果存在附件，则一起发送附件
*
* Date          :  2012－9－10
*
* Parameter     : pstNewMail:邮件结构题指针，其中content为邮件文本本地相对路径，
*				  acAttachedFilePath为附件本地相对路径，acAttachedFilePath为NULL时，该封邮件不存在附件
* Return Code   :  0 发送成功
                                   －1 发送邮件信息失败
                                   －2 接收emailId失败
                  -3 接收到的emailId不合法
                  -4  发送邮件文本失败
                  -5  发送附件失败
* Author        :  张龙德
****************************************************************************************/
int SendEmail(stMail* pstNewMail,char * text)
{	  //printf("text:%s\n",text);
	  char buffer[FILE_BUFFER_SIZE]={0};
	  int length=0;
	  char* msg=NULL;
	  int client_socket=0;
      sprintf(buffer,"sqlinsert|insert into EmailTable values(NULL,'%s','%s',%d,%d,'%s','%s','%s',%d,'%s','%s')",pstNewMail->acTitle,pstNewMail->acContent,pstNewMail->iType,pstNewMail->iState,pstNewMail->acSendTargetID,pstNewMail->acCopyTargetID,pstNewMail->acEmailTime,pstNewMail->iEmailSize,pstNewMail->acAttachedFilePath,pstNewMail->acUserID);
	  
	  client_socket= connect_socket(SERVERIP,SERVERPORT);   
      
	  printf ("string sended to server:%s\n",buffer);
	  length=send_msg(client_socket,buffer,FILE_BUFFER_SIZE);
      if(length<0)
      {
		 return -1;
      }
	  bzero (buffer, FILE_BUFFER_SIZE);
      printf("SERVERIP=%s   SERVERPORT=%d",SERVERIP,SERVERPORT);
	  length = recv_msg(client_socket,buffer,FILE_BUFFER_SIZE);//从服务器端接受到emailId
      printf("SERVERIP=%s   SERVERPORT=%d",SERVERIP,SERVERPORT);
      if(length<0)
      {
		return -2;
      }
	  printf("string received from server: %s length=%d\n",buffer,length);

      int iEmailId=atoi(buffer);
      if(iEmailId<1)
      {
		 printf("error!\tiEmailId:%d\n",iEmailId);
         return -3;
      }
	  else
      { 
          char acEmailID[10]={0};
          sprintf(acEmailID,"%d",iEmailId);
          SaveSendFile(text,pstNewMail->acUserID,acEmailID,pstNewMail->acTitle,pstNewMail->acSendTargetID,pstNewMail->acEmailTime,pstNewMail->iType,pstNewMail->acContent);//将该封邮件保存到本地
		  close_socket(client_socket);
          client_socket= connect_socket(SERVERIP,SERVERPORT);
		  printf("emailId from server:%d\n",iEmailId);
		  
		  int iSendFileRet=send_file(pstNewMail->acContent,iEmailId,client_socket);
          if(iSendFileRet!=0)
          {
               printf("send_file failed\n");
               return -4;
          }
          if(strcmp(pstNewMail->acAttachedFilePath,"NULL")!=0)
          {  printf("pstNewMail->acAttachedFilePath:%s\n",pstNewMail->acAttachedFilePath);
            close_socket(client_socket);
            client_socket= connect_socket(SERVERIP,SERVERPORT);
		iSendFileRet=send_attachedfile(pstNewMail->acAttachedFilePath,iEmailId,client_socket);//先发附件后发邮件产生问题
              if(iSendFileRet!=0)
              {
               printf("send_attachedfile\n");
               return -5;
              }
             //SaveSendFile(text,email.acUserID,char* emailId,char* title,char* target,char* time,char *outputpath);
          }
          close_socket(client_socket);
      }  
	  close_socket(client_socket);
    return 0;
}


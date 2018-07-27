#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "clientLib.h" 
#include "mysocket.h"
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
* Function Name :  GetNewMail
*
* Description   :  获得未读邮件信息
*
* Date          :  2012－9－8
*
* Parameter     : pstNewMail:存储邮件信息的输出参数;state:获取邮件类型
*
* Return Code   :  >0 获得邮件信息的条数，
*				   -1 表示发送socket失败
*                  -2 表示接收socket失败
* Author        :  王明生
****************************************************************************************/
int GetMail(stMail* pstNewMail,int iType,int state)
{
    char buffer[BUFFERSIZE];
    if(iType==2)
    {
        sprintf(buffer,"sqlselect|select emailId,title,content,type from EmailTable where state=%d and iType=%d",state,iType);
	}
   sprintf(buffer,"sqlselect|select emailId,title,content,type from EmailTable where state=%d",iType);
   int sockfd=connect_socket(SERVERIP,SERVERPORT);
   if( send_msg(sockfd,buffer,BUFFERSIZE)==-1)
   {
       return -1;//发送失败
   }
   if( recv_msg(sockfd,buffer,BUFFERSIZE)==-1)
   {
       return -2;//接收失败
   }
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
		   int write_length = fwrite (buf, sizeof (char), length, fp);
		   if (write_length < length)
		     {
		       printf ("File:\t%s Write Failed\n", file_name);
		       return 2;
		     }
       printf("write_length:%d\n",write_length);
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
* Function Name :  AddContact
*
* Description   :  添加联系人
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
****************************************************************************************/
int AddContact (char * pcUserId,char * pcContactId,char * pcTeamId,int argc,char* argv[])
{
    char buffer[BUFFER_SIZE]={0};
	int length=0;
    int client_socket=0;
	client_socket= connect_socket(argv[1],atoi(argv[2]));
    sprintf(buffer,"sqlinsert|insert into ContactTable values(NULL,'%s','%s','%s',%d)",pcUserId,pcUserId,pcContactId,pcTeamId);
	printf ("string sended to server:%s\n",buffer);
	if(send_msg(client_socket,buffer,BUFFER_SIZE)<0)
    {
		return -2;
    }
	//memset(buffer,0,strlen(buffer));
    bzero (buffer, BUFFER_SIZE);
    length = recv_msg(client_socket,buffer,BUFFER_SIZE);
	printf("string received from server: %s length=%d\n",buffer,length);
    close_socket(client_socket);
    return length;	
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
****************************************************************************************/
int DeleteContact (char * pcUserId,char * pcContactId,int argc,char* argv[])
{
    char buffer[BUFFER_SIZE]={0};
    int teamId=1;
	int length=0;
    int client_socket=0;
	client_socket= connect_socket(argv[1],atoi(argv[2]));
    sprintf(buffer,"sqldelete|delete from ContactTable where userId='%s' and contactId='%s'",pcUserId,pcContactId);
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
****************************************************************************************/
int ReviseContact(char * pcUserId,char * pcContactId,char * pcTarget,char * pcFieldType,int argc,char* argv[])
{
    char buffer[BUFFER_SIZE]={0};
    int teamId=1;
	int length=0;
    int client_socket=0;
	client_socket= connect_socket(argv[1],atoi(argv[2]));
	sprintf(buffer,"sqlupdate|update ContactTable set %s = '%s' where userId='%s' and contactId='%s'" ,pcFieldType,pcTarget,pcUserId,pcContactId);
  
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

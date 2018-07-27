#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "mysocket.h"
#include "clientLib.h"


void GetEmail();
int SendEmail(stMail* pstNewMail);
void GetAttachedFile();

int main (int argc,char* argv[])
{
    //DeleteSeverMail ("94");//删除邮件
	//SendEmail(argc,argv);
    //AddContact ("wangmingsheng","zhangwenyue","3",argc,argv);
    // DeleteContact("wms","zhangwenyue",argc,argv);
    //ReviseContact("zld","zhangwenyue","2","teamId",argc,argv);
    //GetEmail();
    //GetAttachedFile();
    exit(0);
}
void GetAttachedFile()
{
	stMail pstNewMail[50];
    int i=GetMail(pstNewMail,1,1);
    printf("%s\n",pstNewMail[1].acEmailID);
    GetMailattach(atoi(pstNewMail[8].acEmailID),"zhanglongde2");
    printf("target record number:%d\n",i);
}

void GetEmail()
{
	stMail pstNewMail[50];
    int i=GetMail(pstNewMail,1,1);
    printf("%s\n",pstNewMail[1].acEmailID);
    GetMailText(atoi(pstNewMail[1].acEmailID),"zhanglongde");
    printf("target record number:%d\n",i);
}

/***************************************************************************************
* Function Name :  SendEmail
*
* Description   :  想服务器端发送邮件信息和邮件文本，如果存在附件，则一起发送附件
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
int SendEmail(stMail* pstNewMail)
{		 
	  char buffer[1024]={0};
	  int length=0;
	  char* msg=NULL;
	  int client_socket=0;
      sprintf(buffer,"sqlinsert|insert into EmailTable values(NULL,'%s','%s',%d,%d,'%s','%s','%s',%d,'%s','%s')",pstNewMail->acTitle,pstNewMail->acContent,pstNewMail->iType,pstNewMail->iState,pstNewMail->acSendTargetID,pstNewMail->acCopyTargetID,pstNewMail->acCopyTargetID,pstNewMail->acEmailTime,pstNewMail->iEmailSize,pstNewMail->iEmailSize,pstNewMail->acAttachedFilePath,pstNewMail->iEmailSize,pstNewMail->acUserID);
	  
	  client_socket= connect_socket(SERVERIP,SERVERPORT);    
	  printf ("string sended to server:%s\n",buffer);
	  length=send_msg(client_socket,buffer,BUFFER_SIZE);
      if(length<0)
      {
		 return -1;
      }
	  bzero (buffer, BUFFER_SIZE);
	  length = recv_msg(client_socket,buffer,BUFFER_SIZE);//从服务器端接受到emailId
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
		  close_socket(client_socket);
          client_socket= connect_socket(SERVERIP,SERVERPORT);
		  printf("emailId from server:%d\n",iEmailId);
		  
		  int iSendFileRet=send_file(pstNewMail->acContent,iEmailId,client_socket);
          if(iSendFileRet!=0)
          {
               printf("send_file failed\n");
               return －4;
          }
          if(strcmp(pstNewMail->acAttachedFilePath,"NULL")!=0)
          {
            close_socket(client_socket);
            client_socket= connect_socket(SERVERIP,SERVERPORT);
		SendFileRet=send_attachedfile(pstNewMail->acAttachedFilePath,iEmailId,client_socket);//先发附件后发邮件产生问题
              if(iSendFileRet!=0)
              {
               printf("send_attachedfile\n");
               return －5;
             }
          }
          close_socket(client_socket);
      }  
	  close_socket(client_socket);
    return 0;
}


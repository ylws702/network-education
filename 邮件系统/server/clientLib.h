#ifndef CLIENTLIB_H
#define CLIENTLIB_H 

#define BUFFER_SIZE 1024
#define BUFFERSIZE 1024
#define FILEBUFFERSIZE 1024*10
#define SERVERIP "127.0.0.1"
#define SERVERPORT 1234

typedef struct tagMAIL_t
{
   char acEmailID[25];//邮件ID
   char acTitle[50];//邮件标题
   char acContent[50];//邮件内容
   int  iType;//邮件类型reserved/send/receive/copy/draft/…
   int  iState;//0保留/1已经读过/2未读过/3已经回复/4未回复/…
   char acSendTargetID[500];//多个目标用户ID串，以分号作为分隔符
   char acCopyTargetID[500];//多个目标用户ID串，以分号作为分隔符
   char acEmailTime[20];//邮件发送时间/接收时间/草稿保存时间/邮件回复时间
   int  iEmailSize;//邮件内容大小;注意取值范围0-1MB
   char acAttachedFilePath[50];//多个附件存放路径字符串，以分号作为分隔符；
   char  acUserID[25];//邮件所属用户ID(邮件撰写者)
}stMail;

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
int SendMailText(int mailId,char* file_name);

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
int GetMail(stMail* pstNewMail,int iType,int state);

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
int GetMailText(int mailId,char* file_name);

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
int GetMailattach(int mailId,char* file_name);

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
int DeleteSeverMail (char * emailId);

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
int AddContact (char * pcUserId,char * pcContactId,char * pcTeamId,int argc,char* argv[]);

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
int DeleteContact (char * pcUserId,char * pcContactId,int argc,char* argv[]);

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
int ReviseContact(char * pcUserId,char * pcContactId,char * pcTarget,char * pcFieldType,int argc,char* argv[]);

#endif

#ifndef MYSOCKET_H
#define MYSOCKET_H
#include "../include/DataStruct.h"
int connect_socket(char* server,int);
int send_msg(int socket,char* sendBuff,int length);
int recv_msg(int socket,char* buffer,int bufflength);
int close_socket(int socket);

void writeto (int new_fd, char *buffer);
int send_file (char *file_name, int pcEmailId,int sockfd);
int send_attachedfile (char *file_name,int pcEmailId, int sockfd);

int ChangeEmailState(char * pcEmailId,int iState);

int  DeleteServerEmail(char * pcEmailId);

int SendEmail(stMail* pstNewMail,char * text);


void CreateName(char* emailId,char* title,char* target,char* time,char* out);
int SaveSendFile(char* text,char* userid,char* emailId,char* title,char* target,char* time,int type,char *outputpath);
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
* Parameter     : pstNewMail:存储邮件信息的输出参数;iType:获取邮件类型
*
* Return Code   :  >0 获得邮件信息的条数，
*				   -1 表示发送socket失败
*                  -2 表示接收socket失败
* Author        :  王明生
****************************************************************************************/
int GetMail(stMail* pstNewMail,char * pcUserId,int iType,int state);

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
int GetContactInfo(char *pcUserId, char *pacContact[50], int *item);

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
int AddContact (char * pcUserId,char * pcContactId, char * pcContactName, int iTeamId);

/***************************************************************************************
* Function Name :  DeleteContact
*
* Description   :  删除联系人
*
* Date          :  2012－9－9
*
* Parameter     : pcUserId:用户Id;itemId:条目信息；
*
* Return Code   :  >0 表示添加成功，受影响的记录数
*				   -1 表示服务器处理失败
*                  0  表示受影响的记录数为0
                   -2 表示发送失败 
* Author        :  张龙德
****************************************************************************************/
int DeleteContact (char * pcUserId,int itemId);

  /***************************************************************************************
* Function Name :  ReviseContact
*
* Description   :  修改contactName或者teamId
*
* Date          :  2012－9－9
*
* Parameter     : pcUserId:用户Id;iItemId:联系人条目；pcTarget：要修改字段的内容；pcFieldType：要修改的字段名；
*
* Return Code   :  >0 表示添加成功，受影响的记录数
*				   -1 表示服务器处理失败
*                  0  表示受影响的记录数为0
                   -2 表示发送失败 
* Author        :  张龙德
****************************************************************************************/
int ReviseContact(char * pcUserId,int iItemId, char * pcTargetID,char * pcTargetName);

#endif

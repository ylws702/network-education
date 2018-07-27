/**************************************************************************************/
/*      著作权所有者     ： 张文跃         								                                                   */
/*      文件名               ： LoginModule.c                                                  */
/*      内容                  ： 登录模块的函数     								                                   */
/*      作成日期  作者   ：                                                         					         			   */
/*      修正日期  作者   ：                                                         									   */
/**************************************************************************************/

#include "../include/LoginModule.h"
#include "../include/mysocket.h"
#include "../include/DataStruct.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

/***************************************************************************************
* Function Name :  CheckLoginInput
*
* Description   :  分析用户输入的帐号密码是否合法
*
* Date          :  2012－9－6
*
* Parameter     :  char *pcUserID,char* pcPasswd
*
* Return Code   :  0 成功
*				   1 帐户密码有非法字符  
*				   2 密码有非法字符
*				   3 密码太短
*
* Author        :  张文跃 
****************************************************************************************/
int CheckLoginInput(char *pcUserID, char* pcPasswd)
{
	int iPasswdLength;
	int iUserIdLength;
	int i;
	iUserIdLength = strlen(pcUserID);
	iPasswdLength = strlen(pcPasswd);
	char *pUserId = pcUserID;
	//遍历分析用户输入的帐号字符
	for ( i = 0; i < iUserIdLength; i++)
	{
		if((pcUserID[i] >= '0' && pcUserID[i] <= '9') || (pcUserID[i] >= 'a' && pcUserID[i] <= 'z') || (pcUserID[i] >= 'A' && pcUserID[i] <= 'Z'))
		{
			continue;
		}
		else
			return 1; //出错
	}
	//密码长度判断
	if (iPasswdLength < 6 || iPasswdLength > 16)
	{
		return 2;//密码长度不对
	}
	return 0;
}

/***************************************************************************************
* Function Name :  SendLoginInfoToServer
*
* Description   :  1、将命令，帐号，密码连接成数据库字符串
*				   2、调用socket函数发送字符串到服务器
*
* Date          :  2012－9－7
*
* Parameter     :  char *pcUserID,char* pcPasswd
*
* Return Code   :  0 发送成功
*				   1 调用socket失败
*
* Author        :  张文跃 
****************************************************************************************/
int SendLoginInfoToServer(char *pcUserID, char* pcPasswd)
{
	int sendResult;
	char buffer[UserIDPasswdLen];
	memset(buffer, '\0',UserIDPasswdLen);
	char event1[10]  = "filefrcli";
	char event3[10] = "sqlinsert";
	char event2[10] = "sqlupdate";
	char event[10] = "sqlselect";
	char event4[10] = "filetocli";

	sprintf(buffer, "%s|SELECT userId,state FROM UserTable WHERE userId='%s' and passwd='%s'", event, pcUserID, pcPasswd);
	isocketfd = connect_socket(SERVERIP, SERVERPORT);
	sendResult = send_msg(isocketfd, buffer, UserIDPasswdLen);
	if (sendResult == -1)
	{
		return 1;
	}
	return 0;
}
/***************************************************************************************
* Function Name :  RecvLoginResFromServer
*  WHERE userId = '1355' and passwd = 'zld'
* Description   :  1、接收服务器返回结果
*				   2、根据返回结果判断登录成功与否
* 				   3、字符流形式'|'一个字段的结束 '~'一条记录的结束   1355|xiaohei|zld|1|123|~
* Date          :  2012－9－7
*
* Parameter     :  char *receBuffer
*
* Return Code   :  0 登录成功
*				   1 用户名或者密码错误，登录失败
*                  2 已经登录，登录失败
* Author        :  张文跃 
****************************************************************************************/
int RecvLoginResFromServer()
{
	int iItemNum = 0;   //记录条目数
	char cState = ' ';    //状态
	char cUserId[25];   //存储用户Id
	int i = 0;
	int j = 0;
	char receBuffer[UserIDPasswdLen];
	memset(cUserId, '\0', 25);
	memset(receBuffer, '\0', UserIDPasswdLen);
	recv_msg(isocketfd,receBuffer,UserIDPasswdLen);
	//对获取的字符流进行处理
	printf("%s\n", receBuffer);
	while (receBuffer[i] != '\0')
	{
		//说明一个字段结束
		if (receBuffer[i] == '|' && receBuffer[i + 1] != '~')
		{
			cState = receBuffer[++i];
		}
		else if (receBuffer[i] == '|' && receBuffer[i + 1] == '~')
		{
			++iItemNum;
			++i;
		}
		else
		{
			cUserId[j] = receBuffer[i];
			++j;		
		}
		++i;
	}
	printf("item:%d\n", iItemNum);
	printf("cUserId:%s\n", cUserId);
	printf("cState:%c\n", cState);
	close_socket(isocketfd);
	if (cState == '1')//已经登录过一次
	{
		return 2;
	}
	else if (cUserId == "0" && cState == ' ')//用户名或者密码错误
	{
		return 1;
	}
	else if (iItemNum != 1 )
	{
		return 1;
	}
	else 
	{
		return 0;
	}
}

/***************************************************************************************
* Function Name :  CreateUserFolder
* 
* Description   :  1、登录成功后创建接收文件夹(recv),发送文件夹(send),草稿箱(draft)
*
* Date          :  2012－9－9
*
* Parameter     :  char *userId
*
* Return Code   :  0 创建成功
* 
* Author        :  张文跃 
****************************************************************************************/
int CreateUserFolder(char *userId)
{
	char buffer[80];
	char dBuffer[85];
	char pBuffer[109];
	char rBuffer[113];
	char sBuffer[113];
	char drBuffer[114];

	memset(buffer, '\0', 80);
	memset(pBuffer, '\0', 105);
	//获取执行程序当前路径
	getcwd(buffer, sizeof(buffer));
	//data文件夹
	sprintf(dBuffer, "%s/data", buffer);
	//用户文件夹
	sprintf(pBuffer, "%s/%s", dBuffer, userId);
	//建用户接收文件夹
	sprintf(rBuffer, "%s/recv", pBuffer);
	//用户发送文件夹
	sprintf(sBuffer, "%s/send", pBuffer);
	//用户草稿文件夹
	sprintf(drBuffer, "%s/draft", pBuffer);
	
	//创建文件夹
	mkdir(dBuffer, 0755);
	mkdir(pBuffer, 0755);
	mkdir(rBuffer, 0755);
	mkdir(sBuffer, 0755);
	mkdir(drBuffer, 0755);
	return 0;
}
